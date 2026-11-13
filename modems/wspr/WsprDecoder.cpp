// SPDX-License-Identifier: GPL-3.0-or-later
#include "WsprDecoder.h"

#include "WsprEngine.h"
#include "WsprMode.h"

#include <QElapsedTimer>

namespace {
constexpr int kRate = M3Wspr::Engine::kSampleRate;
constexpr int kDecodeSamples = M3Wspr::Engine::kInputSamples;           // 114 s
constexpr qint64 kPeriodNs = qint64(WsprMode::periodSeconds()) * 1000000000LL;
// The transmission starts 1 s into the period: up to 1 s missing at the
// start (RX started a moment late) costs nothing.
constexpr int kMaxMissingAtStart = kRate;
}

WsprDecoder::WsprDecoder(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<WsprSpot>("WsprSpot");
    qRegisterMetaType<QVector<WsprSpot>>("QVector<WsprSpot>");
    qRegisterMetaType<WsprDecoder::PeriodState>("WsprDecoder::PeriodState");
    m_resampler.configure(kRate);
    m_thread = std::thread([this] { decodeLoop(); });
}

WsprDecoder::~WsprDecoder()
{
    m_generation.fetch_add(1);
    {
        std::lock_guard<std::mutex> lock(m_jobsMutex);
        m_stop = true;
        m_jobs.clear();
    }
    m_jobsAvailable.notify_one();
    if (m_thread.joinable()) m_thread.join();
}

void WsprDecoder::invalidate()
{
    m_generation.fetch_add(1, std::memory_order_acq_rel);
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs.clear();
}

void WsprDecoder::reset()
{
    m_resampler.reset();
    m_samples.clear();
    m_periodUtc = QDateTime();
    m_periodId = -1;
    m_periodValid = false;
    m_periodSubmitted = false;
    m_nextOutputUtcNs = 0;
    m_outputRemainder = 0;
    m_lastInputEndUtcNs = 0;
    m_captureGeneration = 0;
    invalidate();
    emit statusChanged(QStringLiteral("WSPR RX: waiting for the next even minute"));
    emit periodStateChanged(PeriodState::Waiting, QDateTime());
}

void WsprDecoder::beginPeriod(qint64 periodId, qint64 firstSampleUtcNs)
{
    m_periodId = periodId;
    const qint64 startNs = periodId * kPeriodNs;
    m_periodUtc = QDateTime::fromMSecsSinceEpoch(startNs / 1000000LL, Qt::UTC);
    m_samples.clear();
    m_samples.reserve(kDecodeSamples);
    m_periodSubmitted = false;
    const qint64 offsetNs = qMax<qint64>(qint64{0}, firstSampleUtcNs - startNs);
    const int missing = static_cast<int>(qMin<qint64>(kDecodeSamples, offsetNs * kRate / 1000000000LL));
    m_periodValid = missing <= kMaxMissingAtStart;
    if (missing > 0 && m_periodValid) m_samples.fill(0.0f, missing);
    emit periodStateChanged(m_periodValid ? PeriodState::Collecting : PeriodState::LateStart, m_periodUtc);
    emit statusChanged(m_periodValid
                           ? QStringLiteral("WSPR RX: collecting period %1 UTC").arg(m_periodUtc.toString(QStringLiteral("HH:mm")))
                           : QStringLiteral("WSPR RX: period %1 UTC started before RX; waiting for the next even minute")
                                 .arg(m_periodUtc.toString(QStringLiteral("HH:mm"))));
}

void WsprDecoder::processAudioBlock(const AudioBlock &block)
{
    if (block.samples.isEmpty() || block.sampleRate <= 0) return;
    qint64 blockStartUtcNs = block.firstSampleUtcNs;
    if (blockStartUtcNs <= 0) {
        const qint64 durationNs = qint64(block.samples.size()) * 1000000000LL / block.sampleRate;
        blockStartUtcNs = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() * 1000000LL - durationNs;
    }
    const bool generationChanged = block.captureGeneration != 0 && m_captureGeneration != 0 &&
                                   block.captureGeneration != m_captureGeneration;
    const qint64 inputSampleNs = 1000000000LL / block.sampleRate;
    const bool timestampJump = m_lastInputEndUtcNs > 0 &&
                               qAbs(blockStartUtcNs - m_lastInputEndUtcNs) > qMax<qint64>(qint64{5000000}, inputSampleNs * 4);
    if (generationChanged || timestampJump) {
        // A gap invalidates the running period; decodes already queued stay.
        m_resampler.reset();
        m_samples.clear();
        m_periodId = -1;
        m_periodValid = false;
        m_nextOutputUtcNs = 0;
        m_outputRemainder = 0;
    }
    if (block.captureGeneration != 0) m_captureGeneration = block.captureGeneration;

    const QVector<double> resampled = m_resampler.process(block.samples, block.sampleRate);
    m_lastInputEndUtcNs = blockStartUtcNs + qint64(block.samples.size()) * 1000000000LL / block.sampleRate;
    if (resampled.isEmpty()) return;
    if (m_nextOutputUtcNs <= 0) {
        m_nextOutputUtcNs = blockStartUtcNs - m_resampler.delayNanoseconds();
        m_outputRemainder = 0;
    }
    for (double value : resampled) {
        const qint64 periodId = m_nextOutputUtcNs / kPeriodNs;
        if (periodId != m_periodId) beginPeriod(periodId, m_nextOutputUtcNs);
        if (m_periodValid && !m_periodSubmitted) {
            m_samples.append(static_cast<float>(qBound(-1.0, value, 1.0)));
            if (m_samples.size() >= kDecodeSamples) {
                m_periodSubmitted = true;
                submit(m_periodUtc, std::move(m_samples));
                m_samples = QVector<float>();
            }
        }
        m_outputRemainder += 1000000000LL;
        m_nextOutputUtcNs += m_outputRemainder / kRate;
        m_outputRemainder %= kRate;
    }
}

