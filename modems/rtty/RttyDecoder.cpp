#include "RttyDecoder.h"

#include <QHash>
#include <QtMath>

namespace {

constexpr double kTwoPi = 2.0 * M_PI;
constexpr double kMinStartQuality = 0.075;
constexpr double kMinDataQuality = 0.022;
constexpr double kMinStopQuality = 0.055;
constexpr double kOpenQuality = 0.42;
constexpr double kCloseQuality = 0.30;
constexpr double kAtcBias = 0.25;
constexpr double kNormalBasebandCutoffBaud = 0.75; // 1.5 x baud full Mark/Space channel BW
constexpr double kNarrowBasebandCutoffBaud = 0.60; // 1.2 x baud full channel BW

double decayAverage(double current, double target, double timeSamples)
{
    const double alpha = 1.0 / qMax(1.0, timeSamples);
    return ((1.0 - alpha) * current) + (alpha * target);
}

QString lettersForCode(int code)
{
    static const QHash<int, QString> table = {
        {1, "E"}, {2, "\n"}, {3, "A"}, {4, " "}, {5, "S"},
        {6, "I"}, {7, "U"}, {8, "\r"}, {9, "D"}, {10, "R"},
        {11, "J"}, {12, "N"}, {13, "F"}, {14, "C"}, {15, "K"},
        {16, "T"}, {17, "Z"}, {18, "L"}, {19, "W"}, {20, "H"},
        {21, "Y"}, {22, "P"}, {23, "Q"}, {24, "O"}, {25, "B"},
        {26, "G"}, {28, "M"}, {29, "X"}, {30, "V"}
    };

    return table.value(code, QString());
}

QString figuresForCode(int code)
{
    static const QHash<int, QString> table = {
        {1, "3"}, {2, "\n"}, {3, "-"}, {4, " "}, {5, "'"},
        {6, "8"}, {7, "7"}, {8, "\r"}, {9, "$"}, {10, "4"},
        {11, "\a"}, {12, ","}, {13, "!"}, {14, ":"}, {15, "("},
        {16, "5"}, {17, "\""}, {18, ")"}, {19, "2"}, {20, "#"},
        {21, "6"}, {22, "0"}, {23, "1"}, {24, "9"}, {25, "?"},
        {26, "&"}, {28, "."}, {29, "/"}, {30, ";"}
    };

    return table.value(code, QString());
}

} // namespace


void RttyDecoder::LowPassBiquad::reset()
{
    m_z1 = 0.0;
    m_z2 = 0.0;
}

void RttyDecoder::LowPassBiquad::setLowPass(double sampleRate, double cutoffHz, double q)
{
    if (sampleRate <= 0.0 || cutoffHz <= 0.0) {
        m_b0 = 1.0; m_b1 = m_b2 = m_a1 = m_a2 = 0.0;
        return;
    }
    const double safeCutoff = qBound(1.0, cutoffHz, sampleRate * 0.45);
    const double omega = kTwoPi * safeCutoff / sampleRate;
    const double cosine = qCos(omega);
    const double sine = qSin(omega);
    const double alpha = sine / (2.0 * qMax(0.05, q));
    const double a0 = 1.0 + alpha;
    m_b0 = ((1.0 - cosine) * 0.5) / a0;
    m_b1 = (1.0 - cosine) / a0;
    m_b2 = m_b0;
    m_a1 = (-2.0 * cosine) / a0;
    m_a2 = (1.0 - alpha) / a0;
}

double RttyDecoder::LowPassBiquad::process(double input)
{
    const double output = (m_b0 * input) + m_z1;
    m_z1 = (m_b1 * input) - (m_a1 * output) + m_z2;
    m_z2 = (m_b2 * input) - (m_a2 * output);
    return output;
}

RttyDecoder::RttyDecoder(QObject *parent)
    : QObject(parent)
{
    reset();
}

QString RttyDecoder::modeName()
{
    return "RTTY Text";
}

QVector<FrequencyMarker> RttyDecoder::frequencyMarkers(double markHz,
                                                       double spaceHz,
                                                       bool reverse)
{
    QVector<FrequencyMarker> markers;
    FrequencyMarker markMarker;
    markMarker.frequencyHz = markHz;
    markMarker.label = reverse ? "Space" : "Mark";
    markMarker.color = QColor(100, 220, 255);
    markers.append(markMarker);

    FrequencyMarker spaceMarker;
    spaceMarker.frequencyHz = spaceHz;
    spaceMarker.label = reverse ? "Mark" : "Space";
    spaceMarker.color = QColor(255, 220, 80);
    markers.append(spaceMarker);

    return markers;
}

