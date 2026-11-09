// SPDX-License-Identifier: GPL-3.0-or-later
#include "Js8Transmitter.h"

#include "../Js8Ldpc.h"

#include <QColor>
#include <QPainter>
#include <QtGlobal>

#include <algorithm>
#include <cmath>

namespace {
constexpr double kTwoPi = 6.283185307179586476925286766559;
}

Js8Transmitter::Js8Transmitter(const QVector<Js8Message::Frame> &frames, Js8Mode::Submode submode,
                               int sampleRate, double txFrequencyHz, std::function<bool()> cancelled)
    : m_frames(frames),
      m_submode(submode),
      m_sampleRate(qBound(8000, sampleRate, 96000)),
      m_txFrequencyHz(txFrequencyHz)
{
    m_text = Js8Message::preview(m_frames, m_submode);
    buildWaveform(cancelled);
}

QVector<int> Js8Transmitter::tones(const Js8Message::Frame &frame, Js8Mode::Submode submode)
{
    Js8Ldpc::Message bits{};
    if (!Js8Ldpc::packFrame(frame.frame, frame.bits & 7, &bits)) return {};
    const Js8Ldpc::Codeword cw = Js8Ldpc::encode(bits);
    const Js8Mode::Costas costas = Js8Mode::costas(Js8Mode::parameters(submode).costas);
    QVector<int> t(Js8Mode::kSymbols, 0);
    for (int k = 0; k < 7; ++k) {
        t[k] = costas[0][size_t(k)];
        t[36 + k] = costas[1][size_t(k)];
        t[72 + k] = costas[2][size_t(k)];
    }
    int pos = 7;
    for (int j = 0; j < Js8Mode::kDataSymbols; ++j) {
        if (j == 29) pos += 7;
        t[pos++] = cw[size_t(3 * j)] * 4 + cw[size_t(3 * j + 1)] * 2 + cw[size_t(3 * j + 2)];
    }
    return t;
}

double Js8Transmitter::durationSeconds() const
{
    const Js8Mode::Parameters p = Js8Mode::parameters(m_submode);
    if (m_frames.isEmpty()) return 0.0;
    return (m_frames.size() - 1) * p.periodSeconds + p.startSeconds() + p.durationSeconds();
}

void Js8Transmitter::buildWaveform(const std::function<bool()> &cancelled)
{
    m_samples.clear();
    m_position = 0;
    m_ok = false;
    if (m_frames.isEmpty()) { m_error = QStringLiteral("Empty JS8 message."); return; }
    const Js8Mode::Parameters p = Js8Mode::parameters(m_submode);
    if (m_txFrequencyHz < Js8Mode::minimumFrequencyHz() ||
        m_txFrequencyHz + p.bandwidthHz() >= 0.48 * m_sampleRate) {
        m_error = QStringLiteral("JS8 TX frequency outside the audio passband; TX was inhibited.");
        return;
    }

    const double samplesPerSymbol = double(p.nsps) * m_sampleRate / 12000.0;
    const int total = int(std::ceil(durationSeconds() * m_sampleRate)) + m_sampleRate / 50;
    m_samples.fill(0.0f, total);
    const int ramp = qMax(1, int(0.004 * m_sampleRate));   // 4 ms edges
    const double baud = p.baud();
    for (int f = 0; f < m_frames.size(); ++f) {
        if (cancelled && cancelled()) { m_error = QStringLiteral("Cancelled"); m_samples.clear(); return; }
        const QVector<int> t = tones(m_frames.at(f), m_submode);
        if (t.size() != Js8Mode::kSymbols) {
            m_error = QStringLiteral("Invalid JS8 frame \"%1\".").arg(m_frames.at(f).frame);
            m_samples.clear();
            return;
        }
        const double start = (double(f) * p.periodSeconds + p.startSeconds()) * m_sampleRate;
        const int first = int(std::lround(start));
        const int count = int(std::lround(Js8Mode::kSymbols * samplesPerSymbol));
        double phase = 0.0;
        for (int n = 0; n < count && first + n < total; ++n) {
            const int sym = qMin(Js8Mode::kSymbols - 1, int(n / samplesPerSymbol));
            const double freq = m_txFrequencyHz + t[sym] * baud;
            phase += kTwoPi * freq / m_sampleRate;
            if (phase > kTwoPi) phase -= kTwoPi;
            double gain = 1.0;
            const int edge = qMin(n + 1, count - n);
            if (edge < ramp) gain = 0.5 - 0.5 * std::cos(M_PI * edge / ramp);
            m_samples[first + n] = float(0.95 * gain * std::sin(phase));
        }
    }
    m_ok = true;
}

int Js8Transmitter::sampleRate() const { return m_sampleRate; }

bool Js8Transmitter::prepareForPlayback(qint64 utcMs)
{
    const Js8Mode::Parameters p = Js8Mode::parameters(m_submode);
    const qint64 periodMs = qint64(p.periodSeconds) * 1000;
    const qint64 boundary = ((utcMs + periodMs / 2) / periodMs) * periodMs;
    const qint64 offsetMs = utcMs - boundary;
    if (offsetMs < 0) {
        m_leadingSamples = int(qMin<qint64>(-offsetMs, qint64{2000}) * m_sampleRate / 1000);
    } else {
        // Never cut into the first frame: skip at most the start delay.
        m_position = int(qMin<qint64>(offsetMs, qint64(p.startDelayMs)) * m_sampleRate / 1000);
    }
    return true;
}

int Js8Transmitter::generate(float *output, int sampleCount)
{
    if (output == nullptr || sampleCount <= 0 || isFinished()) return 0;
    int written = 0;
    if (m_leadingSamples > 0) {
        const int lead = qMin(sampleCount, m_leadingSamples);
        std::fill(output, output + lead, 0.0f);
        m_leadingSamples -= lead;
        written = lead;
    }
    const int n = qMin(sampleCount - written, m_samples.size() - m_position);
    if (n > 0) {
        std::copy(m_samples.constData() + m_position, m_samples.constData() + m_position + n, output + written);
        m_position += n;
        written += n;
    }
    return written;
}

bool Js8Transmitter::isFinished() const { return m_leadingSamples <= 0 && m_position >= m_samples.size(); }

double Js8Transmitter::progress() const
{
    if (m_samples.isEmpty()) return 1.0;
    return qBound(0.0, double(m_position) / m_samples.size(), 1.0);
}

QImage Js8Transmitter::previewImage() const
{
    QImage image(480, 64, QImage::Format_RGB32);
    image.fill(QColor(20, 24, 32));
    QPainter painter(&image);
    painter.setPen(QColor(120, 220, 160));
    painter.drawText(image.rect().adjusted(8, 4, -8, -4), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap,
                     QStringLiteral("JS8 %1, %2 frame(s): %3")
                         .arg(Js8Mode::submodeName(m_submode))
                         .arg(m_frames.size())
                         .arg(m_text.trimmed()));
    return image;
}

QString Js8Transmitter::description() const
{
    return QStringLiteral("JS8 %1 TX: %2 (%3 frame(s), %4 Hz)")
        .arg(Js8Mode::submodeName(m_submode), m_text.trimmed())
        .arg(m_frames.size())
        .arg(qRound(m_txFrequencyHz));
}

bool Js8Transmitter::lowLatencyTx() const { return true; }
int Js8Transmitter::trailingSilenceSamples() const { return 0; }
