// SPDX-License-Identifier: GPL-3.0-or-later
// JS8: codec, message layer and transmitter -> receiver loopback.
//
//   m3_js8_regression                 run the checks
//   m3_js8_regression --wav DIR       also write 12 kHz WAVs of the loopback
//                                     signals (for cross-checks with JS8Call's
//                                     js8 decoder: js8 -8 -b A -d 3 file.wav)
//   m3_js8_regression --decode FILE SUBMODE   decode a 12 kHz 16-bit WAV
#include "modems/js8/Js8Decoder.h"
#include "modems/js8/Js8Ldpc.h"
#include "modems/js8/Js8Message.h"
#include "modems/js8/Js8NativeEngine.h"
#include "modems/js8/tx/Js8Transmitter.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>

#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>

static void check(bool ok, const std::string &why)
{
    if (!ok) throw std::runtime_error(why);
}

static QVector<float> waveform(const QVector<Js8Message::Frame> &frames, Js8Mode::Submode submode, double hz)
{
    Js8Transmitter tx(frames, submode, 12000, hz);
    check(tx.generationSucceeded(), "transmitter: " + tx.generationError().toStdString());
    QVector<float> out(int(std::ceil(tx.durationSeconds() * 12000)) + 12000, 0.0f);
    const int n = tx.generate(out.data(), out.size());
    out.resize(qMax(n, Js8Mode::parameters(submode).periodSeconds * 12000));
    return out;
}

// Adds white noise so that the signal (amplitude a) has snrDb in 2500 Hz.
static void addNoise(QVector<float> &x, double signalRms, double snrDb, unsigned seed)
{
    std::mt19937 rng(seed);
    const double noiseRms2500 = signalRms / std::pow(10.0, snrDb / 20.0);
    const double sigma = noiseRms2500 * std::sqrt(6000.0 / 2500.0);
    std::normal_distribution<double> n(0.0, sigma);
    for (float &v : x) v = float(v + n(rng));
}

static void writeWav(const QString &path, const QVector<float> &x)
{
    QFile f(path);
    check(f.open(QIODevice::WriteOnly), "write wav");
    QDataStream s(&f);
    s.setByteOrder(QDataStream::LittleEndian);
    const quint32 bytes = quint32(x.size() * 2);
    s.writeRawData("RIFF", 4); s << quint32(36 + bytes); s.writeRawData("WAVEfmt ", 8);
    s << quint32(16) << quint16(1) << quint16(1) << quint32(12000) << quint32(24000) << quint16(2) << quint16(16);
    s.writeRawData("data", 4); s << bytes;
    for (float v : x) s << qint16(qBound(-32767, int(std::lround(v * 32767.0 * 0.3)), 32767));
}

static QVector<float> readWav(const QString &path)
{
    QFile f(path);
    check(f.open(QIODevice::ReadOnly), "read wav");
    const QByteArray all = f.readAll();
    const int data = all.indexOf("data");
    check(data > 0, "wav data chunk");
    QVector<float> x;
    for (int i = data + 8; i + 1 < all.size(); i += 2)
        x << float(qint16(quint8(all[i]) | (quint8(all[i + 1]) << 8)) / 32768.0);
    return x;
}

