// SPDX-License-Identifier: GPL-3.0-or-later
// C++ port of the JS8Call 2.2.0 receive chain; see Js8NativeEngine.h.
#include "Js8NativeEngine.h"

#include "Js8Ldpc.h"
#include "../../dsp/FftwPlannerLock.h"
#include "../../third_party/mshv_gpl/port/Hv_Lib_fftw/fftw3.h"

#include <QtGlobal>

#include <algorithm>
#include <cmath>
#include <complex>
#include <mutex>
#include <numeric>
#include <stdexcept>
#include <vector>

// M_PI is not defined by MinGW with -std=c++17 (no GNU extensions).
namespace { constexpr double kJs8Pi = 3.14159265358979323846; }

namespace {

using cd = std::complex<double>;
constexpr double kTwoPi = 6.283185307179586476925286766559;
constexpr int NN = Js8Mode::kSymbols;
constexpr int ND = Js8Mode::kDataSymbols;

double db(double x) { return x > 1.259e-10 ? 10.0 * std::log10(x) : -99.0; }

template <typename T>
std::vector<int> ascendingOrder(const std::vector<T> &v)
{
    std::vector<int> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::stable_sort(idx.begin(), idx.end(), [&](int a, int b) { return v[size_t(a)] < v[size_t(b)]; });
    return idx;
}

// Least-squares polynomial fit (polyfit.f90, mode 0) through the normal
// equations; nterms coefficients a[0..nterms-1].
std::vector<double> polyfit(const std::vector<double> &x, const std::vector<double> &y, int nterms)
{
    std::vector<double> a(static_cast<size_t>(nterms), 0.0);
    if (x.size() < size_t(nterms)) return a;
    const int nmax = 2 * nterms - 1;
    std::vector<double> sumx(static_cast<size_t>(nmax), 0.0), sumy(static_cast<size_t>(nterms), 0.0);
    for (size_t i = 0; i < x.size(); ++i) {
        double xt = 1.0;
        for (int n = 0; n < nmax; ++n) { sumx[size_t(n)] += xt; xt *= x[i]; }
        double yt = y[i];
        for (int n = 0; n < nterms; ++n) { sumy[size_t(n)] += yt; yt *= x[i]; }
    }
    std::vector<std::vector<double>> m(static_cast<size_t>(nterms), std::vector<double>(size_t(nterms + 1)));
    for (int j = 0; j < nterms; ++j) {
        for (int k = 0; k < nterms; ++k) m[size_t(j)][size_t(k)] = sumx[size_t(j + k)];
        m[size_t(j)][size_t(nterms)] = sumy[size_t(j)];
    }
    for (int c = 0; c < nterms; ++c) {
        int pivot = c;
        for (int r = c + 1; r < nterms; ++r)
            if (std::fabs(m[size_t(r)][size_t(c)]) > std::fabs(m[size_t(pivot)][size_t(c)])) pivot = r;
        if (std::fabs(m[size_t(pivot)][size_t(c)]) < 1e-300) return std::vector<double>(size_t(nterms), 0.0);
        std::swap(m[size_t(c)], m[size_t(pivot)]);
        for (int r = 0; r < nterms; ++r) {
            if (r == c) continue;
            const double f = m[size_t(r)][size_t(c)] / m[size_t(c)][size_t(c)];
            for (int k = c; k <= nterms; ++k) m[size_t(r)][size_t(k)] -= f * m[size_t(c)][size_t(k)];
        }
    }
    for (int c = 0; c < nterms; ++c) a[size_t(c)] = m[size_t(c)][size_t(nterms)] / m[size_t(c)][size_t(c)];
    return a;
}

struct FftwBuffer
{
    void *p = nullptr;
    explicit FftwBuffer(size_t bytes) : p(fftw_malloc(bytes)) { if (!p) throw std::bad_alloc(); }
    ~FftwBuffer() { fftw_free(p); }
    FftwBuffer(const FftwBuffer &) = delete;
    FftwBuffer &operator=(const FftwBuffer &) = delete;
};

class Plan
{
public:
    Plan() = default;
    ~Plan() { reset(); }
    Plan(const Plan &) = delete;
    Plan &operator=(const Plan &) = delete;
    void reset()
    {
        if (!m_plan) return;
        std::lock_guard<std::mutex> lock(M3Dsp::fftwPlannerMutex);
        fftw_destroy_plan(m_plan);
        m_plan = nullptr;
    }
    void r2c(int n, double *in, fftw_complex *out)
    {
        reset();
        std::lock_guard<std::mutex> lock(M3Dsp::fftwPlannerMutex);
        m_plan = fftw_plan_dft_r2c_1d(n, in, out, FFTW_ESTIMATE);
        if (!m_plan) throw std::runtime_error("JS8 FFT plan");
    }
    void c2c(int n, fftw_complex *data, int sign)
    {
        reset();
        std::lock_guard<std::mutex> lock(M3Dsp::fftwPlannerMutex);
        m_plan = fftw_plan_dft_1d(n, data, data, sign, FFTW_ESTIMATE);
        if (!m_plan) throw std::runtime_error("JS8 FFT plan");
    }
    void execute() const { fftw_execute(m_plan); }

private:
    fftw_plan m_plan = nullptr;
};

} // namespace

