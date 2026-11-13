// SPDX-License-Identifier: GPL-3.0-or-later
// WSPR-2 engine regression: message unpacking, channel symbols and decoding
// of synthesized signals in white noise.
#include "modems/wspr/WsprEngine.h"

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

int main()
{
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

    std::vector<float> noise(120 * 12000);
    for (float &v : noise) v = gauss(rng) * 0.05f;
    check(engine.decode(noise.data(), noise.size()).empty(), "noise only: no decodes");
    return failures == 0 ? 0 : 1;
}