static QVector<Js8Decode> decodeAll(const QVector<float> &x, Js8Mode::Submode submode, int depth = 3)
{
    Js8NativeEngine::Configuration c;
    c.submode = submode;
    c.depth = depth;
    return Js8Decoder::decodeSamples(x, QDateTime::currentDateTimeUtc(), c);
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    const QStringList args = app.arguments();
    if (args.size() >= 4 && args.at(1) == QLatin1String("--decode")) {
        const QVector<float> x = readWav(args.at(2));
        const auto sub = Js8Mode::submodeFromInt(args.at(3).toInt());
        const auto p = Js8Mode::parameters(sub);
        for (int start = 0; start + p.nmax() <= x.size(); start += p.periodSeconds * 12000) {
            for (const Js8Decode &d : decodeAll(x.mid(start, p.nmax()), sub))
                std::cout << start / 12000 << "s " << d.snrDb << " dB " << d.dtSeconds << " " << qRound(d.frequencyHz)
                          << " Hz " << d.frame.toStdString() << " [" << d.bits << "] " << d.text.toStdString() << "\n";
        }
        return 0;
    }
    if (args.size() >= 3 && args.at(1) == QLatin1String("--sweep")) {
        const auto sub = Js8Mode::submodeFromInt(args.at(2).toInt());
        const auto frames = Js8Message::buildFrames("DO3BOX", "JO31", QString(), "DO3BOX: " + Js8Message::heartbeatText("JO31"), sub);
        const auto p = Js8Mode::parameters(sub);
        for (int snr = -14; snr >= -28; snr -= 2) {
            int ok = 0;
            for (int seed = 0; seed < 10; ++seed) {
                QVector<float> x = waveform(frames, sub, 1000.0 + 37 * seed);
                x.resize(p.periodSeconds * 12000);
                addNoise(x, 0.95 / std::sqrt(2.0), snr, 1000u + unsigned(seed));
                if (args.size() >= 5)
                    writeWav(QDir(args.at(4)).filePath(QStringLiteral("sweep_%1_%2_%3.wav").arg(QChar(p.letter)).arg(-snr).arg(seed)), x);
                for (const Js8Decode &d : decodeAll(x.mid(0, p.nmax()), sub, args.value(3, "3").toInt()))
                    if (d.frame == frames.first().frame) { ++ok; break; }
            }
            std::cout << snr << " dB: " << ok << "/10" << std::endl;
        }
        return 0;
    }
    const QString wavDir = args.size() >= 3 && args.at(1) == QLatin1String("--wav") ? args.at(2) : QString();

    // --- JSC dictionary and message layer -------------------------------
    check(Js8Message::dictionaryAvailable(), "JSC tables load");
    {
        const QString text = QStringLiteral("HELLO JOHN, THE WEATHER HERE IS SUNNY. 73");
        const auto frames = Js8Message::buildFrames("DO3BOX", "JO31", QString(), text, Js8Mode::Submode::Normal, false);
        check(frames.size() >= 2, "free text needs several frames");
        check(frames.first().bits & Js8Message::First, "first frame flagged");
        check(frames.last().bits & Js8Message::Last, "last frame flagged");
        const QString shown = Js8Message::preview(frames, Js8Mode::Submode::Normal);
        check(shown.contains(QStringLiteral("THE WEATHER HERE IS SUNNY")), "free text round trip: " + shown.toStdString());
    }
    {
        const auto frames = Js8Message::buildFrames("DO3BOX", "JO31", "DL1ABC", "SNR?", Js8Mode::Submode::Normal);
        check(frames.size() == 1, "directed query is one frame");
        const auto u = Js8Message::unpack(frames.first().frame, frames.first().bits, Js8Mode::Submode::Normal);
        check(u.directed && u.from == "DO3BOX" && u.to == "DL1ABC" && u.command.contains("SNR?"),
              "directed unpack: " + u.text.toStdString());
    }
    {
        const auto frames = Js8Message::buildFrames("DO3BOX", "JO31", QString(), "DO3BOX: " + Js8Message::heartbeatText("JO31"),
                                                    Js8Mode::Submode::Normal);
        check(frames.size() == 1, "heartbeat is one frame");
        const auto u = Js8Message::unpack(frames.first().frame, frames.first().bits, Js8Mode::Submode::Normal);
        check(u.heartbeat && !u.cq && u.from == "DO3BOX" && u.grid == "JO31", "heartbeat unpack: " + u.text.toStdString());
        const auto cq = Js8Message::buildFrames("DO3BOX", "JO31", QString(), Js8Message::cqText("JO31"), Js8Mode::Submode::Normal);
        check(cq.size() == 1, "CQ is one frame");
        const auto c = Js8Message::unpack(cq.first().frame, cq.first().bits, Js8Mode::Submode::Normal);
        check(c.heartbeat && c.cq && c.text.contains("CQ CQ CQ"), "CQ unpack: " + c.text.toStdString());
    }
    {
        // Fast submodes put free text in data-flagged frames.
        const auto frames = Js8Message::buildFrames("DO3BOX", "JO31", QString(), "TESTING FAST DATA FRAMES",
                                                    Js8Mode::Submode::Fast, false);
        check(!frames.isEmpty() && (frames.last().bits & Js8Message::Data), "fast data flag");
        check(Js8Message::preview(frames, Js8Mode::Submode::Fast).contains("TESTING FAST DATA FRAMES"), "fast data round trip");
    }

    // --- LDPC/CRC -----------------------------------------------------------
    {
        Js8Ldpc::Message m{};
        check(Js8Ldpc::packFrame("Ab0-+zZ19xyQ", 3, &m) && Js8Ldpc::crcOk(m), "pack/crc");
        int bits = 0;
        check(Js8Ldpc::unpackFrame(m, &bits) == "Ab0-+zZ19xyQ" && bits == 3, "unpack frame");
        const Js8Ldpc::Codeword cw = Js8Ldpc::encode(m);
        Js8Ldpc::Llr llr{};
        std::mt19937 rng(7);
        std::normal_distribution<float> n(0.0f, 0.9f);
        for (int i = 0; i < Js8Ldpc::N; ++i) llr[size_t(i)] = (cw[size_t(i)] ? 2.0f : -2.0f) + n(rng);
        Js8Ldpc::Message out{};
        check(Js8Ldpc::bpDecode(llr, 30, &out, nullptr) >= 0 && out == m, "BP decodes noisy codeword");
        // OSD: weak, partly wrong soft bits.
        for (int i = 0; i < Js8Ldpc::N; ++i) llr[size_t(i)] = (cw[size_t(i)] ? 1.0f : -1.0f) + 0.35f * n(rng);
        for (int i = 0; i < 12; ++i) llr[size_t(i * 13)] = cw[size_t(i * 13)] ? -0.3f : 0.3f;
        Js8Ldpc::Message osd{};
        Js8Ldpc::Codeword ocw{};
        float dmin = 0;
        const int hard = Js8Ldpc::osdDecode(llr, 4, &osd, &ocw, &dmin);
        check(Js8Ldpc::encode(osd) == ocw, "OSD returns a valid codeword");
        check(hard >= 0 && osd == m, "OSD recovers the message");
        m[5] ^= 1;
        check(!Js8Ldpc::crcOk(m), "CRC detects an error");
    }

    // --- Transmitter -> receiver loopback for every submode --------------
    int index = 0;
    for (Js8Mode::Submode sub : Js8Mode::allSubmodes()) {
        const auto p = Js8Mode::parameters(sub);
        const auto frames = Js8Message::buildFrames("DO3BOX", "JO31", "DL1ABC", "HW CPY?", sub);
        check(frames.size() == 1, "one directed frame");
        QVector<float> x = waveform(frames, sub, 1234.0);
        x.resize(p.periodSeconds * 12000);
        // A second station 300 Hz higher, starting 0.4 s late.
        const auto other = Js8Message::buildFrames("DL1ABC", "JN58", QString(), Js8Message::cqText("JN58"), sub);
        QVector<float> y = waveform(other, sub, 1534.0);
        const int lag = int(0.4 * 12000);
        for (int i = 0; i + lag < x.size() && i < y.size(); ++i) x[i + lag] += 0.7f * y[i];   // about 3 dB weaker
        const double rms = 0.95 / std::sqrt(2.0);
        addNoise(x, rms, -12.0, 100 + unsigned(index));
        if (!wavDir.isEmpty()) writeWav(QDir(wavDir).filePath(QStringLiteral("js8_%1.wav").arg(QChar(p.letter))), x);

        QElapsedTimer timer;
        timer.start();
        const auto decodes = decodeAll(x.mid(0, p.nmax()), sub);
        const qint64 ms = timer.elapsed();
        bool foundDirected = false, foundCq = false;
        for (const Js8Decode &d : decodes) {
            if (d.frame == frames.first().frame && d.bits == frames.first().bits) {
                foundDirected = true;
                check(std::fabs(d.frequencyHz - 1234.0) < 2.0, "frequency estimate");
                check(std::fabs(d.dtSeconds) < 0.15, "dt estimate");
                check(d.snrDb > -20 && d.snrDb < -4, "snr estimate " + std::to_string(d.snrDb));
            }
            if (d.cq && d.from == "DL1ABC") {
                foundCq = true;
                check(std::fabs(d.dtSeconds - 0.4) < 0.15, "late signal dt");
            }
        }
        std::cout << "JS8 " << Js8Mode::submodeName(sub).toStdString() << ": " << decodes.size()
                  << " decode(s) in " << ms << " ms" << std::endl;
        for (const Js8Decode &d : decodes)
            std::cout << "  " << d.snrDb << " dB dt " << d.dtSeconds << " " << d.frequencyHz << " Hz "
                      << d.frame.toStdString() << " " << d.text.toStdString() << std::endl;
        check(foundDirected, "loopback directed frame, submode " + Js8Mode::submodeName(sub).toStdString());
        check(foundCq, "loopback CQ frame, submode " + Js8Mode::submodeName(sub).toStdString());
        ++index;
    }

    // Weak signal near the JS8 Normal threshold (JS8Call 2.2.0 decodes 1/10 at -20 dB).
    {
        const auto frames = Js8Message::buildFrames("DO3BOX", "JO31", QString(), "DO3BOX: " + Js8Message::heartbeatText("JO31"),
                                                    Js8Mode::Submode::Normal);
        QVector<float> x = waveform(frames, Js8Mode::Submode::Normal, 1000.0);
        x.resize(15 * 12000);
        addNoise(x, 0.95 / std::sqrt(2.0), -19.0, 4242);
        const auto decodes = decodeAll(x, Js8Mode::Submode::Normal);
        bool found = false;
        for (const Js8Decode &d : decodes) found = found || d.frame == frames.first().frame;
        check(found, "weak heartbeat at -19 dB");
    }

    // Multi-frame transmission: each frame lands in its own period.
    {
        const auto frames = Js8Message::buildFrames("DO3BOX", "JO31", QString(), "CAN YOU COPY THIS LONGER TEXT MESSAGE",
                                                    Js8Mode::Submode::Turbo, false);
        check(frames.size() >= 2, "long text spans frames");
        Js8Transmitter tx(frames, Js8Mode::Submode::Turbo, 12000, 1500.0);
        QVector<float> all(int(tx.durationSeconds() * 12000) + 12000, 0.0f);
        all.resize(tx.generate(all.data(), all.size()));
        QString text;
        for (int f = 0; f < frames.size(); ++f) {
            QVector<float> period = all.mid(f * 6 * 12000, 6 * 12000);
            period.resize(6 * 12000);
            const auto decodes = decodeAll(period, Js8Mode::Submode::Turbo);
            check(decodes.size() == 1 && decodes.first().frame == frames.at(f).frame, "frame in its period");
            text += decodes.first().text;
        }
        check(text.contains("CAN YOU COPY THIS LONGER TEXT MESSAGE"), "assembled text: " + text.toStdString());
    }

    // UTC alignment of the first audio pull.
    {
        const auto frames = Js8Message::buildFrames("DO3BOX", "JO31", QString(), Js8Message::cqText("JO31"), Js8Mode::Submode::Normal);
        Js8Transmitter late(frames, Js8Mode::Submode::Normal, 12000, 1500.0);
        check(late.prepareForPlayback(15000 * 1000LL + 200), "late pull accepted");
        QVector<float> a(12000, 1.0f);
        late.generate(a.data(), a.size());
        // 200 ms of the 500 ms start delay were skipped: the tone starts at 300 ms.
        check(std::fabs(a[3500]) > 0.0f || std::fabs(a[3700]) > 0.0f, "late pull: tone after 300 ms");
        check(a[3000] == 0.0f, "late pull: silence before 300 ms");
        Js8Transmitter early(frames, Js8Mode::Submode::Normal, 12000, 1500.0);
        check(early.prepareForPlayback(15000 * 1000LL - 100), "early pull accepted");
        QVector<float> b(12000, 1.0f);
        early.generate(b.data(), b.size());
        check(b[7100] == 0.0f && (std::fabs(b[7300]) > 0.0f || std::fabs(b[7400]) > 0.0f), "early pull waits 100 ms");
    }

    std::cout << "PASS js8 dictionary/messages/ldpc/loopback(A,B,C,E)/weak/multi-frame\n";
    return 0;
}
