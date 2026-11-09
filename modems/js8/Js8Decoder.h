// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef JS8DECODER_H
#define JS8DECODER_H

#include "../../audio/AudioBlock.h"
#include "../../dsp/text/LinearResampler.h"
#include "Js8Mode.h"
#include "Js8NativeEngine.h"

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QVector>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

struct Js8Decode
{
    QDateTime utc;              // period start
    Js8Mode::Submode submode = Js8Mode::Submode::Normal;
    QString frame;
    int bits = 0;
    int snrDb = -28;
    double dtSeconds = 0.0;
    double frequencyHz = 0.0;
    double quality = 0.0;
    QString text;               // unpacked text of the frame
    int frameType = 255;
    bool first = false;
    bool last = false;
    bool heartbeat = false;
    bool cq = false;
    bool directed = false;
    QString from;
    QString to;
    QString command;
    QString grid;
};
Q_DECLARE_METATYPE(Js8Decode)

/**
 * @brief Live JS8 receiver: collects UTC periods and decodes them off-thread.
 *
 * Audio is resampled to 12 kHz and cut at UTC period boundaries of the
 * selected submode (15/10/6/30 s).  Each complete period is decoded by
 * Js8NativeEngine in a worker thread; results come back as Js8Decode.
 */
class Js8Decoder : public QObject
{
    Q_OBJECT
public:
    explicit Js8Decoder(QObject *parent = nullptr);
    ~Js8Decoder() override;

    void setLiveInputEnabled(bool enabled) { m_liveInputEnabled.store(enabled, std::memory_order_release); }
    bool liveInputEnabled() const { return m_liveInputEnabled.load(std::memory_order_acquire); }

    void setSubmode(Js8Mode::Submode submode);
    void setDecodeDepth(int depth);
    void setRxFrequencyHz(int hz);
    void setTxFrequencyHz(int hz);
    void setBand(int lowHz, int highHz);

    /// Decodes one period offline (tests, WAV analysis).
    static QVector<Js8Decode> decodeSamples(const QVector<float> &samples12k, const QDateTime &periodStart,
                                            const Js8NativeEngine::Configuration &config);

public slots:
    void reset();
    void processAudioBlock(const AudioBlock &block);
    void flushPeriod();

signals:
    void decoded(const Js8Decode &decode);
    void periodDecoded(int count, int periodSeconds);
    void statusChanged(const QString &status);

private:
    void appendResampled(const AudioBlock &block);
    void beginPeriod(qint64 periodId, qint64 firstSampleUtcNs);
    void finishPeriod(bool force);
    void invalidate();
    void decodeLoop();

    struct Job
    {
        QVector<float> samples;
        QDateTime start;
        Js8NativeEngine::Configuration config;
        quint64 generation = 0;
    };

    std::atomic<bool> m_liveInputEnabled{false};
    LinearResampler m_resampler;
    QVector<float> m_samples;
    Js8NativeEngine::Configuration m_config;
    QDateTime m_periodStart;
    qint64 m_periodId = -1;
    qint64 m_nextOutputUtcNs = 0;
    qint64 m_outputRemainder = 0;
    qint64 m_lastInputEndUtcNs = 0;
    quint64 m_captureGeneration = 0;
    bool m_timelineValid = false;
    bool m_periodSubmitted = false;

    std::atomic<quint64> m_generation{1};
    std::mutex m_jobsMutex;
    std::condition_variable m_jobsAvailable;
    std::deque<Job> m_jobs;
    bool m_stop = false;
    std::thread m_thread;
};

#endif // JS8DECODER_H
