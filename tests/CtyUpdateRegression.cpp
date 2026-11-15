// cty.csv update: a downloaded file replaces the data in use only when it is
// complete and not older; the newer of bundled and user copy is used at start.
#include "dxcc/CtyCountryFile.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTemporaryDir>
#include <iostream>

int main(int argc, char **argv)
{
    QTemporaryDir home;
    qputenv("XDG_DATA_HOME", home.path().toUtf8());     // never touch the real user folder
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("MultiModeModem"));
    bool ok = true;
    auto check = [&](bool condition, const char *name) {
        std::cout << (condition ? "PASS " : "FAIL ") << name << '\n';
        ok = condition && ok;
    };
    QFile bundled(CtyCountryFile::bundledFilePath());
    if (!bundled.open(QIODevice::ReadOnly)) { std::cout << "SKIP no bundled cty.csv\n"; return 0; }
    const QByteArray original = bundled.readAll();
    const CtyCountryFile &start = CtyCountryFile::instance();
    const QString startVersion = start.version();
    check(start.isLoaded() && start.entityCount() >= CtyCountryFile::kMinimumEntities && startVersion.size() == 8,
          "bundled file loaded with version");
    check(CtyCountryFile::userFilePath().startsWith(home.path()), "user copy lives in the data folder");

    QByteArray newer = original;
    newer.replace(("=VER" + startVersion).toLatin1(), QByteArray("=VER29991231"));
    QByteArray older = original;
    older.replace(("=VER" + startVersion).toLatin1(), QByteArray("=VER20000101"));

    auto r = CtyCountryFile::installDownloaded(original.left(original.size() / 3));
    check(!r.ok && !r.updated && CtyCountryFile::instance().version() == startVersion, "truncated download rejected");
    r = CtyCountryFile::installDownloaded(QByteArray("<html>Not found</html>").repeated(1000));
    check(!r.ok && !QFile::exists(CtyCountryFile::userFilePath()), "HTML page rejected, nothing saved");
    r = CtyCountryFile::installDownloaded(older);
    check(r.ok && !r.updated && CtyCountryFile::instance().version() == startVersion, "older file kept out");
    r = CtyCountryFile::installDownloaded(original);
    check(r.ok && !r.updated, "same version: up to date");
    const CtyCountryFile &before = CtyCountryFile::instance();
    r = CtyCountryFile::installDownloaded(newer);
    check(r.ok && r.updated && CtyCountryFile::instance().version() == QStringLiteral("29991231") &&
          QFile::exists(CtyCountryFile::userFilePath()), "newer file installed and in use");
    check(before.lookupCallsign(QStringLiteral("DL1ABC")).valid, "previous data set still valid for running lookups");
    const auto dl = CtyCountryFile::instance().lookupCallsign(QStringLiteral("DL1ABC"));
    check(dl.valid && dl.entity.primaryPrefix == QStringLiteral("DL"), "lookups work on the new data");
    {   // Operating suffixes are not prefixes: /R is the FT rover flag, not Russia.
        const CtyCountryFile &c = CtyCountryFile::instance();
        const auto rover = c.lookupCallsign(QStringLiteral("W1ABC/R"));
        check(rover.valid && rover.entity.primaryPrefix == QStringLiteral("K"), "W1ABC/R stays USA");
        check(!c.lookupCallsign(QStringLiteral("0W2QAL/R")).valid, "noise call 0W2QAL/R is no valid call");
        check(c.lookupCallsign(QStringLiteral("DL1ABC/P")).entity.primaryPrefix == QStringLiteral("DL"), "DL1ABC/P stays DL");
        check(c.primaryPrefixForFilterToken(QStringLiteral("R9")) == QStringLiteral("UA9") &&
              c.primaryPrefixForFilterToken(QStringLiteral("UA")) == QStringLiteral("UA") &&
              c.primaryPrefixForFilterToken(QStringLiteral("Q9")).isEmpty(), "country filter tokens resolve to primary prefixes");
    }
    std::cout << r.message.toStdString() << '\n';
    return ok ? 0 : 1;
}
