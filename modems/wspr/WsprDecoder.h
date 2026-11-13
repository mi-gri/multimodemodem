// SPDX-License-Identifier: GPL-3.0-or-later
//
// Live WSPR-2 receiver: collects 12 kHz audio aligned to even UTC minutes and
// decodes each period on its own thread (M3Wspr::Engine) as soon as the 114 s
// wsprd reads are in.
#pragma once

#include "../../audio/AudioBlock.h"
#include "../../dsp/text/LinearResampler.h"

#include <QDateTime>
#include <QMetaType>
#include <QObject>
#include <QString>
#include <QVector>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

struct WsprSpot
{
    QDateTime periodUtc;      // start of the 2-minute period
    double audioHz = 0.0;
    int snrDb = 0;
    double dtSeconds = 0.0;
    int drift = 0;
    QString message;          // "K1ABC FN42 37"
    QString callsign;         // without <>; empty if the hash is unknown
    QString grid;             // 4 or 6 characters, empty for type 2
    int powerDbm = 0;
    int type = 0;
};
Q_DECLARE_METATYPE(WsprSpot)
Q_DECLARE_METATYPE(QVector<WsprSpot>)

class WsprDecoder : public QObject
{
    Q_OBJECT
public:
    enum class PeriodState { Waiting, Collecting, LateStart, Decoding, BacklogFull };
    Q_ENUM(PeriodState)

    explicit WsprDecoder(QObject *parent = nullptr);
    ~WsprDecoder() override;

    void setLiveInputEnabled(bool enabled) { m_liveInputEnabled.store(enabled, std::memory_order_release); }
    bool liveInputEnabled() const { return m_liveInputEnabled.load(std::memory_order_acquire); }

public slots:
    void reset();
    void processAudioBlock(const AudioBlock &block);
    // Decodes a 12 kHz record (WAV analysis); results as for live periods.
    void decodeRecord(const QVector<float> &samples12k, const QDateTime &periodUtc);

signals:
    void decoded(const QVector<WsprSpot> &spots, const QDateTime &periodUtc, int milliseconds);
    // English text for the diagnostics log.
    void statusChanged(const QString &status);
    // Period progress for the (translated) status line.
    void periodStateChanged(WsprDecoder::PeriodState state, const QDateTime &periodUtc);

private:
    struct Job {
        QVector<float> samples;
        QDateTime periodUtc;
        quint64 generation = 0;
    };
    void beginPeriod(qint64 periodId, qint64 firstSampleUtcNs);
    void submit(const QDateTime &periodUtc, QVector<float> samples);
    void invalidate();
    void decodeLoop();

    std::atomic<bool> m_liveInputEnabled{false};
    LinearResampler m_resampler;
    QVector<float> m_samples;
    QDateTime m_periodUtc;
    qint64 m_periodId = -1;
    bool m_periodValid = false;
    bool m_periodSubmitted = false;
    qint64 m_nextOutputUtcNs = 0;
    qint64 m_outputRemainder = 0;
    qint64 m_lastInputEndUtcNs = 0;
    quint64 m_captureGeneration = 0;

    std::atomic<quint64> m_generation{1};
    std::mutex m_jobsMutex;
    std::condition_variable m_jobsAvailable;
    std::deque<Job> m_jobs;
    bool m_stop = false;
    std::thread m_thread;
};
