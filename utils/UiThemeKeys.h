#pragma once

#include <QString>

namespace M3Ui {

inline QString defaultUiThemeKey()
{
    return QStringLiteral("classic_blue_gray");
}

inline bool isM3ColorTheme(const QString &key)
{
    return key == defaultUiThemeKey() || key == QStringLiteral("m3_green") ||
           key == QStringLiteral("m3_red");
}

inline QString normalizedThemeKey(const QString &themeKey)
{
    const QString key = themeKey.trimmed().toLower();
    if (isM3ColorTheme(key) || key == QStringLiteral("avionica") ||
        key == QStringLiteral("qt_default") || key == QStringLiteral("hacker_green") ||
        key == QStringLiteral("classic_dark") || key == QStringLiteral("high_contrast")) {
        return key;
    }
    return defaultUiThemeKey();
}

} // namespace M3Ui
