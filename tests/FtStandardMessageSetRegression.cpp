// Standard FT messages for non-standard calls follow WSJT-X genStdMsgs():
// a plain "II4GCLB DO3BOX JO30" packs as truncated free text "II4GCLB DO3BO"
// and "DL/N0PNG DO3BOX JO30" silently loses the prefix.
#include "modems/ft8/FtStandardMessageSet.h"
#include <iostream>

int main()
{
    bool ok = true;
    auto check = [&](bool condition, const char *name) {
        std::cout << (condition ? "PASS " : "FAIL ") << name << '\n';
        ok = condition && ok;
    };
    auto set = [](const char *my, const char *dx) {
        FtStandardMessageSet::Inputs in;
        in.myCall = QString::fromLatin1(my); in.myGrid = QStringLiteral("JO30");
        in.dxCall = QString::fromLatin1(dx); in.report = QStringLiteral("-12");
        return FtStandardMessageSet(in).messages();
    };
    check(FtStandardMessageSet::isStandardCall("G4XEE") && FtStandardMessageSet::isStandardCall("K1ABC/P") &&
          FtStandardMessageSet::isStandardCall("DO3BOX") &&
          !FtStandardMessageSet::isStandardCall("II4GCLB") && !FtStandardMessageSet::isStandardCall("DL/N0PNG") &&
          !FtStandardMessageSet::isStandardCall("PA/DO3BOX"), "WSJT-X stdCall");
    check(set("DO3BOX", "G4XEE") == QStringList{"CQ DO3BOX JO30", "G4XEE DO3BOX JO30", "G4XEE DO3BOX -12",
                                                "G4XEE DO3BOX R-12", "G4XEE DO3BOX RR73", "G4XEE DO3BOX 73"},
          "standard calls unchanged");
    check(set("DO3BOX", "II4GCLB") == QStringList{"CQ DO3BOX JO30", "<II4GCLB> DO3BOX JO30", "<II4GCLB> DO3BOX -12",
                                                  "<II4GCLB> DO3BOX R-12", "II4GCLB <DO3BOX> RR73", "II4GCLB <DO3BOX> 73"},
          "non-standard DX call hashed (WSJT-X)");
    check(set("DO3BOX", "DL/N0PNG").at(1) == QStringLiteral("<DL/N0PNG> DO3BOX JO30"), "prefixed DX call hashed");
    check(set("PA/DO3BOX", "G4XEE") == QStringList{"CQ PA/DO3BOX", "<G4XEE> PA/DO3BOX", "G4XEE <PA/DO3BOX> -12",
                                                   "G4XEE <PA/DO3BOX> R-12", "<G4XEE> PA/DO3BOX RR73", "<G4XEE> PA/DO3BOX 73"},
          "non-standard own call (WSJT-X)");
    return ok ? 0 : 1;
}