struct Js8NativeEngine::Impl
{
    Js8Mode::Parameters p{};
    bool ready = false;

    // Derived sizes (js8?_params.f90 names).
    int nfft1 = 0, nh1 = 0, nstep = 0, nhsym = 0, ndown = 0, nqsymbol = 0;
    int ndfft1 = 0, ndfft2 = 0, nmax = 0;
    double az = 0.0;
    Js8Mode::Costas costas{};

    std::unique_ptr<FftwBuffer> specIn, specOut, longIn, longOut, down, subBuf, filt;
    Plan specPlan, longPlan, downPlan, subForward, subBackward;
    std::vector<cd> filterSpectrum;     // subtraction low-pass, frequency domain
    std::vector<double> taper;
    std::vector<cd> csync[3];           // [array][tone*NDOWNSPS + j]

    void configure(Js8Mode::Submode submode)
    {
        if (ready && p.submode == submode) return;
        ready = false;
        p = Js8Mode::parameters(submode);
        nfft1 = 2 * p.nsps;
        nh1 = nfft1 / 2;
        nstep = p.nsps / 4;
        nmax = p.nmax();
        nhsym = nmax / nstep - 3;
        ndown = p.nsps / p.nDownSps;
        nqsymbol = p.nDownSps / 4;
        ndfft1 = p.nsps * p.ndd;
        ndfft2 = ndfft1 / ndown;
        az = 12000.0 / p.nsps * p.dedupeFactor;
        costas = Js8Mode::costas(p.costas);

        specIn = std::make_unique<FftwBuffer>(sizeof(double) * size_t(nfft1));
        specOut = std::make_unique<FftwBuffer>(sizeof(fftw_complex) * size_t(nh1 + 1));
        specPlan.r2c(nfft1, static_cast<double *>(specIn->p), static_cast<fftw_complex *>(specOut->p));
        longIn = std::make_unique<FftwBuffer>(sizeof(double) * size_t(ndfft1));
        longOut = std::make_unique<FftwBuffer>(sizeof(fftw_complex) * size_t(ndfft1 / 2 + 1));
        longPlan.r2c(ndfft1, static_cast<double *>(longIn->p), static_cast<fftw_complex *>(longOut->p));
        down = std::make_unique<FftwBuffer>(sizeof(fftw_complex) * size_t(ndfft2));
        downPlan.c2c(ndfft2, static_cast<fftw_complex *>(down->p), FFTW_BACKWARD);
        subBuf = std::make_unique<FftwBuffer>(sizeof(fftw_complex) * size_t(nmax));
        subForward.c2c(nmax, static_cast<fftw_complex *>(subBuf->p), FFTW_FORWARD);
        subBackward.c2c(nmax, static_cast<fftw_complex *>(subBuf->p), FFTW_BACKWARD);

        // Subtraction filter: cos^2 window of NFILT+1 taps, zero phase.
        const int nfilt = 1400;
        auto *w = static_cast<fftw_complex *>(subBuf->p);
        std::fill(reinterpret_cast<double *>(w), reinterpret_cast<double *>(w) + 2 * size_t(nmax), 0.0);
        double sum = 0.0;
        std::vector<double> window(static_cast<size_t>(nfilt + 1));
        for (int j = -nfilt / 2; j <= nfilt / 2; ++j) {
            const double v = std::pow(std::cos(kJs8Pi * j / nfilt), 2);
            window[size_t(j + nfilt / 2)] = v;
            sum += v;
        }
        for (int j = -nfilt / 2; j <= nfilt / 2; ++j) {
            const int idx = (j + nmax) % nmax;
            w[idx][0] = window[size_t(j + nfilt / 2)] / sum;
        }
        subForward.execute();
        filterSpectrum.resize(size_t(nmax));
        for (int i = 0; i < nmax; ++i) filterSpectrum[size_t(i)] = cd(w[i][0], w[i][1]) / double(nmax);

        taper.resize(size_t(p.ndd + 1));
        for (int i = 0; i <= p.ndd; ++i) taper[size_t(i)] = 0.5 * (1.0 + std::cos(i * kJs8Pi / p.ndd));

        for (int a = 0; a < 3; ++a) {
            csync[a].assign(size_t(7 * p.nDownSps), cd());
            for (int i = 0; i < 7; ++i) {
                const double dphi = kTwoPi * costas[size_t(a)][size_t(i)] / p.nDownSps;
                double phi = 0.0;
                for (int j = 0; j < p.nDownSps; ++j) {
                    csync[a][size_t(i * p.nDownSps + j)] = cd(std::cos(phi), std::sin(phi));
                    phi = std::fmod(phi + dphi, kTwoPi);
                }
            }
        }
        ready = true;
    }