void RttyDecoder::reset()
{
    resetSignalPath();
    m_confidence = 0.0;

    resetFrame();
    m_lettersShift = true;
    m_samplesProcessed = 0;
    m_decodedChars = 0;
    m_goodFrames = 0;
    m_badFrames = 0;
    m_squelchedStarts = 0;
    m_statusCounter = 0;
    m_scopeDecimator = 0;
    m_scopeTrace.clear();
    m_text.clear();

    if (m_sampleRate > 0) {
        updateOscillators(m_sampleRate);
    }

    emit textUpdated(m_text);
    emit statusChanged("RTTY: squelch closed, waiting for mark/space tones");
    emit markersChanged(frequencyMarkers(m_markHz, m_spaceHz, m_reverse));
    emit tuningScopeChanged(0.0, 0.0, 0.0, false);
    emit tuningScopeTraceChanged(QVector<QPointF>(), 0.0, false);
}

void RttyDecoder::resumeAfterLocalTransmit()
{
    // Audio capture is deliberately stopped while our transmitter is keyed.
    // Do not stitch a partially received Baudot frame across that hole, but
    // retain the signal/noise and LETTERS/FIGURES context
    // learned immediately before TX.  This makes a contest reply decodable
    // from its first complete start bit instead of after a fresh squelch acquisition lasting several characters.
    resetFrame();
    m_markI = 0.0;
    m_markQ = 0.0;
    m_spaceI = 0.0;
    m_spaceQ = 0.0;
    m_markILpf.reset();
    m_markQLpf.reset();
    m_spaceILpf.reset();
    m_spaceQLpf.reset();
    m_carrierOpen = false;
    m_qualityMean = 0.0;
    m_startSpaceSamples = 0;
    m_idleMarkSamples = 0;
    m_scopeDecimator = 0;
    m_scopeTrace.clear();
    emit statusChanged(QStringLiteral("RTTY: RX resumed after local TX; retained signal history"));
}

void RttyDecoder::setBaudRate(double baud)
{
    const double boundedBaud = qBound(10.0, baud, 300.0);
    if (qAbs(m_baudRate - boundedBaud) < 1.0e-9) return;
    m_baudRate = boundedBaud;
    if (m_sampleRate > 0) {
        updateOscillators(m_sampleRate);
    }
    reset();
}

void RttyDecoder::setTones(double markHz, double spaceHz)
{
    retuneTones(markHz, spaceHz);
    resetFrame();
}

void RttyDecoder::retuneTones(double markHz, double spaceHz)
{
    m_markHz = qBound(100.0, markHz, 3500.0);
    m_spaceHz = qBound(100.0, spaceHz, 3500.0);
    if (qAbs(m_spaceHz - m_markHz) < 20.0) {
        m_spaceHz = m_markHz + 170.0;
    }
    if (m_sampleRate > 0) {
        updateOscillators(m_sampleRate);
    }
    emit markersChanged(frequencyMarkers(m_markHz, m_spaceHz, m_reverse));
}

void RttyDecoder::setNarrowFilterEnabled(bool enabled)
{
    if (m_narrowFilterEnabled == enabled) {
        return;
    }
    m_narrowFilterEnabled = enabled;
    if (m_sampleRate > 0) {
        updateOscillators(m_sampleRate);
    }
    m_markILpf.reset();
    m_markQLpf.reset();
    m_spaceILpf.reset();
    m_spaceQLpf.reset();
    m_markEnvelope = m_spaceEnvelope = 0.0;
    m_markNoise = m_spaceNoise = 0.0;
    m_qualityMean = 0.0;
    m_carrierOpen = false;
    resetFrame();
}

void RttyDecoder::setReverse(bool reverse)
{
    if (m_reverse == reverse) {
        return;
    }
    m_reverse = reverse;
    reset();
}

double RttyDecoder::baudRate() const
{
    return m_baudRate;
}

double RttyDecoder::markHz() const
{
    return m_markHz;
}

double RttyDecoder::spaceHz() const
{
    return m_spaceHz;
}