void WsprDecoder::decodeRecord(const QVector<float> &samples12k, const QDateTime &periodUtc)
{
    submit(periodUtc, samples12k);
}

void WsprDecoder::submit(const QDateTime &periodUtc, QVector<float> samples)
{
    Job job;
    job.samples = std::move(samples);
    job.periodUtc = periodUtc;
    job.generation = m_generation.load(std::memory_order_acquire);
    bool accepted = false;
    {
        std::lock_guard<std::mutex> lock(m_jobsMutex);
        if (!m_stop && m_jobs.size() < 2) {
            m_jobs.push_back(std::move(job));
            accepted = true;
        }
    }
    if (accepted) {
        m_jobsAvailable.notify_one();
        emit periodStateChanged(PeriodState::Decoding, periodUtc);
        emit statusChanged(QStringLiteral("WSPR RX: decoding period %1 UTC").arg(periodUtc.toString(QStringLiteral("HH:mm"))));
    } else {
        emit periodStateChanged(PeriodState::BacklogFull, periodUtc);
        emit statusChanged(QStringLiteral("WSPR decode backlog full: period %1 UTC not decoded")
                               .arg(periodUtc.toString(QStringLiteral("HH:mm"))));
    }
}

void WsprDecoder::decodeLoop()
{
    // The engine and its callsign hash table belong to this thread alone.
    M3Wspr::Engine engine;
    for (;;) {
        Job job;
        {
            std::unique_lock<std::mutex> lock(m_jobsMutex);
            m_jobsAvailable.wait(lock, [this] { return m_stop || !m_jobs.empty(); });
            if (m_stop) return;
            job = std::move(m_jobs.front());
            m_jobs.pop_front();
        }
        if (job.generation != m_generation.load(std::memory_order_acquire)) continue;
        QElapsedTimer timer;
        timer.start();
        QVector<WsprSpot> spots;
        try {
            for (const M3Wspr::Decode &d : engine.decode(job.samples.constData(), static_cast<size_t>(job.samples.size()))) {
                WsprSpot s;
                s.periodUtc = job.periodUtc;
                s.audioHz = d.audioHz;
                s.snrDb = qRound(d.snrDb);
                s.dtSeconds = d.dtSeconds;
                s.drift = static_cast<int>(d.drift);
                s.message = QString::fromStdString(d.message.text);
                s.callsign = QString::fromStdString(d.message.callsign);
                s.grid = QString::fromStdString(d.message.grid);
                s.powerDbm = d.message.powerDbm;
                s.type = d.message.type;
                spots.append(s);
            }
        } catch (...) {
            const quint64 generation = job.generation;
            QMetaObject::invokeMethod(this, [this, generation] {
                if (generation == m_generation.load()) emit statusChanged(QStringLiteral("WSPR decode failed"));
            }, Qt::QueuedConnection);
            continue;
        }
        const int ms = static_cast<int>(timer.elapsed());
        const quint64 generation = job.generation;
        const QDateTime periodUtc = job.periodUtc;
        QMetaObject::invokeMethod(this, [this, spots, periodUtc, ms, generation] {
            if (generation != m_generation.load(std::memory_order_acquire)) return;
            emit decoded(spots, periodUtc, ms);
            emit statusChanged(QStringLiteral("WSPR RX: %1 decode(s) in period %2 UTC (%3 s)")
                                   .arg(spots.size())
                                   .arg(periodUtc.toString(QStringLiteral("HH:mm")))
                                   .arg(ms / 1000.0, 0, 'f', 1));
        }, Qt::QueuedConnection);
    }
}