    // ---- syncjs8 + baselinejs8 -------------------------------------------
    struct Candidate { double freq; double dt; double sync; };

    std::vector<double> baseline(std::vector<double> savg, int nfa, int nfb)
    {
        std::vector<double> sbase(static_cast<size_t>(nh1 + 1), 0.0);
        const double df = 12000.0 / (p.nsps * 2.0);
        const int ia = std::max(1, int(std::lround(nfa / df)));
        const int ib = std::min(nh1, int(std::lround(nfb / df)));
        if (ib <= ia) return sbase;
        for (int i = ia; i <= ib; ++i) savg[size_t(i)] = 10.0 * std::log10(std::max(savg[size_t(i)], 1e-30));
        const int nseg = 10, npct = 10;
        const int nlen = (ib - ia + 1) / nseg;
        const int i0 = (ib - ia + 1) / 2;
        std::vector<double> x, y;
        for (int n = 1; n <= nseg && nlen > 0; ++n) {
            const int ja = ia + (n - 1) * nlen;
            const int jb = ja + nlen - 1;
            std::vector<double> seg(savg.begin() + ja, savg.begin() + jb + 1);
            std::sort(seg.begin(), seg.end());
            int j = int(std::lround(nlen * 0.01 * npct));
            j = qBound(1, j, nlen);
            const double base = seg[size_t(j - 1)];
            for (int i = ja; i <= jb; ++i) {
                if (savg[size_t(i)] <= base && x.size() < 1000) {
                    x.push_back(i - i0);
                    y.push_back(savg[size_t(i)]);
                }
            }
        }
        const std::vector<double> a = polyfit(x, y, 5);
        for (int i = ia; i <= ib; ++i) {
            const double t = i - i0;
            sbase[size_t(i)] = a[0] + t * (a[1] + t * (a[2] + t * (a[3] + t * a[4]))) + 0.65;
        }
        return sbase;
    }

