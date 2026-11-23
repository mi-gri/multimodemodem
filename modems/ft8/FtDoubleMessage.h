// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// The FT8 "double message" (DXpedition format, message type 0.1): one transmission that
// finishes one QSO and answers a second station:
//     K1ABC RR73; W9XYZ <DL75WAU> -12
//   RR73 to K1ABC, and W9XYZ gets his report from the (hashed) own call. Pure helpers, no Qt widgets.
#include <QRegularExpression>
#include <QString>

namespace M3 {
namespace FtDoubleMessage {

// A callsign the 28-bit field of the message can carry (standard call: 1-2 character prefix, one digit,
// 1-3 letters suffix; no portable parts, no hashes).
inline bool isStandardCall(const QString &call)
{
    static const QRegularExpression standard(QStringLiteral("^(?:[A-Z]|[0-9][A-Z]|[A-Z][0-9]|[A-Z]{2})[0-9][A-Z]{1,3}$"));
    return standard.match(call.trimmed().toUpper()).hasMatch();
}

// The report field has 2 dB steps: -30 ... +30, rounded to the nearest even value.
inline int fieldReportDb(int snrDb)
{
    int r = (snrDb >= 0 ? snrDb + 1 : snrDb - 1) / 2 * 2;
    if (r > 30) r = 30;
    if (r < -30) r = -30;
    return r;
}

inline QString reportText(int db)
{
    return QStringLiteral("%1%2").arg(db < 0 ? QLatin1Char('-') : QLatin1Char('+')).arg(qAbs(db), 2, 10, QLatin1Char('0'));
}

// "A RR73; B <MYCALL> -nn"; empty when a call cannot be carried.
inline QString build(const QString &callA, const QString &callB, const QString &myCall, int snrOfB)
{
    const QString a = callA.trimmed().toUpper();
    const QString b = callB.trimmed().toUpper();
    const QString my = myCall.trimmed().toUpper();
    if (!isStandardCall(a) || !isStandardCall(b) || a == b || my.isEmpty() || my.contains(QLatin1Char('<'))) return QString();
    return QStringLiteral("%1 RR73; %2 <%3> %4").arg(a, b, my, reportText(fieldReportDb(snrOfB)));
}

// A and B must be heard in slots of the same parity: both listen when we transmit.
inline bool sameParity(qint64 slotStartMsA, qint64 slotStartMsB, int slotMs)
{
    if (slotMs <= 0 || slotStartMsA <= 0 || slotStartMsB <= 0) return false;
    return (slotStartMsA / slotMs) % 2 == (slotStartMsB / slotMs) % 2;
}

} // namespace FtDoubleMessage
} // namespace M3
