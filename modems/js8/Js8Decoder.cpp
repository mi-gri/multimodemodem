// SPDX-License-Identifier: GPL-3.0-or-later
#include "Js8Decoder.h"

#include "Js8Message.h"

#include <QMetaObject>
#include <QtGlobal>

#include <array>
#include <memory>

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
    if (m_config.submode == submode && !m_captures.isEmpty()) return;
    m_config.submode = submode;
    reset();
}

void Js8Decoder::setMultiDecode(bool enabled)
{
    if (m_multiDecode == enabled && !m_captures.isEmpty()) return;
    m_multiDecode = enabled;
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

void Js8Decoder::rebuildCaptures()
{
    m_captures.clear();
    const QList<Js8Mode::Submode> modes = m_multiDecode ? Js8Mode::allSubmodes() : QList<Js8Mode::Submode>{m_config.submode};
    for (Js8Mode::Submode s : modes) {
        Capture c;
        c.submode = s;
        m_captures << c;
    }
}

void Js8Decoder::reset()
{
    m_resampler.reset();
    rebuildCaptures();
    m_nextOutputUtcNs = 0;
    m_outputRemainder = 0;
    m_lastInputEndUtcNs = 0;
    m_captureGeneration = 0;
    invalidate();
    const Js8Mode::Parameters p = Js8Mode::parameters(m_config.submode);
    emit statusChanged(QStringLiteral("JS8 %1 RX: %2 s periods, depth %3%4")
                           .arg(Js8Mode::submodeName(m_config.submode))
                           .arg(p.periodSeconds)
                           .arg(m_config.depth)
                           .arg(m_multiDecode ? QStringLiteral(", all speeds") : QString()));
}

void Js8Decoder::processAudioBlock(const AudioBlock &block)
{
    if (block.samples.isEmpty() || block.sampleRate <= 0) return;
    appendResampled(block);
}

void Js8Decoder::flushPeriod()
{
    for (Capture &c : m_captures) finishPeriod(c, true);
}

void Js8Decoder::appendResampled(const AudioBlock &block)
{
    if (m_captures.isEmpty()) rebuildCaptures();
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
        rebuildCaptures();
        m_nextOutputUtcNs = 0;
        m_outputRemainder = 0;
    }
    if (block.captureGeneration != 0) m_captureGeneration = block.captureGeneration;

    const QVector<double> resampled = m_resampler.process(block.samples, block.sampleRate);
    m_lastInputEndUtcNs = startNs + qint64(block.samples.size()) * 1000000000LL / qMax(1, block.sampleRate);
    if (resampled.isEmpty()) return;
    if (m_nextOutputUtcNs <= 0) {
        m_nextOutputUtcNs = startNs - m_resampler.delayNanoseconds();
        m_outputRemainder = 0;
    }
    for (double v : resampled) {
        const float sample = float(qBound(-1.0, v, 1.0));
        for (Capture &c : m_captures) {
            const Js8Mode::Parameters p = Js8Mode::parameters(c.submode);
            const qint64 periodNs = qint64(p.periodSeconds) * 1000000000LL;
            const qint64 id = m_nextOutputUtcNs / periodNs;
            if (id != c.periodId) {
                if (c.periodId >= 0) finishPeriod(c, false);
                beginPeriod(c, id, m_nextOutputUtcNs);
            }
            if (!c.submitted && c.samples.size() < p.nmax()) {
                c.samples.append(sample);
                if (c.samples.size() == p.nmax()) finishPeriod(c, false);
            }
        }
        m_outputRemainder += 1000000000LL;
        m_nextOutputUtcNs += m_outputRemainder / kRate;
        m_outputRemainder %= kRate;
    }
}

void Js8Decoder::beginPeriod(Capture &c, qint64 periodId, qint64 firstSampleUtcNs)
{
    const Js8Mode::Parameters p = Js8Mode::parameters(c.submode);
    const qint64 periodNs = qint64(p.periodSeconds) * 1000000000LL;
    c.periodId = periodId;
    c.submitted = false;
    c.start = QDateTime::fromMSecsSinceEpoch(periodId * periodNs / 1000000LL, Qt::UTC);
    c.samples.clear();
    c.samples.reserve(p.nmax());
    const qint64 offsetNs = qMax<qint64>(qint64{0}, firstSampleUtcNs - periodId * periodNs);
    const int missing = int(qMin<qint64>(p.nmax(), offsetNs * kRate / 1000000000LL));
    // A late start inside the first second still holds complete frames of
    // the faster submodes and most of a Normal frame; pad with silence.
    c.timelineValid = missing <= kRate;
    if (missing > 0) c.samples.fill(0.0f, missing);
}

void Js8Decoder::finishPeriod(Capture &c, bool force)
{
    if (c.periodId < 0 || c.samples.isEmpty() || c.submitted) return;
    const Js8Mode::Parameters p = Js8Mode::parameters(c.submode);
    if (c.samples.size() < p.nmax()) {
        if (!force) return;           // the period ended early; nothing complete
        c.samples.resize(p.nmax());   // flush: pad
    }
    c.submitted = true;
    if (!c.timelineValid && !force) {
        if (c.submode == m_config.submode)
            emit statusChanged(QStringLiteral("JS8 period skipped: audio started too late in the period."));
        c.samples.clear();
        return;
    }
    Job job;
    job.samples = c.samples;
    job.start = c.start;
    job.config = m_config;
    job.config.submode = c.submode;
    job.generation = m_generation.load(std::memory_order_acquire);
    c.samples.clear();
    bool accepted = false;
    {
        std::lock_guard<std::mutex> lock(m_jobsMutex);
        const size_t limit = size_t(2 * qMax(1, int(m_captures.size())));
        if (!m_stop && m_jobs.size() < limit) { m_jobs.push_back(std::move(job)); accepted = true; }
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
    d.extra = u.extra;
    d.grid = u.grid;
    d.compound = u.frameType == 1;   // Varicode::FrameCompound
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
    // One engine per submode keeps its FFT plans across periods.
    std::array<std::unique_ptr<Js8NativeEngine>, 5> engines;
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
        auto &engine = engines[size_t(qBound(0, int(job.config.submode), 4))];
        if (!engine) engine = std::make_unique<Js8NativeEngine>();
        try {
            for (const Js8NativeEngine::Result &r : engine->decode(job.samples, job.config))
                decodes << toDecode(r, job.start, job.config.submode);
        } catch (...) {
            QMetaObject::invokeMethod(this, [this] { emit statusChanged(QStringLiteral("JS8 decode failed; RX continues.")); },
                                      Qt::QueuedConnection);
            continue;
        }
        const quint64 generation = job.generation;
        const int submode = int(job.config.submode);
        QMetaObject::invokeMethod(this, [this, decodes, generation, submode] {
            if (generation != m_generation.load(std::memory_order_acquire)) return;
            for (const Js8Decode &d : decodes) emit decoded(d);
            emit periodDecoded(decodes.size(), submode);
        }, Qt::QueuedConnection);
    }
}
