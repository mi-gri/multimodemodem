#include "../runtime/SessionDiagnostics.h"
#include "TxAudioEngine.h"
#include "AudioDeviceCache.h"

#include <QByteArray>
#include <QIODevice>
#include <QMetaObject>
#include <QtGlobal>
#include <QtMath>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QAudioDevice>
#include <QAudioFormat>
#include <QAudioSink>
#include <QMediaDevices>
#else
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioOutput>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
namespace AudioNs = QtAudio;
#else
namespace AudioNs = QAudio;
#endif

#include <atomic>
#include <algorithm>
#include <cmath>
#include <cstring>

#include "TxOutputDevice.h"

namespace {
// Linear gain -> dB for the TX log lines.
double gainDb(double gain)
{
    return gain > 0.0 ? 20.0 * std::log10(gain) : -999.0;
}
}

// -----------------------------------------------------------------------------
// Construction
// -----------------------------------------------------------------------------

TxAudioEngine::TxAudioEngine(QObject *parent)
    : QObject(parent)
{
    m_outputMonitor.setParent(this);
    m_outputMonitor.setInterval(50);
    connect(&m_outputMonitor, &QTimer::timeout, this, &TxAudioEngine::checkOutputState);
}

TxAudioEngine::~TxAudioEngine()
{
    stopOutput();
}

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------

bool TxAudioEngine::startOutput(const QString &deviceName, std::unique_ptr<TxModulator> modulator,
                                qint64 latestStartUtcMs, std::function<bool()> authorized)
{
    SessionDiagnostics::instance().record("tx.output.request",{{"device",deviceName},{"latest_start_utc_ms",QString::number(latestStartUtcMs)}});
    stopOutput();

    if (!modulator) {
        emit errorOccurred("TX start failed: no modulator available.");
        return false;
    }

    SessionDiagnostics::instance().record("tx.output.modulator",{{"description",modulator->description()},{"sample_rate",modulator->sampleRate()}});
    m_sampleRate = modulator->sampleRate();
    m_modulator = std::move(modulator);
    m_totalSamples = 0;
    m_finishedEmitted = false; m_underruns = 0;
    m_playbackStarted = false;
    m_lastTextPosition = -1;


#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

    QAudioDevice selectedDevice = QMediaDevices::defaultAudioOutput();
    bool exactDeviceMatch = false;

    const QList<QAudioDevice> devices = QMediaDevices::audioOutputs();
    for (const QAudioDevice &device : devices) {
        if (device.description() == deviceName) {
            selectedDevice = device;
            exactDeviceMatch = true;
            break;
        }
    }

    const bool defaultRequested = deviceName.isEmpty() || deviceName == QStringLiteral("default");
    if (!defaultRequested && !exactDeviceMatch) {
        emit errorOccurred(QStringLiteral("Configured TX audio output '%1' is not available; automatic device fallback is disabled.")
                               .arg(deviceName));
        m_modulator.reset();
        return false;
    }

    QAudioFormat format;
    format.setSampleRate(m_sampleRate);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    if (!selectedDevice.isFormatSupported(format)) {
        emit errorOccurred(QString("Audio output does not support the requested TX format: %1 Hz mono signed 16-bit PCM.")
                               .arg(m_sampleRate));
        m_modulator.reset();
        return false;
    }

    m_audioOutput = new QAudioSink(selectedDevice, format, this);
    const bool lowLatencyTx = (m_modulator && m_modulator->lowLatencyTx());
    m_audioOutput->setBufferSize(lowLatencyTx ? (4096 * 2) : (4096 * 2 * 8));

#else

    QAudioDeviceInfo selectedDevice = QAudioDeviceInfo::defaultOutputDevice();
    bool exactDeviceMatch = false;

    exactDeviceMatch = AudioDeviceCache::find(QAudio::AudioOutput, deviceName, &selectedDevice);

    const bool defaultRequested = deviceName.isEmpty() || deviceName == QStringLiteral("default");
    if (!defaultRequested && !exactDeviceMatch) {
        emit errorOccurred(QStringLiteral("Configured TX audio output '%1' is not available; automatic device fallback is disabled.")
                               .arg(deviceName));
        m_modulator.reset();
        return false;
    }

    QAudioFormat format;
    format.setSampleRate(m_sampleRate);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    if (!selectedDevice.isFormatSupported(format)) {
        emit errorOccurred(QString("Audio output does not support the requested TX format: %1 Hz mono signed 16-bit PCM.")
                               .arg(m_sampleRate));
        m_modulator.reset();
        return false;
    }

    m_audioOutput = new QAudioOutput(selectedDevice, format, this);
    const bool lowLatencyTx = (m_modulator && m_modulator->lowLatencyTx());
    m_audioOutput->setBufferSize(lowLatencyTx ? (4096 * 2) : (4096 * 2 * 8));

#endif

    TxOutputDevice *device = new TxOutputDevice(m_modulator.get(), m_sampleRate, m_outputGain, this);

    device->setStartGuard(latestStartUtcMs, std::move(authorized));
    connect(device, &TxOutputDevice::audioBlockReady,
            this, &TxAudioEngine::audioBlockReady);

    connect(device, &TxOutputDevice::rttyToneStateChanged,
            this, &TxAudioEngine::rttyToneStateChanged);

    connect(device, &TxOutputDevice::progressChanged,
            this, &TxAudioEngine::progressChanged);

    connect(device, &TxOutputDevice::finished,
            this, &TxAudioEngine::handleDeviceFinished);

    if (!device->start()) {
        delete device;
        releaseAudioOutput();
        emit errorOccurred("Unable to open TX output device.");
        return false;
    }

    m_outputDevice = device;

    const quint64 generation = ++m_outputGeneration;
    connect(m_audioOutput, & 
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QAudioSink::stateChanged,
#else
            QAudioOutput::stateChanged,
#endif
            this, [this, generation]() {
                if (generation == m_outputGeneration) checkOutputState();
            }, Qt::QueuedConnection);
    m_running = true;
    m_audioOutput->start(m_outputDevice);
    if (m_audioOutput->error() != AudioNs::NoError &&
        m_audioOutput->state() == AudioNs::StoppedState) {
        const int error = static_cast<int>(m_audioOutput->error());
        releaseAudioOutput();
        m_running = false;
        m_modulator.reset();
        emit errorOccurred(QStringLiteral("TX audio backend failed to start (error %1).").arg(error));
        return false;
    }
    // Opening a sink does not prove it has pulled or played any PCM.
    m_playbackWatchdog.reset(qint64(m_audioOutput->bufferSize())*1000/qMax(1,m_sampleRate*2));
    m_outputProgressClock.start();
    m_outputMonitor.start();
    emit logMessage(QStringLiteral("TX audio opened: %1 Hz, mono PCM16, buffer %2 B, source available %3 B, state %4.")
                    .arg(m_sampleRate).arg(m_audioOutput->bufferSize())
                    .arg(device->bytesAvailable()).arg(static_cast<int>(m_audioOutput->state())));
    emit logMessage(QStringLiteral("TX device: %1; output level %2 dB; backend volume %3.").arg(deviceName.isEmpty()?QStringLiteral("default"):deviceName).arg(gainDb(m_outputGain), 0, 'f', 1).arg(m_audioOutput->volume()));
    emit progressChanged(0.0);

    return true;
}

