#include "../utils/TxOccupiedBand.h"
#include <cstdio>
#include <cstdlib>

using namespace M3TxBand;
static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

int main()
{
    const Span ft8{1500.0, 1550.0};
    CHECK(signalInsideBand(14074000.0, QStringLiteral("PKTUSB"), false, ft8));
    CHECK(!signalInsideBand(14349000.0, QStringLiteral("USB"), false, ft8));      // dial inside, signal above 14.350
    CHECK(signalInsideBand(14348000.0, QStringLiteral("USB"), false, ft8));       // highest tone still inside
    CHECK(!signalInsideBand(14001000.0, QStringLiteral("LSB"), false, ft8));      // lower sideband below 14.000
    CHECK(signalInsideBand(14001000.0, QStringLiteral("PKTUSB"), false, ft8));
    CHECK(!signalInsideBand(7001000.0, QString(), true, Span{1100.0, 2300.0}));   // unknown mode, LSB assumed
    CHECK(signalInsideBand(7001000.0, QString(), false, Span{1100.0, 2300.0}));
    CHECK(signalInsideBand(14349700.0, QStringLiteral("CW"), false, Span{600.0, 800.0}));   // CW: at the dial
    CHECK(!signalInsideBand(14349900.0, QStringLiteral("CW"), false, Span{600.0, 800.0}));
    CHECK(!signalInsideBand(147995000.0, QStringLiteral("FM"), false, ft8));      // FM spreads around the dial
    CHECK(signalInsideBand(145500000.0, QStringLiteral("PKTFM"), false, ft8));
    CHECK(signalInsideBand(27555000.0, QStringLiteral("USB"), false, ft8));       // outside the bands: dial rule decides
    double low = 0.0, high = 0.0;
    rfRange(7040000.0, QStringLiteral("RTTY"), false, Span{2000.0, 2400.0}, &low, &high);
    CHECK(low == 7037600.0 && high == 7038000.0);
    if (failures == 0) std::puts("TxOccupiedBand regression OK");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
