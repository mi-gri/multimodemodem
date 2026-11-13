// SPDX-License-Identifier: GPL-3.0-or-later
//
// Port of WSJT-X lib/wsprd/wsprd.c (Copyright 2001-2018 Joe Taylor K1JT,
// 2014-2018 Steven Franke K9AN, GNU GPL v3).  The signal processing follows
// wsprd step by step; differences:
//  - input is a sample buffer instead of a WAV file, FFTs in double precision;
//  - the decoded data bytes are re-encoded for subtraction (wsprd re-packs
//    the printed message, which loses an unknown <...> hash);
//  - no OSD stage yet (wsprd -o).
#include "WsprEngine.h"

#include "../../dsp/FftwPlannerLock.h"

#include <fftw3.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <mutex>

namespace M3Wspr {

namespace {

constexpr int kNfft2 = 46080;              // downsampled points (375 Hz)
constexpr int kNfft1 = kNfft2 * 32;        // input FFT at 12 kHz
// wsprd keeps the 375 Hz record in 65536-point zeroed buffers: the last
// 2-symbol FFTs read up to 46335, past the 46080 samples.
constexpr int kDataBuffer = 65536;
constexpr float kPi = 3.14159265358979323846f;
constexpr float kDt = 1.0f / 375.0f;
constexpr float kDf = 375.0f / 256.0f;     // tone spacing
constexpr int kSymFac = 50;
constexpr float kMinRms = 52.0f * (kSymFac / 64.0f);
constexpr int kFanoDelta = 60;
constexpr int kIifac = 8;
constexpr float kMinSync1 = 0.10f;

struct Candidate {
    float freq = 0.0f;
    float snr = 0.0f;
    int shift = 0;
    float drift = 0.0f;
    float sync = 0.0f;
};

const auto &sync = kSyncVector;

// Precomputed rotators of the four tones over one symbol (256 samples, plus
// the phase after the full symbol at index 256).
struct ToneRotators {
    float c[4][257];
    float s[4][257];
    void compute(float fp)
    {
        const float twopidt = 2.0f * kPi * kDt;
        const float offsets[4] = {-1.5f * kDf, -0.5f * kDf, 0.5f * kDf, 1.5f * kDf};
        for (int t = 0; t < 4; ++t) {
            const float dphi = twopidt * (fp + offsets[t]);
            const float cd = std::cos(dphi);
            const float sd = std::sin(dphi);
            c[t][0] = 1.0f;
            s[t][0] = 0.0f;
            for (int j = 1; j < 257; ++j) {
                c[t][j] = c[t][j - 1] * cd - s[t][j - 1] * sd;
                s[t][j] = c[t][j - 1] * sd + s[t][j - 1] * cd;
            }
        }
    }
};

// Correlate the 4 tones of symbol i at lag: is/qs[tone].
inline void symbolTones(const float *id, const float *qd, long np, int lag, int i,
                        const ToneRotators &r, float is[4], float qs[4])
{
    for (int t = 0; t < 4; ++t) { is[t] = 0.0f; qs[t] = 0.0f; }
    const int k0 = lag + i * 256;
    const int jStart = std::max(0, 1 - k0);
    const int jEnd = std::min(256, static_cast<int>(np) - k0);
    for (int j = jStart; j < jEnd; ++j) {
        const float x = id[k0 + j];
        const float y = qd[k0 + j];
        for (int t = 0; t < 4; ++t) {
            is[t] += x * r.c[t][j] + y * r.s[t][j];
            qs[t] += -x * r.s[t][j] + y * r.c[t][j];
        }
    }
}

// wsprd sync_and_demodulate, modes 0 (lag search) and 1 (frequency search).
void syncAndDemodulate(const float *id, const float *qd, long np, float &f1,
                       int ifmin, int ifmax, float fstep, int &shift1, int lagmin, int lagmax,
                       int lagstep, float drift1, float &syncOut, int mode)
{
    float syncmax = -1e30f;
    int bestShift = 0;
    float fbest = 0.0f;
    if (mode == 0) { ifmin = 0; ifmax = 0; fstep = 0.0f; }
    if (mode == 1) { lagmin = shift1; lagmax = shift1; }
    ToneRotators r;
    for (int ifreq = ifmin; ifreq <= ifmax; ++ifreq) {
        const float f0 = f1 + ifreq * fstep;
        for (int lag = lagmin; lag <= lagmax; lag += lagstep) {
            float ss = 0.0f;
            float totp = 0.0f;
            float fplast = -10000.0f;
            for (int i = 0; i < kSymbols; ++i) {
                const float fp = f0 + (drift1 / 2.0f) * (static_cast<float>(i) - 81.0f) / 81.0f;
                if (i == 0 || fp != fplast) {
                    r.compute(fp);
                    fplast = fp;
                }
                float is[4], qs[4];
                symbolTones(id, qd, np, lag, i, r, is, qs);
                const float p0 = std::sqrt(is[0] * is[0] + qs[0] * qs[0]);
                const float p1 = std::sqrt(is[1] * is[1] + qs[1] * qs[1]);
                const float p2 = std::sqrt(is[2] * is[2] + qs[2] * qs[2]);
                const float p3 = std::sqrt(is[3] * is[3] + qs[3] * qs[3]);
                totp += p0 + p1 + p2 + p3;
                const float cmet = (p1 + p3) - (p0 + p2);
                ss = sync[static_cast<size_t>(i)] == 1 ? ss + cmet : ss - cmet;
            }
            ss = totp > 0.0f ? ss / totp : 0.0f;
            if (ss > syncmax) {
                syncmax = ss;
                bestShift = lag;
                fbest = f0;
            }
        }
    }
    syncOut = syncmax;
    shift1 = bestShift;
    f1 = fbest;
}

// wsprd noncoherent_sequence_detection: soft symbols (0..255) from blocks of
// nblock symbols estimated together.
void noncoherentSequenceDetection(const float *id, const float *qd, long np, unsigned char *symbols,
                                  float f0, int lag, float drift1, int nblock, bool bitByBit)
{
    float is[4][kSymbols], qs[4][kSymbols], cf[4][kSymbols], sf[4][kSymbols];
    float fsymb[kSymbols];
    ToneRotators r;
    float fplast = -10000.0f;
    for (int i = 0; i < kSymbols; ++i) {
        const float fp = f0 + (drift1 / 2.0f) * (static_cast<float>(i) - 81.0f) / 81.0f;
        if (i == 0 || fp != fplast) {
            r.compute(fp);
            fplast = fp;
        }
        float a[4], b[4];
        symbolTones(id, qd, np, lag, i, r, a, b);
        for (int t = 0; t < 4; ++t) {
            cf[t][i] = r.c[t][256];
            sf[t][i] = r.s[t][256];
            is[t][i] = a[t];
            qs[t][i] = b[t];
        }
    }
    const int nseq = 1 << nblock;
    float p[512];
    for (int i = 0; i < kSymbols; i += nblock) {
        for (int j = 0; j < nseq; ++j) {
            float xi = 0.0f, xq = 0.0f, cm = 1.0f, sm = 0.0f;
            for (int ib = 0; ib < nblock; ++ib) {
                const int bit = (j & (1 << (nblock - 1 - ib))) >> (nblock - 1 - ib);
                const int itone = sync[static_cast<size_t>(i + ib)] + 2 * bit;
                xi += is[itone][i + ib] * cm + qs[itone][i + ib] * sm;
                xq += qs[itone][i + ib] * cm - is[itone][i + ib] * sm;
                const float cmp = cf[itone][i + ib] * cm - sf[itone][i + ib] * sm;
                const float smp = sf[itone][i + ib] * cm + cf[itone][i + ib] * sm;
                cm = cmp;
                sm = smp;
            }
            p[j] = std::sqrt(xi * xi + xq * xq);
        }
        for (int ib = 0; ib < nblock; ++ib) {
            const int imask = 1 << (nblock - 1 - ib);
            float xm1 = 0.0f, xm0 = 0.0f;
            for (int j = 0; j < nseq; ++j) {
                if ((j & imask) != 0) { if (p[j] > xm1) xm1 = p[j]; }
                else if (p[j] > xm0) xm0 = p[j];
            }
            fsymb[i + ib] = xm1 - xm0;
            if (bitByBit) {
                const float m = xm1 > xm0 ? xm1 : xm0;
                fsymb[i + ib] = m > 0.0f ? fsymb[i + ib] / m : 0.0f;
            }
        }
    }
    float fsum = 0.0f, f2sum = 0.0f;
    for (int i = 0; i < kSymbols; ++i) {
        fsum += fsymb[i] / 162.0f;
        f2sum += fsymb[i] * fsymb[i] / 162.0f;
    }
    const float fac = std::sqrt(std::max(0.0f, f2sum - fsum * fsum));
    for (int i = 0; i < kSymbols; ++i) {
        float v = fac > 0.0f ? kSymFac * fsymb[i] / fac : 0.0f;
        v = std::min(127.0f, std::max(-128.0f, v));
        symbols[i] = static_cast<unsigned char>(v + 128.0f);
    }
}

// wsprd subtract_signal2: estimate the complex amplitude of the decoded
// signal with a low-pass filter and subtract it.
void subtractSignal(float *id, float *qd, long np, float f0, int shift0, float drift0,
                    const unsigned char *channelSymbols)
{
    constexpr int nsym = kSymbols, nspersym = 256, nfilt = 360;
    constexpr int nsig = nsym * nspersym;
    constexpr int nc2 = 45000;
    const float twopidt = 2.0f * kPi * kDt;
    std::vector<float> refi(nc2, 0.0f), refq(nc2, 0.0f), ci(nc2, 0.0f), cq(nc2, 0.0f),
        cfi(nc2, 0.0f), cfq(nc2, 0.0f);
    float phi = 0.0f;
    for (int i = 0; i < nsym; ++i) {
        const float cs = static_cast<float>(channelSymbols[i]);
        const float dphi = twopidt * (f0 + (drift0 / 2.0f) * (static_cast<float>(i) - nsym / 2.0f) / (nsym / 2.0f) +
                                      (cs - 1.5f) * kDf);
        for (int j = 0; j < nspersym; ++j) {
            const int ii = nspersym * i + j;
            refi[ii] = std::cos(phi);
            refq[ii] = std::sin(phi);
            phi += dphi;
        }
    }
    float w[nfilt], partialsum[nfilt];
    float norm = 0.0f;
    for (int i = 0; i < nfilt; ++i) {
        partialsum[i] = 0.0f;
        w[i] = std::sin(kPi * static_cast<float>(i) / static_cast<float>(nfilt - 1));
        norm += w[i];
    }
    for (int i = 0; i < nfilt; ++i) w[i] /= norm;
    for (int i = 1; i < nfilt; ++i) partialsum[i] = partialsum[i - 1] + w[i];

    for (int i = 0; i < nsig; ++i) {
        const int k = shift0 + i;
        if (k > 0 && k < np) {
            ci[i + nfilt] = id[k] * refi[i] + qd[k] * refq[i];
            cq[i + nfilt] = qd[k] * refi[i] - id[k] * refq[i];
        }
    }
    for (int i = nfilt / 2; i < nc2 - nfilt / 2; ++i) {
        float a = 0.0f, b = 0.0f;
        for (int j = 0; j < nfilt; ++j) {
            a += w[j] * ci[i - nfilt / 2 + j];
            b += w[j] * cq[i - nfilt / 2 + j];
        }
        cfi[i] = a;
        cfq[i] = b;
    }
    for (int i = 0; i < nsig; ++i) {
        if (i < nfilt / 2) norm = partialsum[nfilt / 2 + i];
        else if (i > nsig - 1 - nfilt / 2) norm = partialsum[nfilt / 2 + nsig - 1 - i];
        else norm = 1.0f;
        const int k = shift0 + i;
        const int j = i + nfilt;
        if (k > 0 && k < np && norm > 0.0f) {
            id[k] -= (cfi[j] * refi[i] - cfq[j] * refq[i]) / norm;
            qd[k] -= (cfi[j] * refq[i] + cfq[j] * refi[i]) / norm;
        }
    }
}

unsigned int countHardErrors(const unsigned char *symbols, const unsigned char *channelSymbols)
{
    unsigned char cw[kSymbols];
    for (int i = 0; i < kSymbols; ++i) cw[i] = channelSymbols[i] >= 2 ? 1 : 0;
    deinterleave(cw);
    unsigned int n = 0;
    for (int i = 0; i < kSymbols; ++i) n += ((symbols[i] > 127 ? 1 : 0) == cw[i]) ? 0 : 1;
    return n;
}

} // namespace

struct Engine::Plans {
    double *realIn = nullptr;
    fftw_complex *spectrum = nullptr;     // nfft1/2+1
    fftw_complex *narrowIn = nullptr;     // nfft2
    fftw_complex *narrowOut = nullptr;    // nfft2
    fftw_complex *symIn = nullptr;        // 512
    fftw_complex *symOut = nullptr;       // 512
    fftw_plan forward = nullptr;
    fftw_plan backward = nullptr;
    fftw_plan symbolFft = nullptr;
};

Engine::Engine()
    : m_plans(new Plans), m_idat(kDataBuffer, 0.0f), m_qdat(kDataBuffer, 0.0f), m_mettab(makeMetricTable())
{
    Plans &p = *m_plans;
    p.realIn = fftw_alloc_real(kNfft1);
    p.spectrum = fftw_alloc_complex(kNfft1 / 2 + 1);
    p.narrowIn = fftw_alloc_complex(kNfft2);
    p.narrowOut = fftw_alloc_complex(kNfft2);
    p.symIn = fftw_alloc_complex(512);
    p.symOut = fftw_alloc_complex(512);
    std::lock_guard<std::mutex> lock(M3Dsp::fftwPlannerMutex);
    p.forward = fftw_plan_dft_r2c_1d(kNfft1, p.realIn, p.spectrum, FFTW_ESTIMATE);
    p.backward = fftw_plan_dft_1d(kNfft2, p.narrowIn, p.narrowOut, FFTW_BACKWARD, FFTW_ESTIMATE);
    p.symbolFft = fftw_plan_dft_1d(512, p.symIn, p.symOut, FFTW_FORWARD, FFTW_ESTIMATE);
}

Engine::~Engine()
{
    if (m_plans == nullptr) return;
    Plans &p = *m_plans;
    {
        std::lock_guard<std::mutex> lock(M3Dsp::fftwPlannerMutex);
        if (p.forward) fftw_destroy_plan(p.forward);
        if (p.backward) fftw_destroy_plan(p.backward);
        if (p.symbolFft) fftw_destroy_plan(p.symbolFft);
    }
    fftw_free(p.realIn);
    fftw_free(p.spectrum);
    fftw_free(p.narrowIn);
    fftw_free(p.narrowOut);
    fftw_free(p.symIn);
    fftw_free(p.symOut);
    delete m_plans;
}

// wsprd readwavfile: mix 1500 Hz to 0 and decimate 12 kHz -> 375 Hz by an
// FFT of the whole record, keeping +-187.5 Hz around 1500 Hz.
bool Engine::downconvert(const float *samples, size_t count)
{
    Plans &p = *m_plans;
    if (!p.forward || !p.backward) return false;
    const size_t npoints = std::min<size_t>(count, kInputSamples);
    for (size_t i = 0; i < npoints; ++i) p.realIn[i] = samples[i];
    for (size_t i = npoints; i < static_cast<size_t>(kNfft1); ++i) p.realIn[i] = 0.0;
    fftw_execute(p.forward);
    const double df = 12000.0 / kNfft1;
    const int i0 = static_cast<int>(1500.0 / df + 0.5);
    const int nh2 = kNfft2 / 2;
    for (int i = 0; i < kNfft2; ++i) {
        int j = i0 + i;
        if (i > nh2) j -= kNfft2;
        p.narrowIn[i][0] = p.spectrum[j][0];
        p.narrowIn[i][1] = p.spectrum[j][1];
    }
    fftw_execute(p.backward);
    for (int i = 0; i < kNfft2; ++i) {
        m_idat[static_cast<size_t>(i)] = static_cast<float>(p.narrowOut[i][0] / 1000.0);
        m_qdat[static_cast<size_t>(i)] = static_cast<float>(p.narrowOut[i][1] / 1000.0);
    }
    std::fill(m_idat.begin() + kNfft2, m_idat.end(), 0.0f);
    std::fill(m_qdat.begin() + kNfft2, m_qdat.end(), 0.0f);
    return true;
}

std::vector<Decode> Engine::decode(const float *samples, size_t count, const DecodeOptions &options)
{
    std::vector<Decode> results;
    if (samples == nullptr || count == 0 || !downconvert(samples, count)) return results;
    Plans &plans = *m_plans;
    float *idat = m_idat.data();
    float *qdat = m_qdat.data();
    const long npoints = kNfft2;
    const float df = kDf / 2.0f;              // spectrum bin of the 2-symbol FFT
    const float fmin = options.wideSearch ? -150.0f : -110.0f;
    const float fmax = options.wideSearch ? 150.0f : 110.0f;
    const int npasses = std::max(1, std::min(3, options.subtraction ? options.passes : 1));
    const int nffts = 4 * (npoints / 512) - 1;

    std::vector<float> ps(static_cast<size_t>(512) * nffts);   // ps[j*nffts + i]
    auto PS = [&](int bin, int fftIndex) -> float & { return ps[static_cast<size_t>(bin) * nffts + fftIndex]; };
    float w[512];
    for (int i = 0; i < 512; ++i) w[i] = std::sin(0.006147931f * i);

    std::vector<std::string> uniqueCalls;
    std::vector<float> uniqueFreqs;
    int ndecodesPass = 0;

    for (int ipass = 0; ipass < npasses; ++ipass) {
        if (ipass == 1 && ndecodesPass == 0 && npasses > 2) ipass = 2;
        int nblocksize = 1, maxdrift = 4;
        float minsync2 = 0.12f;
        if (ipass == 2) {
            nblocksize = 4;   // block sizes 1..3 plus bit-by-bit normalisation
            maxdrift = 0;
            minsync2 = 0.10f;
        }
        ndecodesPass = 0;

        static_assert((4 * (kNfft2 / 512) - 2) * 128 + 511 < kDataBuffer, "2-symbol FFTs stay inside the buffer");
        for (int i = 0; i < nffts; ++i) {
            for (int j = 0; j < 512; ++j) {
                const int k = i * 128 + j;
                plans.symIn[j][0] = idat[k] * w[j];
                plans.symIn[j][1] = qdat[k] * w[j];
            }
            fftw_execute(plans.symbolFft);
            for (int j = 0; j < 512; ++j) {
                int k = j + 256;
                if (k > 511) k -= 512;
                PS(j, i) = static_cast<float>(plans.symOut[k][0] * plans.symOut[k][0] +
                                              plans.symOut[k][1] * plans.symOut[k][1]);
            }
        }

        float psavg[512] = {};
        for (int i = 0; i < nffts; ++i)
            for (int j = 0; j < 512; ++j) psavg[j] += PS(j, i);

        float smspec[411];
        for (int i = 0; i < 411; ++i) {
            smspec[i] = 0.0f;
            for (int j = -3; j <= 3; ++j) smspec[i] += psavg[256 - 205 + i + j];
        }
        float tmpsort[411];
        std::copy(smspec, smspec + 411, tmpsort);
        std::sort(tmpsort, tmpsort + 411);
        const float noiseLevel = tmpsort[122];
        const float minSnr = std::pow(10.0f, -8.0f / 10.0f);
        const float snrScaling = 26.3f;
        for (int j = 0; j < 411; ++j) {
            smspec[j] = noiseLevel > 0.0f ? smspec[j] / noiseLevel - 1.0f : 0.0f;
            if (smspec[j] < minSnr) smspec[j] = 0.1f * minSnr;
        }

        std::vector<Candidate> candidates;
        candidates.reserve(200);
        for (int j = 1; j < 410 && candidates.size() < 200; ++j) {
            if (smspec[j] > smspec[j - 1] && smspec[j] > smspec[j + 1]) {
                Candidate c;
                c.freq = (j - 205) * df;
                c.snr = 10.0f * std::log10(smspec[j]) - snrScaling;
                candidates.push_back(c);
            }
        }
        if (options.moreCandidates) {
            for (int j = 0; j < 411 && candidates.size() < 200; j += 3) {
                if (smspec[j] > minSnr) {
                    Candidate c;
                    c.freq = (j - 205) * df;
                    c.snr = 10.0f * std::log10(smspec[j]) - snrScaling;
                    candidates.push_back(c);
                }
            }
        }
        candidates.erase(std::remove_if(candidates.begin(), candidates.end(),
                                        [&](const Candidate &c) { return c.freq < fmin || c.freq > fmax; }),
                         candidates.end());
        // wsprd bubble sort on snr (stable, descending)
        std::stable_sort(candidates.begin(), candidates.end(),
                         [](const Candidate &a, const Candidate &b) { return a.snr > b.snr; });

        // Coarse estimates of shift (DT), frequency and drift.
        for (Candidate &c : candidates) {
            float smax = -1e30f;
            const int if0 = static_cast<int>(c.freq / df + 256);
            for (int ifr = if0 - 2; ifr <= if0 + 2; ++ifr) {
                for (int k0 = -10; k0 < 22; ++k0) {
                    for (int idrift = -maxdrift; idrift <= maxdrift; ++idrift) {
                        float ss = 0.0f, pow = 0.0f;
                        for (int k = 0; k < kSymbols; ++k) {
                            const int ifd = ifr + static_cast<int>((static_cast<float>(k) - 81.0f) / 81.0f *
                                                                   static_cast<float>(idrift) / (2.0f * df));
                            const int kindex = k0 + 2 * k;
                            if (kindex >= 0 && kindex < nffts && ifd >= 3 && ifd + 3 < 512) {
                                const float p0 = std::sqrt(PS(ifd - 3, kindex));
                                const float p1 = std::sqrt(PS(ifd - 1, kindex));
                                const float p2 = std::sqrt(PS(ifd + 1, kindex));
                                const float p3 = std::sqrt(PS(ifd + 3, kindex));
                                ss += (2 * sync[static_cast<size_t>(k)] - 1) * ((p1 + p3) - (p0 + p2));
                                pow += p0 + p1 + p2 + p3;
                            }
                        }
                        const float sync1 = pow > 0.0f ? ss / pow : 0.0f;
                        if (sync1 > smax) {
                            smax = sync1;
                            c.shift = 128 * (k0 + 1);
                            c.drift = static_cast<float>(idrift);
                            c.freq = (ifr - 256) * df;
                            c.sync = sync1;
                        }
                    }
                }
            }
        }

        // Refine frequency, shift and drift.
        for (Candidate &c : candidates) {
            float f1 = c.freq, drift1 = c.drift, sync1 = c.sync;
            int shift1 = c.shift;
            int lagmin = shift1 - 128, lagmax = shift1 + 128, lagstep = 64;
            syncAndDemodulate(idat, qdat, npoints, f1, 0, 0, 0.0f, shift1, lagmin, lagmax, lagstep, drift1, sync1, 0);
            syncAndDemodulate(idat, qdat, npoints, f1, -2, 2, 0.25f, shift1, lagmin, lagmax, lagstep, drift1, sync1, 1);
            if (ipass < 2) {
                float syncp = 0.0f, syncm = 0.0f;
                float fp = f1, fm = f1;
                int sp = shift1, sm = shift1;
                const float driftp = drift1 + 0.5f;
                syncAndDemodulate(idat, qdat, npoints, fp, 0, 0, 0.0f, sp, lagmin, lagmax, lagstep, driftp, syncp, 1);
                const float driftm = drift1 - 0.5f;
                syncAndDemodulate(idat, qdat, npoints, fm, 0, 0, 0.0f, sm, lagmin, lagmax, lagstep, driftm, syncm, 1);
                if (syncp > sync1) { drift1 = driftp; sync1 = syncp; }
                else if (syncm > sync1) { drift1 = driftm; sync1 = syncm; }
            }
            if (sync1 > kMinSync1) {
                lagmin = shift1 - 32; lagmax = shift1 + 32; lagstep = 16;
                syncAndDemodulate(idat, qdat, npoints, f1, 0, 0, 0.0f, shift1, lagmin, lagmax, lagstep, drift1, sync1, 0);
                syncAndDemodulate(idat, qdat, npoints, f1, -2, 2, 0.05f, shift1, lagmin, lagmax, lagstep, drift1, sync1, 1);
                c.freq = f1;
                c.shift = shift1;
                c.drift = drift1;
                c.sync = sync1;
            }
        }

        // Remove duplicates, keep candidates above minsync2.
        std::vector<Candidate> work;
        for (const Candidate &c : candidates) {
            bool dupe = false;
            for (Candidate &k : work) {
                if (std::fabs(c.freq - k.freq) < 0.05f && std::abs(c.shift - k.shift) < 16) {
                    if (c.sync > k.sync) k = c;
                    dupe = true;
                    break;
                }
            }
            if (!dupe && c.sync > minsync2) work.push_back(c);
        }

        for (const Candidate &c : work) {
            float f1 = c.freq;
            const int shift1 = c.shift;
            const float drift1 = c.drift;
            unsigned char symbols[kSymbols];
            unsigned char decdata[kDataBytes] = {};
            unsigned int metric = 0, cycles = 0;
            bool decoded = false;
            int jitter = 0;
            for (int ib = 1; ib <= nblocksize && !decoded; ++ib) {
                const int blocksize = ib < 4 ? ib : 1;
                const bool bitmetric = ib == 4;
                for (int idt = 0; !decoded && idt <= 128 / kIifac; ++idt) {
                    int ii = (idt + 1) / 2;
                    if (idt % 2 == 1) ii = -ii;
                    ii *= kIifac;
                    jitter = ii;
                    noncoherentSequenceDetection(idat, qdat, npoints, symbols, f1, shift1 + ii, drift1,
                                                 blocksize, bitmetric);
                    float sq = 0.0f;
                    for (int i = 0; i < kSymbols; ++i) {
                        const float y = static_cast<float>(symbols[i]) - 128.0f;
                        sq += y * y;
                    }
                    if (std::sqrt(sq / 162.0f) > kMinRms) {
                        deinterleave(symbols);
                        decoded = fano(symbols, m_mettab, kFanoDelta, options.maxCycles, decdata, &metric, &cycles);
                    }
                    if (options.quickMode) break;
                }
            }
            if (!decoded) continue;
            ++ndecodesPass;

            Message message;
            const bool unpacked = unpack(decdata, m_hashes, message);
            if (options.subtraction && unpacked && message.plausible) {
                unsigned char channel[kSymbols];
                channelSymbolsForData(decdata, channel);
                subtractSignal(idat, qdat, npoints, f1, shift1, drift1, channel);
                (void)countHardErrors(symbols, channel);
            }
            if (!unpacked || !message.plausible) continue;
            const std::string dedupeKey = message.text.substr(0, message.text.find(' '));
            bool dupe = false;
            for (size_t i = 0; i < uniqueCalls.size(); ++i)
                if (uniqueCalls[i] == dedupeKey && std::fabs(f1 - uniqueFreqs[i]) < 4.0f) dupe = true;
            if (dupe) continue;
            uniqueCalls.push_back(dedupeKey);
            uniqueFreqs.push_back(f1);
            Decode d;
            d.audioHz = 1500.0 + f1;
            d.snrDb = c.snr;
            d.dtSeconds = shift1 * kDt - 1.0f;
            d.drift = drift1;
            d.sync = c.sync;
            d.cycles = cycles;
            d.jitter = jitter;
            d.pass = ipass;
            d.message = message;
            results.push_back(d);
        }
    }
    std::stable_sort(results.begin(), results.end(),
                     [](const Decode &a, const Decode &b) { return a.audioHz < b.audioHz; });
    return results;
}

} // namespace M3Wspr
