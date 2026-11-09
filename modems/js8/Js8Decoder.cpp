// SPDX-License-Identifier: GPL-3.0-or-later
#include "Js8Decoder.h"

#include "Js8Message.h"

#include <QMetaObject>
#include <QtGlobal>

namespace {
constexpr int kRate = 12000;
}

Js8Decoder::Js8Decoder(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<Js8Decode>("Js8Decode");
    m_resampler.configure(kRate);
    m_thread = std::thread([this] { decodeLoop(); });
}

Js8Decoder::~Js8Decoder()
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

void Js8Decoder::setSubmode(Js8Mode::Submode submode)
{
    if (m_config.submode == submode) return;
    m_config.submode = submode;
    reset();
}

void Js8Decoder::setDecodeDepth(int depth) { m_config.depth = qBound(1, depth, 3); }
void Js8Decoder::setRxFrequencyHz(int hz) { m_config.rxFrequencyHz = hz; }
void Js8Decoder::setTxFrequencyHz(int hz) { m_config.txFrequencyHz = hz; }
void Js8Decoder::setBand(int lowHz, int highHz)
{
    m_config.lowHz = qBound(100, lowHz, 4000);
    m_config.highHz = qBound(m_config.lowHz + 100, highHz, 5000);
}

void Js8Decoder::invalidate()
{
    m_generation.fetch_add(1, std::memory_order_acq_rel);
    std::lock_guard<std::mutex> lock(m_jobsMutex);
    m_jobs.clear();
}

void Js8Decoder::reset()
{
    m_resampler.reset();
    m_samples.clear();
    m_periodStart = QDateTime();
    m_periodId = -1;
    m_nextOutputUtcNs = 0;
    m_outputRemainder = 0;
    m_lastInputEndUtcNs = 0;
    m_captureGeneration = 0;
    m_timelineValid = false;
    invalidate();
    const Js8Mode::Parameters p = Js8Mode::parameters(m_config.submode);
    emit statusChanged(QStringLiteral("JS8 %1 RX: %2 s periods, depth %3")
                           .arg(Js8Mode::submodeName(m_config.submode))
                           .arg(p.periodSeconds)
                           .arg(m_config.depth));
}

void Js8Decoder::processAudioBlock(const AudioBlock &block)
{
    if (block.samples.isEmpty() || block.sampleRate <= 0) return;
    appendResampled(block);
}

void Js8Decoder::flushPeriod() { finishPeriod(true); }

void Js8Decoder::appendResampled(const AudioBlock &block)
{
    qint64 startNs = block.firstSampleUtcNs;
    if (startNs <= 0) {
        const qint64 durationNs = qint64(block.samples.size()) * 1000000000LL / qMax(1, block.sampleRate);
        startNs = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() * 1000000LL - durationNs;
    }
    const bool generationChanged = block.captureGeneration != 0 && m_captureGeneration != 0 &&
                                   block.captureGeneration != m_captureGeneration;
    const qint64 sampleNs = 1000000000LL / qMax(1, block.sampleRate);
    const bool jump = m_lastInputEndUtcNs > 0 &&
                      qAbs(startNs - m_lastInputEndUtcNs) > qMax<qint64>(qint64{5000000}, sampleNs * qint64{4});
    if (generationChanged || jump) {
        m_resampler.reset();
        m_samples.clear();
        m_periodId = -1;
        m_nextOutputUtcNs = 0;
        m_outputRemainder = 0;
        m_timelineValid = false;
    }
    if (block.captureGeneration != 0) m_captureGeneration = block.captureGeneration;

    const QVector<double> resampled = m_resampler.process(block.samples, block.sampleRate);
    m_lastInputEndUtcNs = startNs + qint64(block.samples.size()) * 1000000000LL / qMax(1, block.sampleRate);
    if (resampled.isEmpty()) return;
    if (m_nextOutputUtcNs <= 0) {
        m_nextOutputUtcNs = startNs - m_resampler.delayNanoseconds();
        m_outputRemainder = 0;
    }
    const Js8Mode::Parameters p = Js8Mode::parameters(m_config.submode);
    const qint64 periodNs = qint64(p.periodSeconds) * 1000000000LL;
    const int needed = p.nmax();
    for (double v : resampled) {
        const qint64 id = m_nextOutputUtcNs / periodNs;
        if (id != m_periodId) {
            if (m_periodId >= 0) finishPeriod(false);
            beginPeriod(id, m_nextOutputUtcNs);
        }
        if (!m_periodSubmitted && m_samples.size() < needed) {
            m_samples.append(float(qBound(-1.0, v, 1.0)));
            if (m_samples.size() == needed) finishPeriod(false);
        }
        m_outputRemainder += 1000000000LL;
        m_nextOutputUtcNs += m_outputRemainder / kRate;
        m_outputRemainder %= kRate;
    }
}

