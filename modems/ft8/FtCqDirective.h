// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QString>
#include <QStringList>

/**
 * @brief Directed CQ handling for FT8/FT4 ("CQ DX", "CQ NA", "CQ USA" ...).
 *
 * Pure rules without cty.dat access: the caller supplies the continent and
 * ADIF DXCC number of both stations.  Used by Auto QSO to skip directed CQs
 * the own station does not fit, and to work only stations inside the own CQ
 * destination.  Unknown modifiers (POTA, TEST, WW, frequencies ...) and
 * stations without a cty.dat entry never block a QSO.
 */
namespace FtCqDirective {

struct Station
{
    QString continent;   // cty.dat continent: EU, AF, AS, NA, SA, OC, AN
    QString dxcc;        // ADIF DXCC entity number as text
};

enum class Scope { Any, Dx, Continent, Dxcc };

struct Target
{
    Scope scope = Scope::Any;
    QString continent;
    QStringList dxcc;
};

/// "CQ DX" from a station on my continent / in my country is not for me.
enum class DxRule { Continent, Country };

enum class Fit { Yes, No, Unknown };

/// The directive token of a CQ ("DX" in "CQ DX K1ABC FN42"), or empty.
inline QString modifier(const QStringList &parts, const QString &cqCall)
{
    if (parts.size() < 3 || parts.at(0).trimmed().toUpper() != QStringLiteral("CQ")) {
        return QString();
    }
    const QString token = parts.at(1).trimmed().toUpper();
    if (token.isEmpty() || token == cqCall.trimmed().toUpper()) {
        return QString();
    }
    return token;
}

inline Target targetFor(const QString &modifierText)
{
    const QString m = modifierText.trimmed().toUpper();
    Target t;
    if (m == QStringLiteral("DX")) {
        t.scope = Scope::Dx;
    } else if (m == QStringLiteral("EU") || m == QStringLiteral("AF") || m == QStringLiteral("AS") ||
               m == QStringLiteral("NA") || m == QStringLiteral("SA") || m == QStringLiteral("OC") ||
               m == QStringLiteral("AN")) {
        t.scope = Scope::Continent;
        t.continent = m;
    } else if (m == QStringLiteral("USA") || m == QStringLiteral("US")) {
        t.scope = Scope::Dxcc;
        t.dxcc = QStringList{QStringLiteral("291"), QStringLiteral("6"), QStringLiteral("110")};  // USA, Alaska, Hawaii
    } else if (m == QStringLiteral("JA")) {
        t.scope = Scope::Dxcc;
        t.dxcc = QStringList{QStringLiteral("339")};
    } else if (m == QStringLiteral("VK")) {
        t.scope = Scope::Dxcc;
        t.dxcc = QStringList{QStringLiteral("150")};
    } else if (m == QStringLiteral("ZL")) {
        t.scope = Scope::Dxcc;
        t.dxcc = QStringList{QStringLiteral("170")};
    }
    return t;
}

/**
 * @brief Does @p station belong to @p target?
 *
 * For "DX" the reference is the other end: the CQ caller when answering a
 * CQ, my own station when judging a caller to my CQ.
 */
inline Fit fits(const Target &target, const Station &station, const Station &reference, DxRule dxRule)
{
    switch (target.scope) {
    case Scope::Any:
        return Fit::Yes;
    case Scope::Continent:
        if (station.continent.isEmpty()) return Fit::Unknown;
        return station.continent == target.continent ? Fit::Yes : Fit::No;
    case Scope::Dxcc:
        if (station.dxcc.isEmpty()) return Fit::Unknown;
        return target.dxcc.contains(station.dxcc) ? Fit::Yes : Fit::No;
    case Scope::Dx:
        if (dxRule == DxRule::Country) {
            if (station.dxcc.isEmpty() || reference.dxcc.isEmpty()) return Fit::Unknown;
            return station.dxcc != reference.dxcc ? Fit::Yes : Fit::No;
        }
        if (station.continent.isEmpty() || reference.continent.isEmpty()) return Fit::Unknown;
        return station.continent != reference.continent ? Fit::Yes : Fit::No;
    }
    return Fit::Unknown;
}

} // namespace FtCqDirective