bool RttyDecoder::reverse() const
{
    return m_reverse;
}

QString RttyDecoder::receivedText() const
{
    return m_text;
}

void RttyDecoder::processAudioBlock(const AudioBlock &block)
{
    if (block.samples.isEmpty() || block.sampleRate <= 0) {
        return;
    }

    if (block.sampleRate != m_sampleRate) {
        updateOscillators(block.sampleRate);
    }

    for (float rawSample : block.samples) {
        const double sample = qBound(-1.0, static_cast<double>(rawSample), 1.0);

        m_inputPower = (0.9994 * m_inputPower) + (0.0006 * sample * sample);

        const double markSin = qSin(m_markPhase);
        const double markCos = qCos(m_markPhase);
        const double spaceSin = qSin(m_spacePhase);
        const double spaceCos = qCos(m_spacePhase);

        // RTTY V2 channelizer: translate Mark and Space independently to
        // baseband, then low-pass each I/Q branch with a baud-derived
        // Butterworth section.  No recombination into wideband audio occurs.
        m_markI = m_markILpf.process(sample * markCos);
        m_markQ = m_markQLpf.process(sample * markSin);
        m_spaceI = m_spaceILpf.process(sample * spaceCos);
        m_spaceQ = m_spaceQLpf.process(sample * spaceSin);

        m_markPhase += m_markInc;
        m_spacePhase += m_spaceInc;
        if (m_markPhase >= kTwoPi) {
            m_markPhase -= kTwoPi;
        }
        if (m_spacePhase >= kTwoPi) {
            m_spacePhase -= kTwoPi;
        }

        const double markEnergy = (m_markI * m_markI) + (m_markQ * m_markQ);
        const double spaceEnergy = (m_spaceI * m_spaceI) + (m_spaceQ * m_spaceQ);
        const double sumEnergy = markEnergy + spaceEnergy + 1.0e-14;
        const double markMagnitude = qSqrt(markEnergy);
        const double spaceMagnitude = qSqrt(spaceEnergy);
        const double magnitudeSum = markMagnitude + spaceMagnitude + 1.0e-14;

        // Fast-charge / slow-discharge envelopes follow selective fading but
        // deliberately do not follow individual Baudot bits.  Independent
        // slow noise estimators let the slicer remove the non-zero detector
        // floor before applying automatic threshold correction (ATC).
        if (m_markEnvelope <= 0.0) m_markEnvelope = markMagnitude;
        if (m_spaceEnvelope <= 0.0) m_spaceEnvelope = spaceMagnitude;
        if (m_markNoise <= 0.0) m_markNoise = markMagnitude;
        if (m_spaceNoise <= 0.0) m_spaceNoise = spaceMagnitude;
        m_markEnvelope = decayAverage(m_markEnvelope, markMagnitude,
                                      markMagnitude > m_markEnvelope ? m_symbolSamples * 0.25
                                                                      : m_symbolSamples * 16.0);
        m_spaceEnvelope = decayAverage(m_spaceEnvelope, spaceMagnitude,
                                       spaceMagnitude > m_spaceEnvelope ? m_symbolSamples * 0.25
                                                                        : m_symbolSamples * 16.0);
        m_markNoise = decayAverage(m_markNoise, markMagnitude,
                                   markMagnitude < m_markNoise ? m_symbolSamples * 0.25
                                                               : m_symbolSamples * 48.0);
        m_spaceNoise = decayAverage(m_spaceNoise, spaceMagnitude,
                                    spaceMagnitude < m_spaceNoise ? m_symbolSamples * 0.25
                                                                  : m_symbolSamples * 48.0);

        const double noiseFloor = qMin(m_markNoise, m_spaceNoise);
        const double clippedMark = qBound(noiseFloor, markMagnitude, qMax(noiseFloor, m_markEnvelope));
        const double clippedSpace = qBound(noiseFloor, spaceMagnitude, qMax(noiseFloor, m_spaceEnvelope));
        const double markEnv = qMax(0.0, m_markEnvelope - noiseFloor);
        const double spaceEnv = qMax(0.0, m_spaceEnvelope - noiseFloor);
        const double atcNumerator =
            ((clippedMark - noiseFloor) * markEnv) -
            ((clippedSpace - noiseFloor) * spaceEnv) -
            kAtcBias * ((markEnv * markEnv) - (spaceEnv * spaceEnv));
        const double atcDenominator = (markEnv * markEnv) + (spaceEnv * spaceEnv) + 1.0e-14;
        const double diffNorm = qBound(-1.0, atcNumerator / atcDenominator, 1.0);

        // Framing quality intentionally uses raw channel separation, not the
        // threshold-corrected metric.  This remains meaningful while one tone
        // is selectively faded and avoids the old wideband-power desensitizer.
        const double bitQuality = qBound(0.0, qAbs(markMagnitude - spaceMagnitude) / magnitudeSum, 1.0);
        if (m_visualizationEnabled && ++m_scopeDecimator >= 24) {
            m_scopeDecimator = 0;

            /*
             * Crossed-ellipse tuning display.
             *
             * A real RTTY oscilloscope does not plot random decoder decisions.
             * It drives the X/Y plates from the two tuned tone channels and the
             * phosphor brightness from received energy.  The previous MM trace
             * plotted low-rate I samples from the Mark/Space mixers directly;
             * that creates a square cloud when the bit stream changes state and
             * is not useful for tuning.
             *
             * Keep the decoder signal path unchanged, but synthesize the CRT
             * deflection from the *measured* Mark/Space envelopes and the live
             * oscillator phases:
             *   - Mark dominant  -> horizontal ellipse on the X axis.
             *   - Space dominant -> vertical ellipse on the Y axis.
             *   - transition / mistune -> diagonal smear between the two.
             * This matches the classic MMTTY/Penn crossed-ellipse instrument:
             * two clean right-angle ellipses mean the RX tones and shift are
             * centered; crooked/asymmetric figures mean mistune or wrong shift.
             */
            const double markLevel = qSqrt(markEnergy);
            const double spaceLevel = qSqrt(spaceEnergy);
            const double levelSum = markLevel + spaceLevel + 1.0e-12;
            const double markWeight = markLevel / levelSum;
            const double spaceWeight = spaceLevel / levelSum;
            const double strength = sumEnergy / qMax(m_noiseFloor, 1.0e-12);

            if ((m_carrierOpen || m_gateScore > 0.30) && strength > 2.0) {
                const double bitPurity = qBound(0.0, qAbs(markWeight - spaceWeight), 1.0);
                const double amplitude = qBound(0.18, 0.28 + 0.17 * qLn(1.0 + strength), 0.94);
                const double minor = qBound(0.10, 0.14 + 0.22 * (1.0 - bitPurity), 0.34);
                QPointF point;

                if (bitPurity < 0.18) {
                    // Transition or wrong shift: show the real crossed/diagonal
                    // energy rather than snapping to either ellipse.
                    const double mx = qSin(m_markPhase);
                    const double sy = qSin(m_spacePhase);
                    point = QPointF(qBound(-1.0, amplitude * mx, 1.0),
                                    qBound(-1.0, amplitude * sy, 1.0));
                } else if (markWeight >= spaceWeight) {
                    const double x = amplitude * qCos(m_markPhase);
                    const double y = minor * qSin(m_markPhase);
                    point = QPointF(qBound(-1.0, x, 1.0),
                                    qBound(-1.0, y, 1.0));
                } else {
                    const double x = minor * qSin(m_spacePhase);
                    const double y = amplitude * qCos(m_spacePhase);
                    point = QPointF(qBound(-1.0, x, 1.0),
                                    qBound(-1.0, y, 1.0));
                }

                m_scopeTrace.append(point);
                if (m_scopeTrace.size() > 520) {
                    m_scopeTrace.remove(0, m_scopeTrace.size() - 520);
                }
            }
        }

        m_confidence = (0.9985 * m_confidence) + (0.0015 * bitQuality);
        updateCarrierGate(sumEnergy, bitQuality);

        const bool rawBitIsMark = diffNorm >= 0.0;
        if (!m_softPrefix.isEmpty()) {
            m_softRunning += m_reverse ? -diffNorm : diffNorm;
            m_softPrefix[static_cast<int>(m_samplesProcessed % m_softPrefix.size())] = m_softRunning;
        }

        bool bitIsMark = rawBitIsMark;
        if (m_reverse) {
            bitIsMark = !bitIsMark;
        }

        if (!m_carrierOpen) {
            if (m_state != RxState::WaitingStart) {
                resetFrame();
            }
            ++m_samplesProcessed;
            continue;
        }

        // The slicer gates on the larger of raw tone separation and the
        // ATC-corrected decision.  During a selective fade one tone sits in
        // the noise, so a bit of the faded tone shows almost no raw
        // separation (both channels near the floor) although ATC still
        // decides it reliably against the other tone's envelope.
        advanceSymbolClock(bitIsMark, qMax(bitQuality, qAbs(diffNorm)));
        ++m_samplesProcessed;
    }

    maybeEmitStatus();
}

