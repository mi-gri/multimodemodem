// SPDX-License-Identifier: GPL-3.0-or-later
#include "WsprTransmitter.h"

#include "../../../third_party/wsjtx_gpl/wspr/WsprCodec.h"

#include <QColor>
#include <QImage>
#include <QPainter>
#include <QtGlobal>

#include <cmath>

namespace {
constexpr double kTwoPi = 6.283185307179586476925286766559;
}

WsprTransmitter::WsprTransmitter(const QString &message, int sampleRate, double audioHz,
                                 std::function<bool()> cancelled)
    : m_message(message.simplified().toUpper()),
      m_sampleRate(qBound(8000, sampleRate, 96000)),
      m_audioHz(audioHz),
      m_cancelled(std::move(cancelled))
{
    buildWaveform();
}

int WsprTransmitter::sampleRate() const { return m_sampleRate; }
bool WsprTransmitter::isFinished() const { return m_position >= m_samples.size(); }
bool WsprTransmitter::lowLatencyTx() const { return true; }
int WsprTransmitter::trailingSilenceSamples() const { return 0; }
bool WsprTransmitter::generationSucceeded() const { return m_ok; }
QString WsprTransmitter::generationError() const { return m_error; }

double WsprTransmitter::progress() const
{
    if (m_samples.isEmpty()) return isFinished() ? 1.0 : 0.0;
    return qBound(0.0, static_cast<double>(m_position) / static_cast<double>(m_samples.size()), 1.0);
}

QString WsprTransmitter::description() const
{
    return QStringLiteral("WSPR TX: %1, %2 Hz").arg(m_message).arg(m_audioHz, 0, 'f', 0);
}

int WsprTransmitter::generate(float *output, int sampleCount)
{
    if (output == nullptr || sampleCount <= 0 || isFinished()) return 0;
    const int n = qBound(0, sampleCount, m_samples.size() - m_position);
    std::copy(m_samples.constData() + m_position, m_samples.constData() + m_position + n, output);
    m_position += n;
    return n;
}

void WsprTransmitter::buildWaveform()
{
    m_samples.clear();
    m_position = 0;
    m_ok = false;
    // The whole signal (4 tones, 6 Hz) must stay inside the WSPR sub-band.
    if (!(m_audioHz >= 1400.0 && m_audioHz <= 1600.0)) {
        m_error = QStringLiteral("WSPR TX frequency %1 Hz is outside 1400-1600 Hz.").arg(m_audioHz, 0, 'f', 0);
        return;
    }
    unsigned char data[M3Wspr::kDataBytes];
    std::string error;
    if (!M3Wspr::pack(m_message.toStdString(), data, &error)) {
        m_error = QStringLiteral("WSPR message \"%1\" cannot be sent: %2").arg(m_message, QString::fromStdString(error));
        return;
    }
    unsigned char symbols[M3Wspr::kSymbols];
    M3Wspr::channelSymbolsForData(data, symbols);

    const int lead = static_cast<int>(std::lround(kLeadSeconds * m_sampleRate));
    const double samplesPerSymbol = kSymbolSeconds * m_sampleRate;
    const int active = static_cast<int>(std::llround(M3Wspr::kSymbols * samplesPerSymbol));
    m_samples.fill(0.0f, lead + active);
    const int ramp = qMax(1, m_sampleRate / 200);   // 5 ms cosine edges
    double phase = 0.0;
    for (int i = 0; i < active; ++i) {
        if ((i & 8191) == 0 && m_cancelled && m_cancelled()) {
            m_samples.clear();
            m_error = QStringLiteral("Cancelled");
            return;
        }
        const int s = qBound(0, static_cast<int>(i / samplesPerSymbol), M3Wspr::kSymbols - 1);
        const double f = m_audioHz + (symbols[s] - 1.5) * kToneSpacingHz;
        phase += kTwoPi * f / m_sampleRate;
        if (phase >= kTwoPi) phase -= kTwoPi;
        const int edge = qMin(i + 1, active - i);
        const double gain = edge >= ramp ? 1.0 : 0.5 - 0.5 * std::cos(0.5 * kTwoPi * edge / ramp);
        m_samples[lead + i] = static_cast<float>(0.58 * gain * std::sin(phase));
    }
    m_ok = true;
}

QImage WsprTransmitter::previewImage() const
{
    QImage image(480, 120, QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(8, 10, 12));
    QPainter p(&image);
    p.setPen(QColor(255, 176, 43));
    p.drawText(QRect(12, 10, 456, 28), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("WSPR-2 TX, %1 Hz").arg(m_audioHz, 0, 'f', 0));
    p.setPen(QColor(210, 230, 255));
    p.drawText(QRect(12, 42, 456, 32), Qt::AlignLeft | Qt::AlignVCenter, m_message);
    p.setPen(QColor(90, 160, 255));
    p.drawText(QRect(12, 78, 456, 26), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("162 symbols, 110.6 s from :01"));
    return image;
}
