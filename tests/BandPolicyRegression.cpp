// SPDX-License-Identifier: GPL-3.0-or-later
// Settings > Bands: TX permission per band, preferred band order, plan entries
// that may send WSPR beacons.
#include "utils/BandPolicy.h"
#include "dialogs/BandSchedulerDialog.h"

#include <QApplication>
#include <cstdio>

static int g_fail = 0;
static void check(bool ok, const char *name)
{
    if (!ok) ++g_fail;
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", name);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    using namespace M3BandPolicy;
    const QStringList tx = {QStringLiteral("80m"), QStringLiteral("15m"), QStringLiteral("2m"), QStringLiteral("70cm")};
    check(txAllowedOn(false, {}, QStringLiteral("40m")), "not configured: amateur band allowed");
    check(!txAllowedOn(false, {}, QString()), "not configured: outside the bands never");
    check(txAllowedOn(true, tx, QStringLiteral("80m")), "configured: 80m allowed");
    check(!txAllowedOn(true, tx, QStringLiteral("40m")), "configured: 40m receive only");
    check(!txAllowedOn(true, tx, QString()), "configured: unknown band refused");
    check(txAllowedOn(true, tx, QStringLiteral(" 15M ")), "band names trimmed, case-insensitive");
    check(M3StandardFrequencies::bandOf(7078000.0) == QStringLiteral("40m"), "7.078 MHz is 40m");
    check(M3StandardFrequencies::bandOf(432100000.0) == QStringLiteral("70cm"), "432.1 MHz is 70cm");
    check(M3StandardFrequencies::bandOf(27185000.0).isEmpty(), "27.185 MHz outside the amateur bands");

    const QStringList all = {QStringLiteral("160m"), QStringLiteral("80m"), QStringLiteral("40m"), QStringLiteral("15m"), QStringLiteral("2m")};
    check(orderedBands(all, {QStringLiteral("15m"), QStringLiteral("80m")}) ==
              QStringList({QStringLiteral("80m"), QStringLiteral("15m"), QStringLiteral("160m"), QStringLiteral("40m"), QStringLiteral("2m")}),
          "preferred first, both groups in band order");
    check(preferredInBandOrder({QStringLiteral("70cm"), QStringLiteral("80m"), QStringLiteral("2m")}) ==
              QStringList({QStringLiteral("80m"), QStringLiteral("2m"), QStringLiteral("70cm")}),
          "quick-select row in frequency order");

    // Plan entries: only an explicit wspr_tx on a WSPR entry transmits.
    const QString json = QStringLiteral(
        "[{\"time_utc\":\"10:20\",\"mode\":\"WSPR\",\"band\":\"30m\",\"frequency_hz\":10138700,\"wspr_tx\":true},"
        " {\"time_utc\":\"10:30\",\"mode\":\"WSPR\",\"band\":\"20m\",\"frequency_hz\":14095600},"
        " {\"time_utc\":\"10:40\",\"mode\":\"JS8\",\"band\":\"40m\",\"frequency_hz\":7078000,\"wspr_tx\":true}]");
    const QList<ScheduledQsyEntry> entries = BandSchedulerDialog::deserializeEntries(json);
    check(entries.size() == 3, "three plan entries read");
    check(entries.size() == 3 && entries.at(0).wsprTx && !entries.at(1).wsprTx && !entries.at(2).wsprTx,
          "wspr_tx only on the marked WSPR entry (never JS8)");
    const QList<ScheduledQsyEntry> again = BandSchedulerDialog::deserializeEntries(BandSchedulerDialog::serializeEntries(entries));
    check(again.size() == 3 && again.at(0).wsprTx && !again.at(1).wsprTx, "wspr_tx survives save/load");
    check(BandSchedulerDialog::transmittingEntries(entries).size() == 1, "one transmitting entry listed for confirmation");
    std::printf("%s\n", g_fail ? "RESULT FAIL" : "RESULT PASS");
    return g_fail ? 1 : 0;
}