void RttyDecoder::updateOscillators(int sampleRate)
{
    m_sampleRate = sampleRate;
    m_symbolSamples = static_cast<double>(sampleRate) / qMax(1.0, m_baudRate);
    // Ten symbols of soft history cover start, data and stop bits plus the
    // timing search; never shrink below that.
    const int historySize = static_cast<int>(m_symbolSamples * 10.0) + 64;
    if (m_softPrefix.size() != historySize) {
        m_softPrefix = QVector<double>(historySize, m_softRunning);
    }
    m_markInc = kTwoPi * m_markHz / static_cast<double>(sampleRate);
    m_spaceInc = kTwoPi * m_spaceHz / static_cast<double>(sampleRate);

    const double cutoffMultiplier = m_narrowFilterEnabled
                                      ? kNarrowBasebandCutoffBaud
                                      : kNormalBasebandCutoffBaud;
    const double cutoffHz = qMax(6.0, m_baudRate * cutoffMultiplier);
    constexpr double butterworthQ = 0.7071067811865476;
    m_markILpf.setLowPass(sampleRate, cutoffHz, butterworthQ);
    m_markQLpf.setLowPass(sampleRate, cutoffHz, butterworthQ);
    m_spaceILpf.setLowPass(sampleRate, cutoffHz, butterworthQ);
    m_spaceQLpf.setLowPass(sampleRate, cutoffHz, butterworthQ);
}

