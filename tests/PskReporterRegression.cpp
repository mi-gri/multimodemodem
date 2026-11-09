// SPDX-License-Identifier: GPL-3.0-or-later
// PSK Reporter datagram layout (network/PskReporter, from JTDX).
#include "dsp/text/TextCallSpotter.h"
#include "network/PskReporter.h"

#include <QCoreApplication>
#include <cstdio>

static int failures = 0;
static void check(bool ok, const char *what)
{
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) ++failures;
}
static int u16(const QByteArray &b, int at) { return (quint8(b[at]) << 8) | quint8(b[at + 1]); }

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    using namespace PskReport;
    const LocalStation local{QStringLiteral("DO3BOX"), QStringLiteral("JO31"), QStringLiteral("Dipole"),
                             QStringLiteral("MultiModeModem 0.6.5")};
    QQueue<Spot> queue;
    queue.enqueue(Spot{QStringLiteral("K1ABC"), QStringLiteral("FN42"), 14074000 + 1234, QStringLiteral("FT8"), -12, 0x01020304});
    const QByteArray d = takeDatagram(queue, local, 7, 0xAABBCCDD, 0x11223344);
    check(queue.isEmpty(), "the spot was taken");
    check(u16(d, 0) == 0x000A && u16(d, 2) == d.size(), "IPFIX header: version 10 and total length");
    check(d.mid(4, 4) == QByteArray::fromHex("11223344") && d.mid(8, 4) == QByteArray::fromHex("00000007") &&
              d.mid(12, 4) == QByteArray::fromHex("AABBCCDD"), "header: export time, sequence, random id");
    const int rxAt = d.indexOf(QByteArray::fromHex("50E2"), 16 + 44);
    const int txAt = d.indexOf(QByteArray::fromHex("50E3"), rxAt);
    check(rxAt == 16 + 44 + 60 + 52, "JTDX's templates (44 + 60 bytes) plus the sender template without SNR (52)");
    check(rxAt > 0 && txAt == rxAt + u16(d, rxAt + 2), "receiver set length");
    check(txAt + u16(d, txAt + 2) == d.size(), "sender set length");
    QByteArray expect;
    expect += char(5); expect += "K1ABC";
    expect += QByteArray::fromHex("0000D6C562");   // 14075234 Hz in 5 bytes
    expect += char(-12);
    expect += char(3); expect += "FT8";
    expect += char(4); expect += "FN42";
    expect += char(1);
    expect += QByteArray::fromHex("01020304");
    expect += QByteArray::fromHex("0000");
    check(d.mid(txAt + 4) == expect, "spot record: call, 5-byte frequency, SNR, mode, grid, source, time");

    for (int i = 0; i < 200; ++i)
        queue.enqueue(Spot{QStringLiteral("DL%1XYZ").arg(i), QStringLiteral("JO62QM"), 10489540000LL, QStringLiteral("FT8"), 5, 1});
    int datagrams = 0, biggest = 0;
    while (!queue.isEmpty()) {
        const QByteArray part = takeDatagram(queue, local, 8 + datagrams, 1, 1);
        biggest = qMax(biggest, part.size());
        ++datagrams;
    }
    check(datagrams > 1 && biggest <= kMaxPayloadBytes, "many spots split into datagrams of at most 1200 bytes");

    check(isReportable(QStringLiteral("K1ABC"), QStringLiteral("FN42"), 14074000, QStringLiteral("DO3BOX")) &&
              isReportable(QStringLiteral("PA/DL1ABC"), QStringLiteral("JO22ab"), 7074000, QString()),
          "plain calls with a locator are reported");
    check(!isReportable(QStringLiteral("<K1ABC>"), QStringLiteral("FN42"), 14074000, QString()) &&
              !isReportable(QStringLiteral("K1ABC"), QStringLiteral("RR73"), 14074000, QString()) &&
              !isReportable(QStringLiteral("K1ABC"), QString(), 14074000, QString()) &&
              !isReportable(QStringLiteral("K1ABC"), QStringLiteral("FN42"), 0, QString()) &&
              !isReportable(QStringLiteral("DO3BOX"), QStringLiteral("JO31"), 14074000, QStringLiteral("DO3BOX")),
          "hashed calls, RR73, no locator, no dial, own call are not reported");
    // Text-mode spots: template 0x50E4 without SNR, no locator.
    QQueue<Spot> text;
    Spot rtty{QStringLiteral("K1ABC"), QString(), 14080000 + 2210, QStringLiteral("RTTY"), 0, 0x01020304};
    rtty.hasSnr = false;
    text.enqueue(rtty);
    text.enqueue(Spot{QStringLiteral("DL1ABC"), QStringLiteral("JO31"), 14074000, QStringLiteral("FT8"), -3, 1});
    const QByteArray t = takeDatagram(text, local, 9, 1, 1);
    check(text.size() == 1 && text.head().hasSnr, "one datagram carries spots of one kind only");
    const int noSnrAt = t.lastIndexOf(QByteArray::fromHex("50E4"));
    QByteArray expectText;
    expectText += char(5); expectText += "K1ABC";
    expectText += QByteArray::fromHex("0000D6E0A2");   // 14082210 Hz
    expectText += char(4); expectText += "RTTY";
    expectText += char(0);
    expectText += char(1);
    expectText += QByteArray::fromHex("01020304");
    expectText += QByteArray::fromHex("0000");
    check(noSnrAt > 0 && noSnrAt + u16(t, noSnrAt + 2) == t.size(), "no-SNR set length");
    check(t.mid(noSnrAt + 4) == expectText,
          "text spot record: call, frequency, mode, empty grid, source, time (no SNR)");
    check(isReportable(QStringLiteral("K1ABC"), QString(), 14080000, QString(), false) &&
              !isReportable(QStringLiteral("K1ABC"), QStringLiteral("RR73"), 14080000, QString(), false),
          "locator optional for text modes, but a given one must be valid");

    // Callsign spotting in decoded text.
    TextCallSpotter spotter;
    check(spotter.append(QStringLiteral("cq cq cq de dl1abc dl1abc dl1abc pse k ")) == QStringList{QStringLiteral("DL1ABC")},
          "CQ call confirmed by repetition");
    check(spotter.append(QStringLiteral("K1ABC DE G4XYZ G4XYZ KN ")) == QStringList{QStringLiteral("G4XYZ")},
          "DE identification");
    check(spotter.append(QStringLiteral("TNX DE K1ABX K1ABC ")).isEmpty(), "two different calls are not an ID");
    check(spotter.append(QStringLiteral("JO31 JO31 DL1ABC DL1ABC ")).isEmpty(), "calls outside CQ/DE are ignored");
    check(spotter.append(QStringLiteral("CQ JO31 JO31 73 ")).isEmpty(), "locators are no calls");
    check(spotter.append(QStringLiteral("CQ TEST DE OK1X")).isEmpty() && spotter.append(QStringLiteral("YZ OK1XYZ ")) ==
              QStringList{QStringLiteral("OK1XYZ")}, "text arriving in pieces");
    return failures == 0 ? 0 : 1;
}