void Js8Decoder::beginPeriod(qint64 periodId, qint64 firstSampleUtcNs)
{
    const Js8Mode::Parameters p = Js8Mode::parameters(m_config.submode);
    const qint64 periodNs = qint64(p.periodSeconds) * 1000000000LL;
    m_periodId = periodId;
    m_periodSubmitted = false;
    m_periodStart = QDateTime::fromMSecsSinceEpoch(periodId * periodNs / 1000000LL, Qt::UTC);
    m_samples.clear();
    m_samples.reserve(p.nmax());
    const qint64 offsetNs = qMax<qint64>(qint64{0}, firstSampleUtcNs - periodId * periodNs);
    const int missing = int(qMin<qint64>(p.nmax(), offsetNs * kRate / 1000000000LL));
    // A late start inside the first second still holds complete frames of
    // the faster submodes and most of a Normal frame; pad with silence.
    m_timelineValid = missing <= kRate;
    if (missing > 0) m_samples.fill(0.0f, missing);
}

void Js8Decoder::finishPeriod(bool force)
{
    if (m_periodId < 0 || m_samples.isEmpty() || m_periodSubmitted) return;
    const Js8Mode::Parameters p = Js8Mode::parameters(m_config.submode);
    if (m_samples.size() < p.nmax()) {
        if (!force) return;           // the period ended early; nothing complete
        m_samples.resize(p.nmax());   // flush: pad
    }
    m_periodSubmitted = true;
    if (!m_timelineValid && !force) {
        emit statusChanged(QStringLiteral("JS8 period skipped: audio started too late in the period."));
        m_samples.clear();
        return;
    }
    Job job;
    job.samples = m_samples;
    job.start = m_periodStart;
    job.config = m_config;
    job.generation = m_generation.load(std::memory_order_acquire);
    m_samples.clear();
    bool accepted = false;
    {
        std::lock_guard<std::mutex> lock(m_jobsMutex);
        if (!m_stop && m_jobs.size() < 2) { m_jobs.push_back(std::move(job)); accepted = true; }
    }
    if (accepted) m_jobsAvailable.notify_one();
    else emit statusChanged(QStringLiteral("JS8 decode backlog full: period skipped."));
}

static Js8Decode toDecode(const Js8NativeEngine::Result &r, const QDateTime &start, Js8Mode::Submode submode)
{
    Js8Decode d;
    d.utc = start;
    d.submode = submode;
    d.frame = r.frame;
    d.bits = r.bits;
    d.snrDb = r.snrDb;
    d.dtSeconds = r.dtSeconds;
    d.frequencyHz = r.frequencyHz;
    d.quality = r.quality;
    const Js8Message::Unpacked u = Js8Message::unpack(r.frame, r.bits, submode);
    d.text = u.text;
    d.frameType = u.frameType;
    d.first = u.first;
    d.last = u.last;
    d.heartbeat = u.heartbeat;
    d.cq = u.cq;
    d.directed = u.directed;
    d.from = u.from;
    d.to = u.to;
    d.command = u.command;
    d.grid = u.grid;
    return d;
}

QVector<Js8Decode> Js8Decoder::decodeSamples(const QVector<float> &samples12k, const QDateTime &periodStart,
                                            const Js8NativeEngine::Configuration &config)
{
    Js8NativeEngine engine;
    QVector<Js8Decode> out;
    for (const Js8NativeEngine::Result &r : engine.decode(samples12k, config))
        out << toDecode(r, periodStart, config.submode);
    return out;
}

void Js8Decoder::decodeLoop()
{
    Js8NativeEngine engine;
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
        QVector<Js8Decode> decodes;
        try {
            for (const Js8NativeEngine::Result &r : engine.decode(job.samples, job.config))
                decodes << toDecode(r, job.start, job.config.submode);
        } catch (...) {
            QMetaObject::invokeMethod(this, [this] { emit statusChanged(QStringLiteral("JS8 decode failed; RX continues.")); },
                                      Qt::QueuedConnection);
            continue;
        }
        const quint64 generation = job.generation;
        const int period = Js8Mode::parameters(job.config.submode).periodSeconds;
        QMetaObject::invokeMethod(this, [this, decodes, generation, period] {
            if (generation != m_generation.load(std::memory_order_acquire)) return;
            for (const Js8Decode &d : decodes) emit decoded(d);
            emit periodDecoded(decodes.size(), period);
        }, Qt::QueuedConnection);
    }
}
