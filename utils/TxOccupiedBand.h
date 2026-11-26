// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// The TX band permission is a statement about the signal on the air, not only about the dial: dial plus the audio
// offsets of the transmission must lie inside the same amateur band. Pure logic.
#include "StandardFrequencies.h"

#include <QString>

namespace M3TxBand {

struct Span {
    double lowHz = 0.0;     // lowest audio frequency of the transmitted signal
    double highHz = 0.0;    // highest audio frequency of the transmitted signal
};

// RF range of a transmission. SSB adds (upper sideband) or subtracts (lower sideband) the audio frequencies; CW rig
// modes transmit at the dial; FM and AM spread around the dial. @p rigMode is the CAT mode name ("" = unknown, then
// @p assumeLower decides the sideband).
inline void rfRange(double dialHz, const QString &rigMode, bool assumeLower, Span audio, double *rfLow, double *rfHigh)
{
    constexpr double kFmHalfWidthHz = 8000.0;
    constexpr double kCwHalfWidthHz = 200.0;
    const QString mode = rigMode.trimmed().toUpper();
    double low = 0.0;
    double high = 0.0;
    if (mode.contains(QLatin1String("FM"))) {
        low = dialHz - kFmHalfWidthHz;
        high = dialHz + kFmHalfWidthHz;
    } else if (mode.contains(QLatin1String("AM"))) {
        low = dialHz - audio.highHz;
        high = dialHz + audio.highHz;
    } else if (mode.startsWith(QLatin1String("CW"))) {
        low = dialHz - kCwHalfWidthHz;
        high = dialHz + kCwHalfWidthHz;
    } else if (mode.contains(QLatin1String("LSB")) || mode == QLatin1String("RTTY") || (mode.isEmpty() && assumeLower)) {
        low = dialHz - audio.highHz;
        high = dialHz - audio.lowHz;
    } else {
        low = dialHz + audio.lowHz;
        high = dialHz + audio.highHz;
    }
    if (rfLow != nullptr) *rfLow = low;
    if (rfHigh != nullptr) *rfHigh = high;
}

// False when the dial is inside an amateur band but the signal reaches beyond one of its edges. A dial outside the
// bands is not judged here (the dial permission already refused it, or the operator confirmed it).
inline bool signalInsideBand(double dialHz, const QString &rigMode, bool assumeLower, Span audio,
                             double *rfLow = nullptr, double *rfHigh = nullptr)
{
    double low = 0.0;
    double high = 0.0;
    rfRange(dialHz, rigMode, assumeLower, audio, &low, &high);
    if (rfLow != nullptr) *rfLow = low;
    if (rfHigh != nullptr) *rfHigh = high;
    const QString band = M3StandardFrequencies::bandOf(dialHz);
    if (band.isEmpty()) return true;
    return M3StandardFrequencies::bandOf(low) == band && M3StandardFrequencies::bandOf(high) == band;
}

} // namespace M3TxBand
