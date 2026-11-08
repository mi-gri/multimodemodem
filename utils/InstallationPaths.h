#pragma once
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStandardPaths>

namespace MadModemInstallation {
inline bool isManaged(const QString &executableDir = QCoreApplication::applicationDirPath())
{
    return QFileInfo::exists(QDir(executableDir).filePath(QStringLiteral("managed-install.flag")));
}

// Never replace an existing user file or remove the original portable data.
// If migration fails, retain access to the original rather than silently loading
// defaults/an empty log. The existing save-error handling reports write failures.
inline QString userFile(const QString &executableDir, const QString &userDir, const QString &name,
                        QString installerBackupRoot = QString())
{
    const QString target = QDir(userDir).filePath(name);
    QString legacy = QDir(executableDir).filePath(name);
#ifdef Q_OS_MACOS
    if (installerBackupRoot.isEmpty())
        installerBackupRoot = QStringLiteral("/Library/Application Support/MultiModeModem/Portable Backups");
#endif
    // macOS replaces the signed bundle atomically. The installer preserves
    // portable data outside it, so migration must also look in that snapshot.
    if (!QFileInfo::exists(legacy) && isManaged(executableDir) && !installerBackupRoot.isEmpty()) {
        const QDir backups(installerBackupRoot);
        const QStringList snapshots = backups.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::Reversed);
        if (!snapshots.isEmpty()) {
            const QString saved = backups.filePath(snapshots.first() + QLatin1Char('/') + name);
            if (QFileInfo::exists(saved)) legacy = saved;
        }
    }
    if (userDir.isEmpty()) return legacy;
    if (!QFileInfo::exists(target) && QFileInfo::exists(legacy)) {
        if (!QDir().mkpath(userDir) || !QFile::copy(legacy, target)) {
            if (!QFileInfo::exists(target)) return legacy;
        }
    }
    QDir().mkpath(userDir);
    return target;
}
// MultiModeModem (m³) runs under its own application name, so Qt's user
// folders moved from …/MadModem to …/MultiModeModem. Copy the settings and the
// default logbook from the MadModem folders once; never overwrite, never
// delete, so MadModem itself keeps working. Diagnostics are not copied.
// Settings files hold account secrets; keep them readable by the owner only.
inline void restrictToOwner(const QString &file)
{
#ifndef Q_OS_WIN
    QFile::setPermissions(file, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
#else
    Q_UNUSED(file);
#endif
}

// Folder of the settings files. M3_CONFIG_DIR overrides it (the tests use
// this so they never touch the operator's settings).
inline QString configDirectory()
{
    const QString overrideDir = qEnvironmentVariable("M3_CONFIG_DIR");
    if (!overrideDir.isEmpty()) return overrideDir;
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

// The configuration chosen with --config; empty = the default configuration.
inline QString &activeConfig()
{
    static QString config;
    return config;
}

// --config takes a name (letters, digits, '.', '_', '-') or a path to a .mad file.
inline bool isConfigFileArgument(const QString &config)
{
    return config.contains(QLatin1Char('/')) || config.contains(QLatin1Char('\\')) ||
           config.endsWith(QStringLiteral(".mad"), Qt::CaseInsensitive);
}

inline bool isValidConfigArgument(const QString &config)
{
    if (config.isEmpty()) return false;
    if (isConfigFileArgument(config)) return true;
    static const QRegularExpression name(QStringLiteral("^[A-Za-z0-9][A-Za-z0-9._-]{0,63}$"));
    return name.match(config).hasMatch();
}

// Short name for the title bar: the name itself or the file's base name.
inline QString configLabel(const QString &config)
{
    return isConfigFileArgument(config) ? QFileInfo(config).completeBaseName() : config;
}

/**
 * @brief Settings file of a configuration.
 *
 * The default configuration lives in @p configDir/settings.mad; settings.mad
 * next to the program (portable builds, older versions) is copied there once
 * and never deleted.  A named configuration is configDir/configs/NAME.mad, a
 * path is used as given.  A configuration that does not exist yet starts as a
 * copy of the default settings.
 */
inline QString settingsFileFor(const QString &executableDir, const QString &configDir, const QString &config)
{
    const QString defaultFile = userFile(executableDir, configDir, QStringLiteral("settings.mad"));
    if (config.isEmpty()) return defaultFile;
    const QString file = isConfigFileArgument(config)
        ? QFileInfo(config).absoluteFilePath()
        : QDir(configDir).filePath(QStringLiteral("configs/") + config + QStringLiteral(".mad"));
    if (!QFileInfo::exists(file)) {
        QDir().mkpath(QFileInfo(file).absolutePath());
        if (QFileInfo::exists(defaultFile) && QFile::copy(defaultFile, file))
            restrictToOwner(file);
    }
    return file;
}

inline void adoptUpstreamUserData()
{
    // Settings next to the program take precedence over MadModem's folder.
    if (!qEnvironmentVariableIsSet("M3_CONFIG_DIR"))
        userFile(QCoreApplication::applicationDirPath(), configDirectory(), QStringLiteral("settings.mad"));
    const struct { QStandardPaths::StandardLocation location; const char *file; } items[] = {
        {QStandardPaths::AppConfigLocation, "settings.mad"},
        {QStandardPaths::AppLocalDataLocation, "logbook.adi"},
    };
    for (const auto &item : items) {
        const QString target = QStandardPaths::writableLocation(item.location);
        if (target.isEmpty()) continue;
        if (item.location == QStandardPaths::AppConfigLocation && qEnvironmentVariableIsSet("M3_CONFIG_DIR")) continue;
        QDir upstreamDir(target);
        if (upstreamDir.dirName() == QStringLiteral("MadModem")) continue;
        upstreamDir.cdUp();
        const QString source = upstreamDir.filePath(QStringLiteral("MadModem/") + QLatin1String(item.file));
        const QString destination = QDir(target).filePath(QLatin1String(item.file));
        if (QFileInfo::exists(destination) || !QFileInfo::exists(source)) continue;
        QDir().mkpath(target);
        QFile::copy(source, destination);
    }
}

// Settings always live in the platform's config folder (Linux:
// ~/.config/MultiModeModem), for installed and portable builds alike.
inline QString settingsFilePath()
{
    return settingsFileFor(QCoreApplication::applicationDirPath(), configDirectory(), activeConfig());
}
}