    std::vector<Candidate> sync(const std::vector<float> &dd, int nfa, int nfb, double syncmin, int nfqso,
                                int maxCandidates, std::vector<double> *sbaseOut)
    {
        // s(i,j): i = 1..nh1 frequency bin, j = 1..nhsym quarter-symbol step.
        const int stride = nh1 + 1;
        std::vector<float> s(static_cast<size_t>(stride) * size_t(nhsym + 1), 0.0f);
        std::vector<double> savg(static_cast<size_t>(nh1 + 1), 0.0);
        const double tstep = nstep / 12000.0;
        const double df = 12000.0 / nfft1;
        const double fac = 1.0 / 300.0;
        auto *in = static_cast<double *>(specIn->p);
        auto *out = static_cast<fftw_complex *>(specOut->p);
        for (int j = 1; j <= nhsym; ++j) {
            const int ia = (j - 1) * nstep;
            for (int k = 0; k < p.nsps; ++k) in[k] = fac * dd[size_t(ia + k)];
            std::fill(in + p.nsps, in + nfft1, 0.0);
            specPlan.execute();
            for (int i = 1; i <= nh1; ++i) {
                const double v = out[i][0] * out[i][0] + out[i][1] * out[i][1];
                s[size_t(j) * size_t(stride) + size_t(i)] = float(v);
                savg[size_t(i)] += v;
            }
        }
        *sbaseOut = baseline(savg, nfa, nfb);

        const int ia = std::max(1, int(std::lround(nfa / df)));
        const int ib = std::min(nh1 - 7 * (nfft1 / p.nsps), int(std::lround(nfb / df)));
        const int nssy = p.nsps / nstep;
        const int nfos = nfft1 / p.nsps;
        const int jstrt = int(p.startSeconds() / tstep);
        if (ib < ia) return {};

        auto S = [&](int i, int j) { return double(s[size_t(j) * size_t(stride) + size_t(i)]); };
        std::vector<double> red(static_cast<size_t>(nh1 + 1), 0.0);
        std::vector<int> jpeak(static_cast<size_t>(nh1 + 1), 0);
        for (int i = ia; i <= ib; ++i) {
            double bestSync = -1.0;
            int bestJ = -p.jz;
            for (int j = -p.jz; j <= p.jz; ++j) {
                double ta = 0, tb = 0, tc = 0, t0a = 0, t0b = 0, t0c = 0;
                for (int n = 0; n <= 6; ++n) {
                    const int k = j + jstrt + nssy * n;
                    const int offs[3] = {k, k + nssy * 36, k + nssy * 72};
                    double *tt[3] = {&ta, &tb, &tc};
                    double *t0[3] = {&t0a, &t0b, &t0c};
                    for (int a = 0; a < 3; ++a) {
                        const int sy = offs[a];
                        if (sy < 1 || sy > nhsym) continue;
                        *tt[a] += S(i + nfos * costas[size_t(a)][size_t(n)], sy);
                        double row = 0.0;
                        for (int q = 0; q <= 6; ++q) row += S(i + nfos * q, sy);
                        *t0[a] += row;
                    }
                }
                auto ratio = [](double t, double t0) {
                    const double base = (t0 - t) / 6.0;
                    return base > 0.0 ? t / base : 0.0;
                };
                const double abc = ratio(ta + tb + tc, t0a + t0b + t0c);
                const double ab = ratio(ta + tb, t0a + t0b);
                const double bc = ratio(tb + tc, t0b + t0c);
                const double v = std::max({abc, ab, bc});
                if (v > bestSync) { bestSync = v; bestJ = j; }
            }
            jpeak[size_t(i)] = bestJ;
            red[size_t(i)] = bestSync;
        }

        const int iz = ib - ia + 1;
        std::vector<double> redRange(red.begin() + ia, red.begin() + ib + 1);
        const std::vector<int> indx = ascendingOrder(redRange);
        const int npctile = int(std::lround(0.40 * iz));
        if (npctile < 1) return {};
        const double base = red[size_t(ia + indx[size_t(npctile - 1)])];
        if (!(base > 0.0)) return {};
        for (int i = ia; i <= ib; ++i) red[size_t(i)] /= base;

        std::vector<Candidate> c0;
        for (int i = 1; i <= std::min(iz, maxCandidates); ++i) {
            const int n = ia + indx[size_t(iz - i)];
            if (red[size_t(n)] < syncmin || std::isnan(red[size_t(n)])) break;
            c0.push_back({n * df, (jpeak[size_t(n)] - 1) * tstep, red[size_t(n)]});
        }
        for (size_t i = 1; i < c0.size(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                const double fdiff = std::fabs(c0[i].freq) - std::fabs(c0[j].freq);
                if (std::fabs(fdiff) < az) {
                    if (c0[i].sync >= c0[j].sync) c0[j].sync = 0.0;
                    else c0[i].sync = 0.0;
                }
            }
        }
        for (Candidate &c : c0)
            if (std::fabs(c.freq - nfqso) < 10.0) c.freq = -c.freq;
        std::stable_sort(c0.begin(), c0.end(), [](const Candidate &a, const Candidate &b) { return a.freq < b.freq; });
        std::vector<Candidate> out2;
        for (Candidate c : c0) {
            if (c.sync >= syncmin) { c.freq = std::fabs(c.freq); out2.push_back(c); }
        }
        return out2;
    }

