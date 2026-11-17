#include "../utils/InstallationPaths.h"
#include <QTemporaryDir>
#include <QTextStream>

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QTemporaryDir storage;
    if (!storage.isValid()) return 1;
    bool all = true;
    auto check = [&](bool ok, const char *name) {
        all &= ok; QTextStream(stdout) << (ok ? "PASS " : "FAIL ") << name << '\n';
    };
    auto write = [&](const QString &path, const QByteArray &data) {
        QFile file(path); return file.open(QIODevice::WriteOnly) && file.write(data) == data.size();
    };
    auto read = [](const QString &path) { QFile file(path); file.open(QIODevice::ReadOnly); return file.readAll(); };
    const QString install = storage.filePath("application"), user = storage.filePath("user/data");
    QDir().mkpath(install);
    check(!M3Installation::isManaged(install), "portable builds retain portable behavior");
    check(write(install + "/managed-install.flag", "managed"), "managed marker created");
    check(M3Installation::isManaged(install), "managed installation recognized");
    for (const QString &name : {QStringLiteral("settings.mad"), QStringLiteral("logbook.adi")}) {
        const QString legacy = install + '/' + name, target = user + '/' + name;
        check(write(legacy, "original user data"), "legacy data available");
        check(M3Installation::userFile(install, user, name) == target, "migrate to writable user folder");
        check(read(target) == "original user data" && read(legacy) == "original user data", "migration preserves original bytes and source");
        check(write(target, "newer user data"), "user data changed after installation");
        check(M3Installation::userFile(install, user, name) == target && read(target) == "newer user data", "reinstall never overwrites newer user data");
        check(M3Installation::userFile(install, QString(), name) == legacy, "missing platform data path preserves legacy access");
        const QString blocker = storage.filePath("blocked");
        check(write(blocker, "not a directory"), "migration failure fixture");
        check(M3Installation::userFile(install, blocker + "/child", name) == legacy, "failed migration retains original data access");
    }
    const QString empty = storage.filePath("fresh/user");
    check(M3Installation::userFile(install, empty, "new-log.adi") == empty + "/new-log.adi" && QDir(empty).exists(), "new install creates user directory without inventing a log");
    const QString backups = storage.filePath("Portable Backups");
    QDir().mkpath(backups + "/20261011T100000Z-old");
    QDir().mkpath(backups + "/20261011T110000Z-new");
    const QString saved = backups + "/20261011T110000Z-new/settings.mad";
    check(write(backups + "/20261011T100000Z-old/settings.mad", "older") && write(saved, "latest portable settings"), "installer snapshots available");
    QFile::remove(install + "/settings.mad");
    const QString migrated = storage.filePath("mac-user");
    check(M3Installation::userFile(install, migrated, "settings.mad", backups) == migrated + "/settings.mad" &&
          read(migrated + "/settings.mad") == "latest portable settings" && read(saved) == "latest portable settings",
          "atomic Mac upgrade migrates preserved portable data without changing backup");
    check(write(migrated + "/settings.mad", "current user settings") &&
          M3Installation::userFile(install, migrated, "settings.mad", backups) == migrated + "/settings.mad" &&
          read(migrated + "/settings.mad") == "current user settings", "Mac reinstall keeps newer user settings");
    check(M3Installation::userFile(install, storage.filePath("blocked/child"), "settings.mad", backups) == saved,
          "failed Mac migration retains access to preserved data");

    // Central settings and named configurations (--config): *.m3cfg, old *.mad converted.
    const QString portable = storage.filePath("portable"), config = storage.filePath("config");
    QDir().mkpath(portable);
    check(write(portable + "/settings.mad", "portable settings"), "portable settings fixture");
    check(M3Installation::settingsFileFor(portable, config, QString()) == config + "/settings.m3cfg" &&
          read(config + "/settings.m3cfg") == "portable settings" && read(portable + "/settings.mad") == "portable settings" &&
          !QFileInfo::exists(config + "/settings.mad"),
          "portable settings.mad becomes settings.m3cfg in the config folder, portable file kept");
    check(write(config + "/settings.m3cfg", "central settings") &&
          M3Installation::settingsFileFor(portable, config, QString()) == config + "/settings.m3cfg" &&
          read(config + "/settings.m3cfg") == "central settings", "central settings never overwritten by portable file");
    // settings.mad in the config folder is renamed
    const QString oldCfg = storage.filePath("oldcfg");
    QDir().mkpath(oldCfg);
    check(write(oldCfg + "/settings.mad", "old default") &&
          M3Installation::settingsFileFor(storage.filePath("noportable"), oldCfg, QString()) == oldCfg + "/settings.m3cfg" &&
          read(oldCfg + "/settings.m3cfg") == "old default" && !QFileInfo::exists(oldCfg + "/settings.mad"),
          "settings.mad in the config folder is renamed to settings.m3cfg");
    // an existing settings.m3cfg wins; the old file is left untouched
    check(write(oldCfg + "/settings.mad", "stale old") && M3Installation::settingsFileFor(storage.filePath("noportable"), oldCfg, QString()) == oldCfg + "/settings.m3cfg" &&
          read(oldCfg + "/settings.m3cfg") == "old default" && read(oldCfg + "/settings.mad") == "stale old",
          "existing settings.m3cfg is never replaced by an old settings.mad");
    // named configurations
    QDir().mkpath(config + "/configs");
    check(write(config + "/configs/ftx-1.mad", "ftx-1 old"), "old named configuration fixture");
    check(M3Installation::settingsFileFor(portable, config, "ftx-1") == config + "/configs/ftx-1.m3cfg" &&
          read(config + "/configs/ftx-1.m3cfg") == "ftx-1 old" && !QFileInfo::exists(config + "/configs/ftx-1.mad"),
          "configs/NAME.mad is renamed to NAME.m3cfg at start");
    // a stale instance lock of the old name is cleaned up when its config is converted
    {
        const QString oldFile = config + "/configs/stale.mad", lockPath = oldFile + ".instance";
        QLockFile lf(lockPath); lf.setStaleLockTime(0);
        QByteArray content;
        if (lf.tryLock(0)) { QFile f(lockPath); if (f.open(QIODevice::ReadOnly)) content = f.readAll(); lf.unlock(); }
        QStringList lines = QString::fromUtf8(content).split('\n'); if (!lines.isEmpty()) lines[0] = "2147483646";   // no such process
        check(write(oldFile, "stale old") && write(lockPath, lines.join('\n').toUtf8()) && QFileInfo::exists(lockPath), "stale lock fixture");
        M3Installation::settingsFileFor(portable, config, "stale");
        check(!QFileInfo::exists(lockPath) && QFileInfo::exists(config + "/configs/stale.m3cfg"), "stale NAME.mad.instance removed when the config is converted");
    }
    const QString contest = config + "/configs/contest.m3cfg";
    check(M3Installation::settingsFileFor(portable, config, "contest") == contest &&
          read(contest) == "central settings", "new named configuration starts as copy of default");
