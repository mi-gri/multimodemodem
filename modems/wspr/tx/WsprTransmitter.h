// SPDX-License-Identifier: GPL-3.0-or-later
//
// WSPR-2 transmission: 162 4-FSK symbols of 8192/12000 s, 1.4648 Hz tone
// spacing, phase-continuous, starting 1 s after the even UTC minute (the
// waveform begins with that second of silence; PTT is keyed at the minute).
#pragma once

#include "../../../core/tx/TxModulator.h"

#include <QString>
#include <QVector>

#include <functional>

class WsprTransmitter final : public TxModulator
{
public:
    // @p audioHz: centre of the signal (tones at audioHz + (k - 1.5) * 1.4648 Hz),
    // 1400 .. 1600 Hz as WSJT-X allows.
    WsprTransmitter(const QString &message, int sampleRate, double audioHz,
                    std::function<bool()> cancelled = {});

    int sampleRate() const override;
    int generate(float *output, int sampleCount) override;
    bool isFinished() const override;
    double progress() const override;
    QImage previewImage() const override;
    QString description() const override;
    bool lowLatencyTx() const override;
    int trailingSilenceSamples() const override;

    bool generationSucceeded() const;
    QString generationError() const;
    QString message() const { return m_message; }

    static constexpr double kToneSpacingHz = 12000.0 / 8192.0;
    static constexpr double kSymbolSeconds = 8192.0 / 12000.0;
    static constexpr double kLeadSeconds = 1.0;

private:
    void buildWaveform();

    QString m_message;
    QString m_error;
    int m_sampleRate = 48000;
    double m_audioHz = 1500.0;
    QVector<float> m_samples;
    int m_position = 0;
    bool m_ok = false;
    std::function<bool()> m_cancelled;
};