    // ---- js8_downsample ---------------------------------------------------
    void longFft(const std::vector<float> &dd)
    {
        auto *in = static_cast<double *>(longIn->p);
        for (int i = 0; i < ndfft1; ++i) in[i] = i < nmax ? double(dd[size_t(i)]) : 0.0;
        longPlan.execute();
    }

    std::vector<cd> downsample(double f0)
    {
        const auto *cx = static_cast<const fftw_complex *>(longOut->p);
        auto *c1 = static_cast<fftw_complex *>(down->p);
        std::vector<cd> bins(static_cast<size_t>(ndfft2), cd(0.0, 0.0));
        const double df = 12000.0 / ndfft1;
        const double baud = 12000.0 / p.nsps;
        const int i0 = int(std::lround(f0 / df));
        const int it = std::min(int(std::lround((f0 + 8.5 * baud) / df)), ndfft1 / 2);
        const int ib = std::max(1, int(std::lround((f0 - 1.5 * baud) / df)));
        int k = 0;
        for (int i = ib; i <= it && k < ndfft2; ++i) bins[size_t(k++)] = cd(cx[i][0], cx[i][1]);
        const int ndd = p.ndd;
        if (k > ndd) {
            for (int m = 0; m <= ndd; ++m) bins[size_t(m)] *= taper[size_t(ndd - m)];
            for (int m = 0; m <= ndd; ++m) bins[size_t(k - 1 - ndd + m)] *= taper[size_t(m)];
        }
        const int shift = ((i0 - ib) % ndfft2 + ndfft2) % ndfft2;
        for (int i = 0; i < ndfft2; ++i) {
            const cd v = bins[size_t((i + shift) % ndfft2)];
            c1[i][0] = v.real();
            c1[i][1] = v.imag();
        }
        downPlan.execute();
        const double fac = 1.0 / std::sqrt(double(ndfft1) * ndfft2);
        std::vector<cd> out(static_cast<size_t>(ndfft2));
        for (int i = 0; i < ndfft2; ++i) out[size_t(i)] = cd(c1[i][0], c1[i][1]) * fac;
        return out;
    }

    // ---- syncjs8d ---------------------------------------------------------
    double fineSync(const std::vector<cd> &cd0, int i0, double delf) const
    {
        const int nds = p.nDownSps;
        const int np2 = int(cd0.size());
        std::vector<cd> ctwk(static_cast<size_t>(nds), cd(1.0, 0.0));
        if (delf != 0.0) {
            const double dphi = kTwoPi * delf / (12000.0 / ndown);
            double phi = 0.0;
            for (int i = 0; i < nds; ++i) { ctwk[size_t(i)] = cd(std::cos(phi), std::sin(phi)); phi = std::fmod(phi + dphi, kTwoPi); }
        }
        double sync = 0.0;
        for (int i = 0; i <= 6; ++i) {
            const int starts[3] = {i0 + i * nds, i0 + i * nds + 36 * nds, i0 + i * nds + 72 * nds};
            for (int a = 0; a < 3; ++a) {
                const int s0 = starts[a];
                if (s0 < 0 || s0 + nds - 1 > np2 - 1) continue;
                cd z;
                for (int j = 0; j < nds; ++j)
                    z += cd0[size_t(s0 + j)] * std::conj(csync[a][size_t(i * nds + j)] * ctwk[size_t(j)]);
                sync += std::norm(z);
            }
        }
        return sync;
    }