void RttyDecoder::resetSignalPath()
{
    m_markPhase = 0.0;
    m_spacePhase = 0.0;
    m_markI = m_markQ = 0.0;
    m_spaceI = m_spaceQ = 0.0;
    m_markILpf.reset();
    m_markQLpf.reset();
    m_spaceILpf.reset();
    m_spaceQLpf.reset();
    m_markEnvelope = m_spaceEnvelope = 0.0;
    m_markNoise = m_spaceNoise = 0.0;
    m_qualityMean = 0.0;
    m_inputPower = 0.0;
    m_noiseFloor = 0.0;
    m_energySnr = 1.0;
    m_toneRatio = 0.0;
    m_gateScore = 0.0;
    m_carrierOpen = false;
}

void RttyDecoder::updateCarrierGate(double sumEnergy, double bitQuality)
{
    // The old gate divided Mark+Space energy by total wideband input power.
    // A strong off-frequency contest signal could therefore close the RTTY
    // squelch even after the Mark/Space filters had rejected that interferer.
    // Gate on sustained separation of the two *channelized* branches instead.
    const double qualityAlpha = 1.0 / qMax(1.0, m_symbolSamples * 6.0);
    m_qualityMean = ((1.0 - qualityAlpha) * m_qualityMean) +
                    (qualityAlpha * qBound(0.0, bitQuality, 1.0));

    if (m_noiseFloor <= 0.0) {
        m_noiseFloor = qMax(sumEnergy, 1.0e-14);
    } else if (sumEnergy < m_noiseFloor) {
        m_noiseFloor = (0.98 * m_noiseFloor) + (0.02 * sumEnergy);
    } else {
        m_noiseFloor = (0.9999 * m_noiseFloor) + (0.0001 * sumEnergy);
    }
    m_energySnr = sumEnergy / qMax(m_noiseFloor, 1.0e-14);
    m_toneRatio = sumEnergy / qMax(m_inputPower, 1.0e-14); // diagnostic only
    m_gateScore = m_qualityMean;

    const bool hasNarrowEnergy = sumEnergy > 1.0e-10;
    if (!m_carrierOpen && hasNarrowEnergy && m_qualityMean >= kOpenQuality) {
        m_carrierOpen = true;
        resetFrame();
    } else if (m_carrierOpen && m_qualityMean <= kCloseQuality) {
        m_carrierOpen = false;
        resetFrame();
    }
}