#ifndef Q_OS_WIN
    check(!(QFileInfo(contest).permissions() & (QFileDevice::ReadGroup | QFileDevice::ReadOther | QFileDevice::WriteGroup | QFileDevice::WriteOther)),
          "copied configuration readable by the owner only");
    check(!(QFileInfo(config + "/configs/ftx-1.m3cfg").permissions() & (QFileDevice::ReadGroup | QFileDevice::ReadOther)),
          "converted configuration readable by the owner only");
#endif
    check(write(contest, "contest settings") &&
          M3Installation::settingsFileFor(portable, config, "contest") == contest &&
          read(contest) == "contest settings" && read(config + "/settings.m3cfg") == "central settings",
          "named configuration independent of default");
    const QString explicitFile = storage.filePath("elsewhere/field.m3cfg");
    check(M3Installation::settingsFileFor(portable, config, explicitFile) == explicitFile &&
          read(explicitFile) == "central settings", "configuration given as file path");
    // a path to an old .mad continues as .m3cfg beside it
    QDir().mkpath(storage.filePath("elsewhere"));
    check(write(storage.filePath("elsewhere/day.mad"), "day old") &&
          M3Installation::settingsFileFor(portable, config, storage.filePath("elsewhere/day.mad")) == storage.filePath("elsewhere/day.m3cfg") &&
          read(storage.filePath("elsewhere/day.m3cfg")) == "day old" && !QFileInfo::exists(storage.filePath("elsewhere/day.mad")),
          "path to an old .mad file is converted");
    check(M3Installation::isValidConfigArgument("contest") && M3Installation::isValidConfigArgument("field-2026.day1") &&
          M3Installation::isValidConfigArgument("/tmp/x.m3cfg") && M3Installation::isValidConfigArgument("x.mad") &&
          !M3Installation::isValidConfigArgument("") && !M3Installation::isValidConfigArgument("..") &&
          !M3Installation::isValidConfigArgument("a b") && M3Installation::configLabel(explicitFile) == "field",
          "configuration names validated");
    return all ? 0 : 1;
}