    // ---- genjs8refsig + subtractjs8 --------------------------------------
    void subtract(std::vector<float> &dd, const std::vector<int> &tones, double f0, double dt)
    {
        const int nframe = p.nsps * NN;
        const int nstart = int(dt * 12000.0);   // 0-based first sample
        auto *buf = static_cast<fftw_complex *>(subBuf->p);
        std::fill(reinterpret_cast<double *>(buf), reinterpret_cast<double *>(buf) + 2 * size_t(nmax), 0.0);
        std::vector<cd> cref(static_cast<size_t>(nframe));
        double phi = 0.0;
        int k = 0;
        for (int i = 0; i < NN; ++i) {
            const double dphi = kTwoPi * (f0 / 12000.0 + double(tones[size_t(i)]) / p.nsps);
            for (int s = 0; s < p.nsps; ++s) {
                cref[size_t(k++)] = cd(std::cos(phi), std::sin(phi));
                phi = std::fmod(phi + dphi, kTwoPi);
            }
        }
        for (int i = 0; i < nframe && i < nmax; ++i) {
            const int id = nstart + i;
            if (id >= 0 && id < nmax) {
                const cd v = double(dd[size_t(id)]) * std::conj(cref[size_t(i)]);
                buf[i][0] = v.real();
                buf[i][1] = v.imag();
            }
        }
        subForward.execute();
        for (int i = 0; i < nmax; ++i) {
            const cd v = cd(buf[i][0], buf[i][1]) * filterSpectrum[size_t(i)];
            buf[i][0] = v.real();
            buf[i][1] = v.imag();
        }
        subBackward.execute();
        for (int i = 0; i < nframe && i < nmax; ++i) {
            const int j = nstart + i;
            if (j >= 0 && j < nmax)
                dd[size_t(j)] -= float(2.0 * (cd(buf[i][0], buf[i][1]) * cref[size_t(i)]).real());
        }
    }