void RttyDecoder::resetFrame()
{
    m_state = RxState::WaitingStart;
    m_samplesToNextDecision = 0.0;
    m_idleMarkSamples = 0;
    m_startSpaceSamples = 0;
    m_dataBitIndex = 0;
    m_currentCode = 0;
}

void RttyDecoder::advanceSymbolClock(bool bitIsMark, double bitQuality)
{
    switch (m_state) {
    case RxState::WaitingStart:
        if (bitIsMark) {
            if (bitQuality >= kMinDataQuality) {
                m_idleMarkSamples = qMin(m_idleMarkSamples + 1,
                                         static_cast<int>(m_symbolSamples * 4.0));
            }
            m_startSpaceSamples = 0;
        } else {
            ++m_startSpaceSamples;
            const bool hadStableIdle = m_idleMarkSamples >= static_cast<int>(m_symbolSamples * 0.32);
            const bool plausibleStart = hadStableIdle &&
                                        bitQuality >= kMinStartQuality &&
                                        m_carrierOpen &&
                                        m_startSpaceSamples <= static_cast<int>(m_symbolSamples * 0.80);
            if (plausibleStart) {
                m_frameStartSample = m_samplesProcessed - m_startSpaceSamples + 1;
                m_state = RxState::ValidateStart;
                m_samplesToNextDecision = qMax(1.0,
                                               (m_symbolSamples * 0.50) - static_cast<double>(m_startSpaceSamples));
            } else if (m_startSpaceSamples > static_cast<int>(m_symbolSamples * 1.20)) {
                ++m_squelchedStarts;
                m_idleMarkSamples = 0;
                m_startSpaceSamples = 0;
            }
        }
        break;

    case RxState::ValidateStart:
        m_samplesToNextDecision -= 1.0;
        if (m_samplesToNextDecision <= 0.0) {
            // Start bit: Space either in the single sample at mid-bit or in
            // the mean soft value over its first half (noise or a fade can
            // flip one of them).
            const bool sampledBitIsMark = frameSoftMean(0.1, 0.5) >= 0.0 &&
                                          (bitIsMark || bitQuality < kMinStartQuality);
            if (!sampledBitIsMark && m_carrierOpen) {
                m_state = RxState::DataBits;
                m_samplesToNextDecision = m_symbolSamples;
                m_dataBitIndex = 0;
                m_currentCode = 0;
            } else {
                ++m_squelchedStarts;
                resetFrame();
            }
        }
        break;

    case RxState::DataBits:
        m_samplesToNextDecision -= 1.0;
        if (m_samplesToNextDecision <= 0.0) {
            // No single-sample quality abort here: the bits are decided
            // afterwards by integrating the soft history (refinedFrameCode).
            const bool sampledBitIsMark = bitIsMark;
            if (sampledBitIsMark) {
                m_currentCode |= (1 << m_dataBitIndex);
            }
            ++m_dataBitIndex;
            m_samplesToNextDecision += m_symbolSamples;

            if (m_dataBitIndex >= 5) {
                m_state = RxState::StopBits;
                m_samplesToNextDecision = m_symbolSamples;
            }
        }
        break;

    case RxState::StopBits:
        m_samplesToNextDecision -= 1.0;
        if (m_samplesToNextDecision <= 0.0) {
            const bool sampledBitIsMark = bitIsMark;
            if (sampledBitIsMark && bitQuality >= kMinStopQuality && m_carrierOpen) {
                ++m_goodFrames;
                const int refined = refinedFrameCode();
                handleCode(refined >= 0 ? refined : m_currentCode);
                resetFrame();
                m_idleMarkSamples = static_cast<int>(m_symbolSamples);
                m_startSpaceSamples = 0;
            } else {
                ++m_badFrames;
                if ((m_badFrames % 3) == 0) {
                    m_gateScore *= 0.55;
                }
                resetFrame();
            }
        }
        break;
    }

}

double RttyDecoder::softSum(qint64 from, qint64 to) const
{
    const int size = m_softPrefix.size();
    const auto at = [&](qint64 index) { return m_softPrefix[static_cast<int>(index % size)]; };
    return at(to - 1) - at(from - 1);
}

