// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QDateTime>
#include <QString>
#include <QStringList>

// The "Only CQ/RR73/73" filter of the FT decode table: the stations that are
// free to be called - those calling CQ and those ending a QSO with RR73 or 73.
// A line addressed to my call always passes.
namespace FtDecodeFilter {
// A hashed (non-standard) call such as the special call DL75WAU appears in
// messages as "<DL75WAU>": compare without the angle brackets.
inline QString plainCall(QString call)
{
    call.remove(QLatin1Char('<')).remove(QLatin1Char('>'));
    return call.trimmed().toUpper();
}

inline bool passesCqEnd(const QString &message, const QString &myCall)
{
    const QStringList words = message.trimmed().toUpper().split(QLatin1Char(' '), Qt::SkipEmptyParts);
    if (words.isEmpty()) return false;
    if (words.first() == QStringLiteral("CQ")) return true;
    const QString me = plainCall(myCall);
    if (!me.isEmpty() && plainCall(words.first()) == me) return true;
    return words.last() == QStringLiteral("RR73") || words.last() == QStringLiteral("73");
}

// Kind of a decode line, used for its JTDX colour.
enum class LineKind { Standard, Cq, End, MyCall };

inline LineKind lineKind(const QString &message, const QString &myCall)
{
    const QStringList words = message.trimmed().toUpper().split(QLatin1Char(' '), Qt::SkipEmptyParts);
    if (words.isEmpty()) return LineKind::Standard;
    const QString me = plainCall(myCall);
    if (!me.isEmpty() && plainCall(words.first()) == me) return LineKind::MyCall;
    if (words.first() == QStringLiteral("CQ")) return LineKind::Cq;
    if (words.last() == QStringLiteral("RR73") || words.last() == QStringLiteral("73")) return LineKind::End;
    return LineKind::Standard;
}

// The "worked stations" filter: hide a station already in the log.
// days = 0 hides every worked station; days > 0 hides only those whose last
// QSO is at most that many days ago (older contacts are shown again).
inline bool hiddenAsWorked(bool worked, const QDateTime &lastQsoUtc, int days, const QDateTime &nowUtc)
{
    if (!worked) return false;
    if (days <= 0) return true;
    return lastQsoUtc.isValid() && lastQsoUtc.secsTo(nowUtc) <= qint64{days} * 86400;
}

// The "hide countries" filter (JTDX "Hide messages from countries"): a comma
// separated list of DXCC primary prefixes as in cty.dat (e.g. "UA,UA9,JA",
// "*4U1V" for a WAE entity). Spaces are tolerated, case does not matter.
inline QStringList countryPrefixList(const QString &text)
{
    QStringList list;
    const QStringList parts = text.toUpper().split(QLatin1Char(','), Qt::SkipEmptyParts);
    for (const QString &part : parts) {
        const QString prefix = part.trimmed();
        if (!prefix.isEmpty() && !list.contains(prefix)) list << prefix;
    }
    return list;
}

inline bool hiddenByCountry(const QString &primaryPrefix, const QStringList &hiddenPrefixes)
{
    const QString prefix = primaryPrefix.trimmed().toUpper();
    return !prefix.isEmpty() && hiddenPrefixes.contains(prefix);
}

// The "hide continents" filter (JTDX "Hide messages from continents"): the
// cty.dat continent codes, stored comma separated in a fixed order.
inline const QStringList &continentCodes()
{
    static const QStringList codes = {QStringLiteral("AF"), QStringLiteral("AN"), QStringLiteral("AS"),
                                      QStringLiteral("EU"), QStringLiteral("NA"), QStringLiteral("OC"),
                                      QStringLiteral("SA")};
    return codes;
}

inline QStringList continentList(const QString &text)
{
    const QStringList parts = countryPrefixList(text);
    QStringList list;
    for (const QString &code : continentCodes()) {
        if (parts.contains(code)) list << code;
    }
    return list;
}

inline bool hiddenByContinent(const QString &continent, const QStringList &hiddenContinents)
{
    const QString code = continent.trimmed().toUpper();
    return !code.isEmpty() && hiddenContinents.contains(code);
}
} // namespace FtDecodeFilter