void TxAudioEngine::stopOutput()
{
    SessionDiagnostics::instance().record("tx.output.stop");
    if (!m_running && m_audioOutput == nullptr && m_outputDevice == nullptr) {
        return;
    }

    /*
     * Manual stop can race with a queued natural-finish notification from the
     * pull device.  Mark the finish as already handled before destroying the
     * device so a late queued slot cannot emit finished()/stopped() again.
     */
    m_finishedEmitted = true;

    releaseAudioOutput();

    m_running = false;
    m_modulator.reset();

    emit stopped();
}

bool TxAudioEngine::isRunning() const
{
    return m_running;
}

int TxAudioEngine::sampleRate() const
{
    return m_sampleRate;
}

void TxAudioEngine::setOutputVolumePercent(int percent)
{
    setOutputGain(qBound(0, percent, 100) / 100.0);
}

int TxAudioEngine::outputVolumePercent() const
{
    return qRound(m_outputGain * 100.0);
}

void TxAudioEngine::setOutputGain(double gain)
{
    m_outputGain = qBound(0.0, gain, 1.0);
    TxOutputDevice *device = qobject_cast<TxOutputDevice *>(m_outputDevice);
    if (device != nullptr) {
        device->setGain(m_outputGain);
    }
}

double TxAudioEngine::outputGain() const
{
    return m_outputGain;
}

// -----------------------------------------------------------------------------
// Private slots
// -----------------------------------------------------------------------------

void TxAudioEngine::handleDeviceFinished()
{
    // Source EOF only means the last PCM buffer has been handed to Qt.
    // The sink's IdleState is the acknowledgement that its queue drained.
    checkOutputState();
}

