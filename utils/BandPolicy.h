// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "StandardFrequencies.h"

#include <QStringList>

/**
 * @brief Which bands may transmit, and the order of band selectors.
 *
 * Pure functions on the Settings > Bands lists, shared by the TX gate, the
 * quick-select row and the band combos.
 */
namespace M3BandPolicy {

/// TX allowed on @p band ("40m")?  An empty band (frequency outside the
/// amateur bands, or unknown) is never allowed.  Not configured yet: every
/// amateur band is allowed (behaviour of earlier versions).
inline bool txAllowedOn(bool configured, const QStringList &txBands, const QString &band)
{
    const QString b = band.trimmed().toLower();
    if (b.isEmpty()) return false;
    if (!configured) return true;
    return txBands.contains(b);
}

/// @p bands with the preferred ones first (in @p bands order), the rest after.
inline QStringList orderedBands(const QStringList &bands, const QStringList &preferred)
{
    QStringList first, rest;
    for (const QString &b : bands) {
        (preferred.contains(b.trimmed().toLower()) ? first : rest) << b;
    }
    return first + rest;
}

/// Preferred bands in frequency order (quick-select row).
inline QStringList preferredInBandOrder(const QStringList &preferred)
{
    QStringList out;
    for (const QString &b : M3StandardFrequencies::allBands())
        if (preferred.contains(b)) out << b;
    return out;
}

} // namespace M3BandPolicy
