#include "logbook/AdifLogbook.h"
#include <QCoreApplication>
#include <QTemporaryDir>
#include <QFile>
#include <iostream>

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    bool ok = true;
    auto check = [&](bool result, const char *name) { std::cout << (result ? "PASS " : "FAIL ") << name << '\n'; ok &= result; };
    const auto records = AdifLogbook::parseAdif("<CALL:6>IZ6NNH <COMMENT:5><EOH> <MODE:3>FT8 <EOR>");
    check(records.size() == 1 && records[0].callsign == "IZ6NNH" && records[0].comment == "<EOH>", "EOH inside headerless COMMENT");
    if (!records.isEmpty()) check(!AdifLogbook::entryToAdif(records[0]).contains("QSO_DATE"), "missing date stays missing");
    const auto spaces = AdifLogbook::parseAdif("<CALL:6>IZ6NNH <COMMENT:6>a  b c <EOR>");
    check(!spaces.isEmpty() && AdifLogbook::entryToAdif(spaces[0]).contains("a  b c"), "preserve repeated spaces");
    const auto paddedHeader = AdifLogbook::parseAdif("<PROGRAMID:5><EOH>< EOH ><CALL:6>IZ6NNH <EOR>");
    check(paddedHeader.size() == 1 && paddedHeader[0].callsign == "IZ6NNH", "length-aware padded header delimiter");
    QTemporaryDir dir;
    check(dir.isValid(), "temporary directory");
    AdifLogbook first(dir.filePath("log.adi")), second(dir.filePath("log.adi"));
    check(first.load() && second.load(), "load empty snapshots");
    LogbookEntry entry; entry.callsign = "IZ6NNH"; entry.mode = "FT8";
    check(first.append(entry), "first append");
    QString error;
    check(!second.append(entry, &error) && error.contains("externally"), "stale writer rejected");
    check(second.load() && second.count() == 1, "existing QSO retained");
    entry.callsign = "K1ABC";
    check(second.append(entry) && first.load() && first.count() == 2, "reload and append");
    // New QSOs are appended; QSOs another program (WSJT-X) appended meanwhile
    // are taken over, any other external change still requires a reload.
    const QString path = dir.filePath("log.adi");
    auto readFile = [&] { QFile f(path); f.open(QIODevice::ReadOnly); return f.readAll(); };
    const QByteArray beforeAppend = readFile();
    entry.callsign = "DL1AAA";
    check(first.append(entry) && readFile().startsWith(beforeAppend) && first.count() == 3, "append keeps existing bytes");
    {QFile f(path); f.open(QIODevice::Append); f.write("<call:6>G0JEIX <mode:3>FT8 <eor>");}   // no trailing newline
    entry.callsign = "SP2XYZ";
    check(first.append(entry) && first.count() == 5, "external append taken over");
    AdifLogbook fresh(path);
    check(fresh.load() && fresh.count() == 5 && fresh.containsCallsign("G0JEIX") && fresh.containsCallsign("SP2XYZ"), "file holds both QSOs intact");
    const QByteArray full = readFile();
    {QFile f(path); f.open(QIODevice::WriteOnly); f.write(QByteArray(full).replace("DL1AAA", "DL1AAB"));}
    entry.callsign = "OK1ABC";
    check(!first.append(entry, &error) && error.contains("externally"), "rewritten file still rejected");
    const auto legacyFields = AdifLogbook::parseAdif("<CALL:6>IZ6NNH <APP_MADMODEM_QSO_ID:3>abc <EOR>");   // m3-compat
    check(legacyFields.size() == 1 && legacyFields[0].adifFields.value("APP_M3_QSO_ID") == "abc" &&
          !legacyFields[0].adifFields.contains("APP_MADMODEM_QSO_ID"), "fields written before the rename are read as APP_M3_*");   // m3-compat
    const auto malformed = AdifLogbook::parseAdif("<CALL:2147483647>X");
    check(malformed.isEmpty(), "oversized field rejected without overflow");
    return ok ? 0 : 1;
}
