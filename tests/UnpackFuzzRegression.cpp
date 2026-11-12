// SPDX-License-Identifier: GPL-3.0-or-later
// Random-payload fuzzing of the message unpackers that see over-the-air data.
// Any station can transmit an arbitrary payload with a valid CRC, so the
// unpackers must cope with every bit pattern.  Meant for ASan/UBSan builds;
// ctest runs 20000 rounds, "m3_unpack_fuzz_regression 2000000 11" a long one.
#include "modems/js8/Js8AutoReply.h"
#include "modems/js8/Js8CommandBuffer.h"
#include "modems/js8/Js8Decoder.h"
#include "modems/js8/Js8Inbox.h"
#include "modems/js8/Js8Message.h"
#include "third_party/js8call_gpl/Jsc.h"
#include "HvPackUnpackMsg/pack_unpack_msg77.h"

#include <QCoreApplication>
#include <QTemporaryDir>
#include <cstdio>
#include <random>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    const long iterations = argc > 1 ? std::atol(argv[1]) : 20000;
    std::mt19937_64 rng(argc > 2 ? std::strtoull(argv[2], nullptr, 10) : 1);

    // FT8/FT4/MSK144/Q65 77-bit messages, incl. hashed calls that fill the table.
    PackUnpackMsg77 ft;
    ft.initPackUnpack77(true);
    long ftOk = 0;
    for (long n = 0; n < iterations; ++n) {
        bool c77[100] = {};
        const int mode = int(rng() % 4);
        for (int i = 0; i < 77; ++i) {
            const auto r = rng();
            c77[i] = mode == 0 ? (r & 1) : mode == 1 ? ((r % 8) == 0) : mode == 2 ? ((r % 8) != 0) : (r & 1);
        }
        if (mode == 3) {   // force each i3/n3 message type
            const int i3 = int(rng() % 8), n3 = int(rng() % 8);
            for (int b = 0; b < 3; ++b) { c77[74 + b] = (i3 >> (2 - b)) & 1; c77[71 + b] = (n3 >> (2 - b)) & 1; }
        }
        bool ok = false;
        const QString text = ft.unpack77(c77, ok);
        if (ok) ++ftOk;
        if (ok && (n % 97) == 0) {   // re-pack what was unpacked
            bool back[100] = {};
            int i3 = -1, n3 = -1;
            ft.pack77(text, i3, n3, back);
        }
    }

    // JS8: random 12-character frames (72 bits) with every flag combination,
    // fed through the command buffer and the automatic-reply rules.
    static const QString alphabet = QStringLiteral("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-+");
    QTemporaryDir dir;
    Js8Inbox inbox(dir.path() + QStringLiteral("/inbox.json"));
    inbox.load();
    Js8CommandBuffer buffer;
    Js8ReplyContext ctx;
    ctx.myCall = QStringLiteral("DO3BOX");
    ctx.myGrid = QStringLiteral("JO31");
    ctx.info = QStringLiteral("INFO");
    ctx.status = QStringLiteral("STATUS");
    ctx.autoReply = true;
    ctx.heartbeatAck = true;
    ctx.relay = true;
    ctx.inbox = &inbox;
    ctx.heardRecentFirst = QStringList{QStringLiteral("DL1ABC"), QStringLiteral("OE1XYZ")};
    ctx.heardReport = [](const QString &) { return QStringLiteral("+05 (1m)"); };
    QDateTime utc = QDateTime::fromSecsSinceEpoch(1800000000, Qt::UTC);
    long js8Commands = 0;
    for (long n = 0; n < iterations; ++n) {
        QString frame;
        for (int i = 0; i < 12; ++i) frame += alphabet.at(int(rng() % 64));
        const int bits = int(rng() % 8);
        const auto sub = Js8Mode::submodeFromInt(int(rng() % 5));
        const auto u = Js8Message::unpack(frame, bits, sub);
        Js8Decode d;
        utc = utc.addSecs(15);
        d.utc = utc;
        d.submode = sub;
        d.frame = frame;
        d.bits = bits;
        d.frequencyHz = 1000.0 + double(rng() % 3) * 50.0;   // few offsets: frames join buffers
        d.text = u.text;
        d.frameType = u.frameType;
        d.first = u.first;
        d.last = u.last;
        d.heartbeat = u.heartbeat;
        d.cq = u.cq;
        d.directed = u.directed;
        d.from = u.from;
        d.to = u.to;
        d.command = u.command;
        d.extra = u.extra;
        d.grid = u.grid;
        d.compound = u.frameType == 1;
        QVector<Js8Command> commands = buffer.add(d);
        commands += buffer.expire(utc);
        for (Js8Command c : commands) {
            ++js8Commands;
            if ((rng() % 2) == 0) c.to = ctx.myCall;   // also as if addressed to us
            Js8AutoReply::process(c, ctx);
        }
        // Raw JSC data bits of random length.
        js8::Codeword bitsRaw;
        const int len = int(rng() % 80);
        for (int i = 0; i < len; ++i) bitsRaw << bool(rng() & 1);
        js8::JSC::decompress(bitsRaw);
    }
    std::printf("PASS unpack fuzz iterations=%ld ft_valid=%ld js8_commands=%ld\n", iterations, ftOk, js8Commands);
    return 0;
}
