// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QDir>
#include <QFileInfo>
#include <QLockFile>
#include <QString>
#include <QSysInfo>

#include <memory>

/**
 * @brief One running instance per configuration.
 *
 * Two instances on the same settings file would overwrite each other's
 * settings, logbook appends and session state.  The lock file sits next to
 * the settings file (settings.mad.instance, configs/NAME.mad.instance), so instances
 * with different configurations (--config) run side by side.  Not ".lock":
 * QSettings itself locks the file through settings.mad.lock while it syncs,
 * and would wait forever for this lock.  QLockFile
 * stores the PID: a lock left by a crashed instance is taken over.
 */
namespace M3InstanceLock {

/// The folder is resolved (symlinks) whether or not the settings file exists
/// yet, so the first start and later starts agree on the lock.
inline QString lockFilePath(const QString &settingsFile)
{
    const QFileInfo info(settingsFile);
    const QString folder = info.absolutePath();
    QDir().mkpath(folder);
    const QString canonicalFolder = QDir(folder).canonicalPath();
    return QDir(canonicalFolder.isEmpty() ? folder : canonicalFolder).filePath(info.fileName() + QStringLiteral(".instance"));
}

enum class Result { Acquired, AlreadyRunning, Unavailable };

/// Tries to lock @p settingsFile.  On Acquired, @p lock holds the lock for as
/// long as it lives.  Unavailable means the lock file cannot be written (for
/// example a read-only folder); the caller should start anyway.
/// @p owner receives the PID of the running instance, with "@host" when the
/// lock belongs to another computer (shared home folder).
inline Result acquire(const QString &settingsFile, std::unique_ptr<QLockFile> &lock, QString *owner = nullptr)
{
    const QString path = lockFilePath(settingsFile);
    auto candidate = std::make_unique<QLockFile>(path);
    // The lock is never refreshed while the program runs, so it must not
    // become stale by age; only a dead owner process frees it.
    candidate->setStaleLockTime(0);
    if (candidate->tryLock(0)) {
        lock = std::move(candidate);
        return Result::Acquired;
    }
    if (candidate->error() != QLockFile::LockFailedError) return Result::Unavailable;
    if (owner) {
        qint64 pid = 0;
        QString host, app;
        if (candidate->getLockInfo(&pid, &host, &app) && pid > 0) {
            *owner = QString::number(pid);
            if (!host.isEmpty() && host != QSysInfo::machineHostName()) *owner += QLatin1Char('@') + host;
        } else {
            *owner = QStringLiteral("?");
        }
    }
    return Result::AlreadyRunning;
}

} // namespace M3InstanceLock
