// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <QIcon>
#include <QSize>
#include <QString>

namespace M3AppIcon {
// All sizes of the program icon, so Qt picks the simplified artwork
// (antenna and m³ only) for 16-64 px instead of scaling down the full logo.
inline QIcon icon()
{
    QIcon icon;
    for (int size : {16, 24, 32, 48, 64, 128, 256, 512})
        icon.addFile(QStringLiteral(":/icons/multimodemodem_%1.png").arg(size), QSize(size, size));
    return icon;
}
}
