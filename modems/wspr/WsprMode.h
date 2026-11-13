// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QString>

// WSPR-2: 2-minute periods starting at even UTC minutes, receive only.
class WsprMode
{
public:
    static QString modeName() { return QStringLiteral("WSPR"); }
    static bool isMode(const QString &mode) { return mode.trimmed().compare(modeName(), Qt::CaseInsensitive) == 0; }
    static constexpr int periodSeconds() { return 120; }
};
