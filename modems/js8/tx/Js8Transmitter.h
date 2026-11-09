// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef JS8TRANSMITTER_H
#define JS8TRANSMITTER_H

#include "../../../core/tx/TxModulator.h"
#include "../Js8Message.h"
#include "../Js8Mode.h"

#include <QImage>
#include <QString>
#include <QVector>
#include <functional>

/**
 * @brief JS8 waveform for one or more frames in consecutive periods.
 *
 * The audio starts at a UTC period boundary.  Frame n begins at
 * n * period + start delay (500/200/100/500 ms) and is 79 symbols of
 * continuous-phase 8-FSK with tone spacing = keying rate, as JS8Call's
 * Modulator produces.  Short raised-cosine edges avoid key clicks.
 */
class Js8Transmitter final : public TxModulator
{
public:
    Js8Transmitter(const QVector<Js8Message::Frame> &frames,
                   Js8Mode::Submode submode,
                   int sampleRate,
                   double txFrequencyHz,
                   std::function<bool()> cancelled = {});

    /// Tones of one frame (79 values 0..7); empty if the frame is invalid.
    static QVector<int> tones(const Js8Message::Frame &frame, Js8Mode::Submode submode);

    int sampleRate() const override;
    /// Aligns the waveform to the UTC period grid: a late first pull skips
    /// part of the start delay, an early one waits.
    bool prepareForPlayback(qint64 utcMs) override;
    int generate(float *output, int sampleCount) override;
    bool isFinished() const override;
    double progress() const override;
    QImage previewImage() const override;
    QString description() const override;
    bool lowLatencyTx() const override;
    int trailingSilenceSamples() const override;

    bool generationSucceeded() const { return m_ok; }
    QString generationError() const { return m_error; }
    int frameCount() const { return m_frames.size(); }
    /// Seconds from the first boundary to the end of the last frame.
    double durationSeconds() const;

private:
    void buildWaveform(const std::function<bool()> &cancelled);

    QVector<Js8Message::Frame> m_frames;
    Js8Mode::Submode m_submode;
    int m_sampleRate;
    double m_txFrequencyHz;
    QVector<float> m_samples;
    int m_position = 0;
    int m_leadingSamples = 0;
    bool m_ok = false;
    QString m_error;
    QString m_text;
};

#endif // JS8TRANSMITTER_H