    // ---- js8dec -----------------------------------------------------------
    bool decodeCandidate(std::vector<float> &dd, bool &newdat, const Configuration &cfg, bool lsubtract,
                         double &f1, double &xdt, double xbase, Result *result)
    {
        if (newdat) { longFft(dd); newdat = false; }
        const double fs2 = 12000.0 / ndown;
        const double dt2 = 1.0 / fs2;
        std::vector<cd> cd0 = downsample(f1);

        int i0 = int(std::lround((xdt + p.startSeconds()) * fs2));
        double smax = 0.0;
        int ibest = i0;
        for (int idt = i0 - nqsymbol; idt <= i0 + nqsymbol; ++idt) {
            const double s = fineSync(cd0, idt, 0.0);
            if (s > smax) { smax = s; ibest = idt; }
        }
        const double xdt2 = ibest * dt2;

        i0 = int(std::lround(xdt2 * fs2));
        smax = 0.0;
        double delfbest = 0.0;
        for (int ifr = -5; ifr <= 5; ++ifr) {
            const double delf = ifr * 0.5;
            const double s = fineSync(cd0, i0, delf);
            if (s > smax) { smax = s; delfbest = delf; }
        }
        {
            // twkfreq1 with a(1) = -delfbest.
            const double dphi = -delfbest * kTwoPi / fs2;
            const cd wstep(std::cos(dphi), std::sin(dphi));
            cd w(1.0, 0.0);
            for (cd &v : cd0) { w *= wstep; v *= w; }
        }
        xdt = xdt2;
        f1 += delfbest;
        const double sync = fineSync(cd0, i0, 0.0);

        const int nds = p.nDownSps;
        const int np2 = int(cd0.size());
        std::vector<std::array<double, 8>> s2(static_cast<size_t>(NN));
        for (int k = 0; k < NN; ++k) {
            const int i1 = ibest + k * nds;
            std::array<double, 8> &row = s2[size_t(k)];
            row.fill(0.0);
            if (i1 < 0 || i1 + nds - 1 > np2 - 1) continue;
            for (int t = 0; t < 8; ++t) {
                cd z;
                for (int n = 0; n < nds; ++n) {
                    const double ang = -kTwoPi * t * n / nds;
                    z += cd0[size_t(i1 + n)] * cd(std::cos(ang), std::sin(ang));
                }
                row[size_t(t)] = std::abs(z) / 1e3;
            }
        }

        int nsync = 0;
        for (int k = 0; k < 7; ++k) {
            const int pos[3] = {k, k + 36, k + 72};
            for (int a = 0; a < 3; ++a) {
                const auto &row = s2[size_t(pos[a])];
                const int ip = int(std::max_element(row.begin(), row.end()) - row.begin());
                if (costas[size_t(a)][size_t(k)] == ip) ++nsync;
            }
        }
        if (nsync <= 6) return false;

        std::vector<std::array<double, 8>> s1;
        s1.reserve(ND);
        for (int k = 0; k < NN; ++k) {
            if (k < 7 || (k >= 36 && k <= 42) || k > 71) continue;
            s1.push_back(s2[size_t(k)]);
        }
        std::vector<double> all;
        all.reserve(8 * ND);
        for (const auto &row : s1) all.insert(all.end(), row.begin(), row.end());
        std::vector<double> sorted = all;
        std::sort(sorted.begin(), sorted.end());
        const double xmed = sorted[size_t(std::lround(0.5 * 8 * ND) - 1)];
        if (!(xmed > 0.0)) return false;

        Js8Ldpc::Llr bmeta{}, bmetb{};
        for (int j = 0; j < ND; ++j) {
            std::array<double, 8> ps = s1[size_t(j)];
            for (double &v : ps) v /= xmed;
            auto metrics = [&](const std::array<double, 8> &q, float *out) {
                const double r1 = std::max({q[1], q[3], q[5], q[7]}) - std::max({q[0], q[2], q[4], q[6]});
                const double r2 = std::max({q[2], q[3], q[6], q[7]}) - std::max({q[0], q[1], q[4], q[5]});
                const double r4 = std::max({q[4], q[5], q[6], q[7]}) - std::max({q[0], q[1], q[2], q[3]});
                out[0] = float(r4); out[1] = float(r2); out[2] = float(r1);
            };
            metrics(ps, &bmeta[size_t(3 * j)]);
            std::array<double, 8> psl{};
            for (int t = 0; t < 8; ++t) psl[size_t(t)] = std::log(ps[size_t(t)] + 1e-32);
            metrics(psl, &bmetb[size_t(3 * j)]);
        }
        auto normalize = [](Js8Ldpc::Llr &b) {
            double av = 0, av2 = 0;
            for (float v : b) { av += v; av2 += double(v) * v; }
            av /= b.size(); av2 /= b.size();
            const double var = av2 - av * av;
            const double sig = var > 0 ? std::sqrt(var) : std::sqrt(av2);
            if (sig > 0) for (float &v : b) v = float(v / sig);
        };
        normalize(bmeta);
        normalize(bmetb);
        const float scalefac = 2.83f;
        Js8Ldpc::Llr llr0{}, llr1{};
        for (int i = 0; i < Js8Ldpc::N; ++i) { llr0[size_t(i)] = scalefac * bmeta[size_t(i)]; llr1[size_t(i)] = scalefac * bmetb[size_t(i)]; }

        for (int ipass = 1; ipass <= 4; ++ipass) {
            Js8Ldpc::Llr llr = ipass == 2 ? llr1 : llr0;
            if (ipass == 3) std::fill(llr.begin(), llr.begin() + 24, 0.0f);
            if (ipass == 4) std::fill(llr.begin(), llr.begin() + 48, 0.0f);

            Js8Ldpc::Message decoded{};
            Js8Ldpc::Codeword cw{};
            int nharderrors = Js8Ldpc::bpDecode(llr, 30, &decoded, &cw);
            float dmin = 0.0f;
            bool usedOsd = false;
            if (cfg.depth >= 3 && nharderrors < 0) {
                int ndeep = 3;
                if (std::fabs(cfg.rxFrequencyHz - f1) <= cfg.apWidthHz || std::fabs(cfg.txFrequencyHz - f1) <= cfg.apWidthHz)
                    ndeep = (ipass == 3 || ipass == 4) ? 3 : 4;
                nharderrors = Js8Ldpc::osdDecode(llr, ndeep, &decoded, &cw, &dmin);
                usedOsd = true;
            }
            if (std::all_of(cw.begin(), cw.end(), [](std::uint8_t b) { return b == 0; })) continue;
            if (!(nharderrors >= 0 && nharderrors + dmin < 60.0f &&
                  !(sync < 2.0 && nharderrors > 35) &&
                  !(ipass > 2 && nharderrors > 39) &&
                  !(ipass == 4 && nharderrors > 30))) continue;
            if (!Js8Ldpc::crcOk(decoded)) continue;

            int bits = 0;
            const QString frame = Js8Ldpc::unpackFrame(decoded, &bits);
            const Js8Ldpc::Codeword regenerated = Js8Ldpc::encode(decoded);
            std::vector<int> tones(static_cast<size_t>(NN));
            for (int k = 0; k < 7; ++k) {
                tones[size_t(k)] = costas[0][size_t(k)];
                tones[size_t(36 + k)] = costas[1][size_t(k)];
                tones[size_t(72 + k)] = costas[2][size_t(k)];
            }
            int kpos = 7;
            for (int j = 0; j < ND; ++j) {
                if (j == 29) kpos += 7;
                tones[size_t(kpos++)] = regenerated[size_t(3 * j)] * 4 + regenerated[size_t(3 * j + 1)] * 2 + regenerated[size_t(3 * j + 2)];
            }
            if (lsubtract) subtract(dd, tones, f1, xdt2);

            double xsig = 0.0;
            for (int i = 0; i < NN; ++i) xsig += std::pow(s2[size_t(i)][size_t(tones[size_t(i)])], 2);
            double xsnr = db(xsig / xbase - 1.0) - 32.0;
            if (xsnr < -28.0) xsnr = -28.0;

            result->frame = frame;
            result->bits = bits;
            result->frequencyHz = f1;
            result->dtSeconds = xdt2 - p.startSeconds();
            result->snrDb = int(std::lround(xsnr));
            result->quality = 1.0 - (nharderrors + dmin) / 60.0;
            result->hardErrors = nharderrors;
            result->osd = usedOsd;
            return true;
        }
        return false;
    }
};

