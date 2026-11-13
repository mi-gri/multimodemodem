// SPDX-License-Identifier: GPL-3.0-or-later
// WSPR-2 engine regression: message unpacking, channel symbols and decoding
// of synthesized signals in white noise.
#include "modems/wspr/WsprDecoder.h"
#include "modems/wspr/WsprEngine.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QElapsedTimer>

#include <cmath>
#include <cstdio>
#include <random>
#include <vector>

namespace {
int failures = 0;
void check(bool ok, const char *what)
{
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) ++failures;
}

constexpr double kPi = 3.14159265358979323846;

// One WSPR-2 transmission: 162 symbols of 8192 samples at 12 kHz starting
// 1 s after the even minute, amplitude for @p snrDb in 2500 Hz with unit
// noise variance over 6 kHz.
void addSignal(std::vector<float> &audio, const unsigned char *data, double audioHz, double snrDb)
{
    unsigned char symbols[M3Wspr::kSymbols];
    M3Wspr::channelSymbolsForData(data, symbols);
    const double amplitude = std::sqrt(2.0 * std::pow(10.0, snrDb / 10.0) * 2500.0 / 6000.0);
    const double toneSpacing = 12000.0 / 8192.0;
    double phase = 0.0;
    size_t k = 12000;
    for (int s = 0; s < M3Wspr::kSymbols; ++s) {
        const double f = audioHz + (symbols[s] - 1.5) * toneSpacing;
        for (int j = 0; j < 8192 && k < audio.size(); ++j, ++k) {
            audio[k] += static_cast<float>(amplitude * std::sin(phase));
            phase += 2.0 * kPi * f / 12000.0;
        }
    }
}
} // namespace

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    // K1ABC FN42 37 and DO3BOX JO30 23 packed as by wsprsim.
    const unsigned char k1abc[M3Wspr::kDataBytes] = {0xf7, 0x0c, 0x23, 0x8b, 0x0d, 0x19, 0x40, 0, 0, 0, 0};
    const unsigned char do3box[M3Wspr::kDataBytes] = {0x5c, 0x69, 0x5b, 0xb7, 0xa0, 0x95, 0xc0, 0, 0, 0, 0};

    M3Wspr::HashTable hashes;
    M3Wspr::Message message;
    check(M3Wspr::unpack(k1abc, hashes, message) && message.text == "K1ABC FN42 37" && message.type == 1 &&
              message.plausible && message.powerDbm == 37,
          "unpack type 1 K1ABC FN42 37");
    check(hashes.callAt(M3Wspr::HashTable::hashOf("K1ABC")) == "K1ABC", "type 1 call enters the hash table");

    // Encode -> soft symbols -> deinterleave -> Fano returns the data bytes.
    unsigned char channel[M3Wspr::kSymbols];
    M3Wspr::channelSymbolsForData(do3box, channel);
    unsigned char soft[M3Wspr::kSymbols];
    for (int i = 0; i < M3Wspr::kSymbols; ++i) soft[i] = channel[i] >= 2 ? 228 : 28;
    M3Wspr::deinterleave(soft);
    unsigned char decoded[M3Wspr::kDataBytes] = {};
    unsigned int metric = 0, cycles = 0;
    const bool fanoOk = M3Wspr::fano(soft, M3Wspr::makeMetricTable(), 60, 10000, decoded, &metric, &cycles);
    bool same = fanoOk;
    for (int i = 0; i < 7; ++i) same = same && decoded[i] == do3box[i];
    check(same, "Fano decodes clean channel symbols");

    std::mt19937 rng(20261113u);
    std::normal_distribution<float> gauss(0.0f, 1.0f);
    std::vector<float> audio(120 * 12000);
    for (float &v : audio) v = gauss(rng) * 0.05f;
    // Noise above is scaled by 0.05; scale signals the same way.
    std::vector<float> signal(audio.size(), 0.0f);
    addSignal(signal, k1abc, 1540.0, -20.0);
    addSignal(signal, do3box, 1440.0, -26.0);
    for (size_t i = 0; i < audio.size(); ++i) audio[i] += 0.05f * signal[i];

    M3Wspr::Engine engine;
    const auto results = engine.decode(audio.data(), audio.size());
    bool foundK1 = false, foundDo3 = false;
    for (const auto &d : results) {
        std::printf("  %5.1f dB  DT %4.1f  %8.2f Hz  drift %d  %s\n", d.snrDb, d.dtSeconds, d.audioHz,
                    static_cast<int>(d.drift), d.message.text.c_str());
        if (d.message.text == "K1ABC FN42 37" && std::fabs(d.audioHz - 1540.0) < 1.0 &&
            std::fabs(d.dtSeconds) < 0.3 && std::fabs(d.snrDb + 20.0) < 3.0)
            foundK1 = true;
        if (d.message.text == "DO3BOX JO30 23" && std::fabs(d.audioHz - 1440.0) < 1.0 &&
            std::fabs(d.dtSeconds) < 0.3 && std::fabs(d.snrDb + 26.0) < 3.0)
            foundDo3 = true;
    }
    check(foundK1, "decodes K1ABC FN42 37 at -20 dB, 1540 Hz, DT 0");
    check(foundDo3, "decodes DO3BOX JO30 23 at -26 dB, 1440 Hz, DT 0");
    check(results.size() == 2, "no extra decodes");

    // A second decode of the same record must give the same result (state
    // of the previous decode, e.g. buffer tails, must not leak).
    const auto again = engine.decode(audio.data(), audio.size());
    bool repeatSame = again.size() == results.size();
    for (size_t i = 0; repeatSame && i < again.size(); ++i) repeatSame = again[i].message.text == results[i].message.text;
    check(repeatSame, "second decode of the same record gives the same decodes");

    std::vector<float> noise(120 * 12000);
    for (float &v : noise) v = gauss(rng) * 0.05f;
    check(engine.decode(noise.data(), noise.size()).empty(), "noise only: no decodes");
    // Live path: 48 kHz blocks with UTC time stamps from an even minute,
    // resampled, aligned to the period and decoded on the decoder thread.
    {
        const qint64 periodStartMs = (QDateTime::currentMSecsSinceEpoch() / 120000 - 2) * 120000;
        std::vector<float> live(static_cast<size_t>(120 * 48000));
        std::normal_distribution<float> n48(0.0f, 0.05f);
        for (float &v : live) v = n48(rng);
        unsigned char sym[M3Wspr::kSymbols];
        M3Wspr::channelSymbolsForData(k1abc, sym);
        const double amp = 0.05 * std::sqrt(2.0 * std::pow(10.0, -18.0 / 10.0) * 2500.0 / 24000.0);
        double ph = 0.0;
        size_t k = 48000;
        for (int sIdx = 0; sIdx < M3Wspr::kSymbols; ++sIdx)
            for (int j = 0; j < 8192 * 4 && k < live.size(); ++j, ++k) {
                live[k] += static_cast<float>(amp * std::sin(ph));
                ph += 2.0 * kPi * (1480.0 + (sym[sIdx] - 1.5) * 12000.0 / 8192.0) / 48000.0;
            }
        WsprDecoder decoder;
        decoder.setLiveInputEnabled(true);
        QVector<WsprSpot> got;
        QDateTime gotPeriod;
        QObject::connect(&decoder, &WsprDecoder::decoded, [&](const QVector<WsprSpot> &spots, const QDateTime &p, int) {
            got = spots;
            gotPeriod = p;
        });
        decoder.reset();
        for (size_t pos = 0; pos < live.size(); pos += 1024) {
            AudioBlock block;
            block.sampleRate = 48000;
            block.captureGeneration = 1;
            block.firstSampleUtcNs = periodStartMs * 1000000LL + static_cast<qint64>(pos) * 1000000000LL / 48000;
            block.samples = QVector<float>(live.begin() + static_cast<long>(pos),
                                           live.begin() + static_cast<long>(std::min(live.size(), pos + 1024)));
            decoder.processAudioBlock(block);
        }
        QElapsedTimer wait;
        wait.start();
        while (gotPeriod.isNull() && wait.elapsed() < 20000) QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
        bool liveOk = gotPeriod.toMSecsSinceEpoch() == periodStartMs && got.size() == 1 &&
                      got.first().message == QStringLiteral("K1ABC FN42 37") && std::fabs(got.first().audioHz - 1480.0) < 1.5 &&
                      std::fabs(got.first().dtSeconds) < 0.5;
        if (!got.isEmpty())
            std::printf("  live: %s %.1f Hz DT %.1f %d dB\n", qPrintable(got.first().message), got.first().audioHz,
                        got.first().dtSeconds, got.first().snrDb);
        check(liveOk, "live 48 kHz blocks: period aligned and decoded on the decoder thread");
    }
    return failures == 0 ? 0 : 1;
}