double RttyDecoder::frameSoftMean(double fromSymbols, double toSymbols) const
{
    if (m_softPrefix.isEmpty()) {
        return 0.0;
    }
    const qint64 from = m_frameStartSample + static_cast<qint64>(fromSymbols * m_symbolSamples);
    const qint64 to = qMin(m_samplesProcessed + 1,
                           m_frameStartSample + static_cast<qint64>(toSymbols * m_symbolSamples));
    if (from < 1 || to <= from || from <= m_samplesProcessed - m_softPrefix.size() + 2) {
        return 0.0;
    }
    return softSum(from, to) / static_cast<double>(to - from);
}

int RttyDecoder::refinedFrameCode() const
{
    if (m_softPrefix.isEmpty()) {
        return -1;
    }
    // Called at the stop-bit decision, 6.5 symbols after the detected edge.
    const double symbol = m_symbolSamples;
    const qint64 newest = m_samplesProcessed;              // last stored sample
    const qint64 oldest = newest - m_softPrefix.size() + 2;
    const double maxShift = 0.30 * symbol;
    const double step = qMax(1.0, symbol / 40.0);
    double bestMetric = -1.0e300;
    int bestCode = -1;
    for (double shift = -maxShift; shift <= maxShift + 1.0e-9; shift += step) {
        const double edge = static_cast<double>(m_frameStartSample) + shift;
        double metric = 0.0;
        int code = 0;
        bool covered = true;
        for (int bit = 0; bit <= 5 && covered; ++bit) {
            // Integrate the central 60 % of the bit.
            const qint64 from = static_cast<qint64>(edge + (bit + 0.2) * symbol);
            const qint64 to = static_cast<qint64>(edge + (bit + 0.8) * symbol);
            if (from < 1 || from <= oldest || to > newest + 1 || to <= from) {
                covered = false;
                break;
            }
            const double value = softSum(from, to);
            if (bit == 0) {
                metric -= value;                            // start bit is Space
            } else {
                metric += qAbs(value);
                if (value >= 0.0) {
                    code |= 1 << (bit - 1);
                }
            }
        }
        if (covered && metric > bestMetric) {
            bestMetric = metric;
            bestCode = code;
        }
    }
    return bestCode;
}

void RttyDecoder::handleCode(int code)
{
    if (code == 31) {
        m_lettersShift = true;
        return;
    }

    if (code == 27) {
        m_lettersShift = false;
        return;
    }

    const QString decoded = decodeCode(code);
    if (decoded.isEmpty()) {
        return;
    }

    m_text.append(decoded);
    if (m_text.size() > 20000) {
        m_text.remove(0, m_text.size() - 20000);
    }

    ++m_decodedChars;
    emit characterReceived(decoded);
    emit textUpdated(m_text);
}

QString RttyDecoder::decodeCode(int code) const
{
    if (m_lettersShift) {
        return lettersForCode(code);
    }

    return figuresForCode(code);
}


void RttyDecoder::maybeEmitStatus()
{
    if (!m_visualizationEnabled) return;
    ++m_statusCounter;
    if (m_statusCounter < 10) {
        return;
    }

    m_statusCounter = 0;
    const double markLevel = qSqrt((m_markI * m_markI) + (m_markQ * m_markQ));
    const double spaceLevel = qSqrt((m_spaceI * m_spaceI) + (m_spaceQ * m_spaceQ));
    emit statusChanged(QString("RTTY: %1 baud, mark %2 Hz, space %3 Hz, squelch %4, tone %5%, conf %6%, good/bad %7/%8, chars %9%10")
                           .arg(m_baudRate, 0, 'f', 2)
                           .arg(m_markHz, 0, 'f', 0)
                           .arg(m_spaceHz, 0, 'f', 0)
                           .arg(m_carrierOpen ? "open" : "closed")
                           .arg(qBound(0.0, m_toneRatio * 100.0, 999.0), 0, 'f', 0)
                           .arg(m_confidence * 100.0, 0, 'f', 0)
                           .arg(m_goodFrames)
                           .arg(m_badFrames)
                           .arg(m_decodedChars)
                           .arg(QString()));
    const bool scopeLocked = m_carrierOpen && (m_confidence > 0.20);
    emit tuningScopeChanged(markLevel, spaceLevel, m_energySnr, scopeLocked);
    emit tuningScopeTraceChanged(m_scopeTrace, m_energySnr, scopeLocked);
}
