// SPDX-License-Identifier: GPL-3.0-or-later
#include "modems/ft8/FtCqDirective.h"
#include <iostream>
#include <stdexcept>
using namespace FtCqDirective;
static void check(bool b, const char *why) { if (!b) throw std::runtime_error(why); }
int main()
{
    const Station dl{QStringLiteral("EU"), QStringLiteral("230")};
    const Station f{QStringLiteral("EU"), QStringLiteral("227")};
    const Station w{QStringLiteral("NA"), QStringLiteral("291")};
    const Station kl{QStringLiteral("NA"), QStringLiteral("6")};
    const Station ja{QStringLiteral("AS"), QStringLiteral("339")};
    const Station unknown;

    check(modifier({"CQ", "DX", "K1ABC", "FN42"}, "K1ABC") == "DX", "CQ DX modifier");
    check(modifier({"CQ", "K1ABC", "FN42"}, "K1ABC").isEmpty(), "general CQ has a modifier");
    check(modifier({"CQ", "K1ABC"}, "K1ABC").isEmpty(), "short CQ has a modifier");

    // Answering CQs: station = me (DL), reference = CQ caller.
    const auto dx = targetFor("DX");
    check(fits(dx, dl, f, DxRule::Continent) == Fit::No, "CQ DX from EU answered by EU");
    check(fits(dx, dl, f, DxRule::Country) == Fit::Yes, "CQ DX from F refused for DL with country rule");
    check(fits(dx, dl, dl, DxRule::Country) == Fit::No, "CQ DX from DL answered by DL with country rule");
    check(fits(dx, dl, w, DxRule::Continent) == Fit::Yes, "CQ DX from W refused for DL");
    check(fits(targetFor("NA"), dl, w, DxRule::Continent) == Fit::No, "CQ NA answered from EU");
    check(fits(targetFor("EU"), dl, w, DxRule::Continent) == Fit::Yes, "CQ EU refused for EU");
    check(fits(targetFor("JA"), dl, ja, DxRule::Continent) == Fit::No, "CQ JA answered from DL");
    check(fits(targetFor("POTA"), dl, w, DxRule::Continent) == Fit::Yes, "CQ POTA refused");
    check(fits(targetFor("290"), dl, w, DxRule::Continent) == Fit::Yes, "CQ 290 refused");
    check(fits(targetFor("NA"), unknown, w, DxRule::Continent) == Fit::Unknown, "missing cty blocked");

    // Own CQ destination: station = caller, reference = me.
    const auto usa = targetFor("USA");
    check(fits(usa, w, dl, DxRule::Continent) == Fit::Yes, "W outside CQ USA");
    check(fits(usa, kl, dl, DxRule::Continent) == Fit::Yes, "Alaska outside CQ USA");
    check(fits(usa, f, dl, DxRule::Continent) == Fit::No, "F inside CQ USA");
    check(fits(dx, f, dl, DxRule::Continent) == Fit::No, "EU caller accepted for my CQ DX");
    check(fits(dx, ja, dl, DxRule::Continent) == Fit::Yes, "JA caller refused for my CQ DX");
    std::cout << "PASS modifier/DX continent+country/continent/DXCC/unknown/own destination\n";
    return 0;
}