void TxAudioEngine::checkOutputState()
{
    if (!m_running || !m_audioOutput || m_finishedEmitted) return;
    const quint64 generation=m_outputGeneration;
    const auto state = m_audioOutput->state();
    const auto error = m_audioOutput->error();
    const auto *source = static_cast<TxOutputDevice *>(m_outputDevice);
    const qint64 produced = source ? source->samplesProduced() : 0;
    const qint64 processed = m_audioOutput->processedUSecs();
    if (source && source->aborted()) {
        m_finishedEmitted = true;
        releaseAudioOutput();
        m_running = false;
        m_modulator.reset();
        emit errorOccurred(QStringLiteral("TX audio cancelled before PCM pull: authorization revoked or start deadline expired."));
        return;
    }
    if (!m_playbackStarted && produced > 0 && processed > 0) {
        m_playbackStarted = true;
        emit logMessage(QStringLiteral("TX audio playback confirmed: %1 samples pulled, %2 us processed.").arg(produced).arg(processed));
        emit started();
        if (!m_running || !m_audioOutput || generation!=m_outputGeneration) return;
    }
    if (m_playbackStarted && m_modulator) {
        const qint64 played = qMin(produced, processed * m_sampleRate / 1000000);
        const int position = m_modulator->textPositionAtSample(played);
        if (position >= 0 && position != m_lastTextPosition) {
            m_lastTextPosition = position;
            emit textPositionChanged(position);
            if (!m_running || !m_audioOutput || generation != m_outputGeneration) return;
        }
    }
    if (m_playbackStarted && state == AudioNs::IdleState && source && source->atEnd() &&
        (error == AudioNs::NoError || error == AudioNs::UnderrunError)) {
        m_finishedEmitted = true;
        emit progressChanged(1.0);
        emit finished();
        stopOutput();
        return;
    }
    const auto failure=m_playbackWatchdog.observe(m_outputProgressClock.elapsed(),produced,processed,source && source->atEnd());
    const bool stalled=failure!=TxPlaybackWatchdog::Failure::None;
    // An underrun (the sink ran dry for a moment, e.g. all cores busy with a
    // decode) is not the end of the transmission: the pull-mode sink resumes
    // with the next buffer.  Only a sink or source that stops advancing (the
    // watchdog, >= 1.5 s) or a real backend error ends it.  Aborting on every
    // underrun lost a final 73 after 1.1 s at 97 % CPU (2026-11-15).
    if (error == AudioNs::UnderrunError && state != AudioNs::StoppedState && !stalled &&
        !(source && source->atEnd())) {
        if (m_underruns++ == 0)
            emit logMessage(QStringLiteral("TX audio underrun after %1 ms (sound card ran dry for a moment, CPU load); transmission continues.")
                                .arg(processed / 1000));
        SessionDiagnostics::instance().record("tx.output.underrun", {{"processed_us", double(processed)}, {"pulled", double(produced)}, {"count", m_underruns}});
        return;
    }
    if (state == AudioNs::StoppedState || error != AudioNs::NoError || stalled) {
        m_finishedEmitted = true;
        const int errorCode = static_cast<int>(error);
        releaseAudioOutput();
        m_running = false;
        m_modulator.reset();
        // One terminal event; MainWindow's error handler releases PTT.
        emit errorOccurred(QStringLiteral("TX audio backend %1 (state %2, error %3, pulled %4 samples, processed %5 us).")
                           .arg(stalled ? (failure==TxPlaybackWatchdog::Failure::PlaybackStalled ? QStringLiteral("playback stalled") : QStringLiteral("source stalled")) : QStringLiteral("stopped unexpectedly"))
                           .arg(static_cast<int>(state)).arg(errorCode).arg(produced).arg(processed));
    }
}

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

void TxAudioEngine::releaseAudioOutput()
{
    ++m_outputGeneration;
    m_outputMonitor.stop();
    if (m_audioOutput != nullptr) {
        const auto* source=static_cast<TxOutputDevice*>(m_outputDevice);
        emit logMessage(QStringLiteral("TX audio closing: pulled %1 samples, processed %2 us, PCM peak %3/32767, output level %4 dB.")
            .arg(source?source->samplesProduced():0).arg(m_audioOutput->processedUSecs())
            .arg(source?source->peakPcm():0).arg(gainDb(m_outputGain), 0, 'f', 1));
        disconnect(m_audioOutput, nullptr, this, nullptr);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        m_audioOutput->stop();
#else
        m_audioOutput->stop();
#endif
        delete m_audioOutput;
        m_audioOutput = nullptr;
    }

    if (m_outputDevice != nullptr) {
        m_outputDevice->close();
        delete m_outputDevice;
        m_outputDevice = nullptr;
    }
}


