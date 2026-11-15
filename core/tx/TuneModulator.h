// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "TxModulator.h"

#include <QImage>
#include <QString>
#include <QtMath>

#include <algorithm>
#include <cmath>

/**
 * @brief Steady carrier for tuning (ATU, power, ALC) in any mode.
 *
 * One sine tone with 5 ms raised-cosine edges, for at most maxSeconds; the
 * operator normally stops it earlier with the Tune button.
 */
class TuneModulator final : public TxModulator
{
public:
    TuneModulator(double toneHz, int sampleRate, double maxSeconds = 60.0)
        : m_toneHz(qBound(200.0, toneHz, 3000.0)),
          m_sampleRate(qBound(8000, sampleRate, 192000)),
          m_total(qint64(qMax(1.0, maxSeconds) * m_sampleRate)),
          m_ramp(qMax(1, m_sampleRate / 200))
    {
    }

    int sampleRate() const override { return m_sampleRate; }

    int generate(float *output, int sampleCount) override
    {
        if (output == nullptr || sampleCount <= 0 || isFinished()) return 0;
        const int n = int(std::min<qint64>(sampleCount, m_total - m_position));
        const double step = 2.0 * M_PI * m_toneHz / m_sampleRate;
        for (int i = 0; i < n; ++i, ++m_position) {
            const qint64 edge = std::min(m_position + 1, m_total - m_position);
            const double gain = edge >= m_ramp ? 1.0 : 0.5 - 0.5 * std::cos(M_PI * double(edge) / m_ramp);
            output[i] = float(0.62 * gain * std::sin(m_phase));
            m_phase += step;
            if (m_phase > 2.0 * M_PI) m_phase -= 2.0 * M_PI;
        }
        return n;
    }

    bool isFinished() const override { return m_position >= m_total; }
    double progress() const override { return m_total > 0 ? double(m_position) / double(m_total) : 1.0; }
    QImage previewImage() const override { return QImage(); }
    QString description() const override { return QStringLiteral("Tune carrier %1 Hz").arg(qRound(m_toneHz)); }
    bool lowLatencyTx() const override { return true; }
    int trailingSilenceSamples() const override { return 0; }

private:
    double m_toneHz;
    int m_sampleRate;
    qint64 m_total;
    qint64 m_ramp;
    qint64 m_position = 0;
    double m_phase = 0.0;
};
