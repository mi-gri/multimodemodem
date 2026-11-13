// SPDX-License-Identifier: GPL-3.0-or-later
//
// WSPR-2 decoder for m³: candidate search, sync/frequency/drift refinement,
// noncoherent block detection, Fano decoding and signal subtraction over
// three passes.  Port of WSJT-X lib/wsprd/wsprd.c (Joe Taylor K1JT, Steven
// Franke K9AN, GNU GPL v3) without file I/O or global state; FFTW plans are
// created under the m³ planner lock.
#pragma once

#include "../../third_party/wsjtx_gpl/wspr/WsprCodec.h"

#include <string>
#include <vector>

namespace M3Wspr {

struct DecodeOptions
{
    int passes = 3;                 // wsprd default; 2 with -B, 1 with -s
    bool subtraction = true;
    bool quickMode = false;         // -q: no shift jittering
    bool moreCandidates = false;    // -d
    bool wideSearch = false;        // -w: +-150 Hz instead of +-110 Hz
    unsigned int maxCycles = 10000; // -C: Fano timeout per bit
};

struct Decode
{
    double audioHz = 0.0;     // 1500 + frequency offset
    float snrDb = 0.0f;       // in 2500 Hz
    float dtSeconds = 0.0f;
    float drift = 0.0f;       // Hz over the transmission
    float sync = 0.0f;
    unsigned int cycles = 0;
    int jitter = 0;
    int pass = 0;
    Message message;
};

class Engine
{
public:
    Engine();
    ~Engine();
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    static constexpr int kSampleRate = 12000;
    static constexpr int kInputSamples = 114 * kSampleRate;  // wsprd reads 114 s

    // @p samples: 12 kHz audio from the start of the even UTC minute (missing
    // samples at the end are zero).  Decodes are sorted by frequency.
    std::vector<Decode> decode(const float *samples, size_t count, const DecodeOptions &options = {});

    HashTable &hashes() { return m_hashes; }

private:
    bool downconvert(const float *samples, size_t count);

    struct Plans;
    Plans *m_plans = nullptr;
    std::vector<float> m_idat;
    std::vector<float> m_qdat;
    HashTable m_hashes;
    MetricTable m_mettab;
};

} // namespace M3Wspr
