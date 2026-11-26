// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// Entries of the "stations I am waiting for" list. A fragment like "P" would match every ".../P" station, so only
// text shaped like a callsign is accepted: one part between slashes with a digit followed by letters (long special
// event calls included). Pure logic.
#include <QRegularExpression>
#include <QString>
#include <QStringList>

namespace M3 {

inline bool isWaitListCall(const QString &text)
{
    const QString call = text.trimmed().toUpper();
    static const QRegularExpression allowed(QStringLiteral("^[A-Z0-9/]{3,20}$"));
    static const QRegularExpression part(QStringLiteral("^[A-Z0-9]*[0-9][A-Z]+$"));
    static const QRegularExpression letter(QStringLiteral("[A-Z]"));
    if (!allowed.match(call).hasMatch()) return false;
    for (const QString &piece : call.split(QLatin1Char('/'), Qt::SkipEmptyParts))
        if (piece.size() >= 3 && part.match(piece).hasMatch() && piece.contains(letter)) return true;
    return false;
}

} // namespace M3