Js8NativeEngine::Js8NativeEngine() : d(std::make_unique<Impl>()) {}
Js8NativeEngine::~Js8NativeEngine() = default;

QVector<Js8NativeEngine::Result> Js8NativeEngine::decode(const QVector<float> &samples, const Configuration &config)
{
    d->configure(config.submode);
    const int nmax = d->nmax;
    std::vector<float> dd(static_cast<size_t>(nmax), 0.0f);
    for (int i = 0; i < nmax && i < samples.size(); ++i) dd[size_t(i)] = samples.at(i) * 32768.0f;

    const int depth = qBound(1, config.depth, 3);
    const int npass = depth == 1 ? 1 : (depth == 2 ? 3 : 4);
    const int nfa = qBound(100, config.lowHz, 5000);
    const int nfb = qBound(nfa + 50, config.highHz, 5500);
    QVector<Result> results;
    QStringList seen;
    QVector<int> seenSnr;
    int n2 = 0, n3 = 0;
    for (int ipass = 1; ipass <= npass; ++ipass) {
        bool lsubtract = true;
        if (ipass == 1) {
            lsubtract = depth != 1;
        } else if (ipass == 2) {
            n2 = results.size();
            if (results.isEmpty()) continue;
        } else if (ipass == 3) {
            n3 = results.size();
            if (results.size() - n2 == 0) continue;
        } else {
            if (results.size() - n3 == 0) continue;
            lsubtract = false;
        }
        std::vector<double> sbase;
        const auto candidates = d->sync(dd, nfa, nfb, config.syncMin, config.rxFrequencyHz,
                                        qBound(1, config.maxCandidates, 300), &sbase);
        bool newdat = true;
        const double df = 12000.0 / d->nfft1;
        for (const auto &c : candidates) {
            double f1 = c.freq;
            double xdt = c.dt;
            const int bin = qBound(0, int(std::lround(f1 / df)), int(sbase.size()) - 1);
            const double xbase = std::pow(10.0, 0.1 * (sbase[size_t(bin)] - 40.0));
            Result r;
            if (!d->decodeCandidate(dd, newdat, config, lsubtract, f1, xdt, xbase, &r)) continue;
            const QString key = r.frame + QString::number(r.bits);
            bool dupe = false;
            for (int k = 0; k < seen.size(); ++k)
                if (seen.at(k) == key && r.snrDb <= seenSnr.at(k)) dupe = true;
            if (dupe) continue;
            seen << key;
            seenSnr << r.snrDb;
            results << r;
        }
    }
    return results;
}
