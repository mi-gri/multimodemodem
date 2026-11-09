// SPDX-License-Identifier: GPL-3.0-or-later
// C++ port of the LDPC(174,87) routines used by JS8Call 2.2.0 (lib/ft8/*.f90,
// WSJT-X heritage).  Index conventions follow the Fortran code; arrays here
// are 0-based.
#include "Js8Ldpc.h"

#include "../../third_party/js8call_gpl/Js8LdpcTables.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <unordered_map>
#include <vector>

namespace Js8Ldpc {
namespace {

struct Generator
{
    // Parity rows: gen[i][j] = parity bit i depends on message bit j.
    std::array<std::array<std::uint8_t, K>, M> parity{};
};

const Generator &generator()
{
    static const Generator g = [] {
        Generator out;
        for (int i = 0; i < M; ++i) {
            const char *hex = js8ldpc::kGenerator[i];
            for (int j = 0; j < 22; ++j) {
                const char c = hex[j];
                const int nibble = (c >= '0' && c <= '9') ? c - '0' : 10 + (c - 'a');
                for (int b = 0; b < 4; ++b) {
                    const int col = j * 4 + b;
                    if (col < K) out.parity[size_t(i)][size_t(col)] = std::uint8_t((nibble >> (3 - b)) & 1);
                }
            }
        }
        return out;
    }();
    return g;
}

float platanh(float x)
{
    const float sign = x < 0 ? -1.0f : 1.0f;
    const float z = std::fabs(x);
    if (z <= 0.664f) return x / 0.83f;
    if (z <= 0.9217f) return sign * (z - 0.4064f) / 0.322f;
    if (z <= 0.9951f) return sign * (z - 0.8378f) / 0.0524f;
    if (z <= 0.9998f) return sign * (z - 0.9914f) / 0.0012f;
    return sign * 7.0f;
}

// Next test pattern of the given weight (nextpat in osd174.f90), 0-based.
void nextPattern(std::vector<std::uint8_t> &mi, int order, int &flag)
{
    const int k = int(mi.size());
    int ind = -1;
    for (int i = 0; i < k - 1; ++i)
        if (mi[size_t(i)] == 0 && mi[size_t(i + 1)] == 1) ind = i;
    if (ind < 0) { flag = -1; return; }
    std::vector<std::uint8_t> ms(static_cast<size_t>(k), 0);
    std::copy(mi.begin(), mi.begin() + ind, ms.begin());
    ms[size_t(ind)] = 1;
    ms[size_t(ind + 1)] = 0;
    if (ind + 1 < k - 1) {
        const int nz = order - int(std::accumulate(ms.begin(), ms.end(), 0));
        for (int i = k - nz; i < k; ++i) ms[size_t(i)] = 1;
    }
    mi = ms;
    flag = -1;
    for (int i = 0; i < k; ++i) if (mi[size_t(i)]) { flag = i; break; }
}

} // namespace

const QString &alphabet()
{
    static const QString a = QStringLiteral("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-+/?.");
    return a;
}

int crc12(const Message &bits)
{
    // boost::augmented_crc<12,0xc06> over 11 bytes: the 75 message bits,
    // zero padded to 88 bits.
    unsigned rem = 0;
    for (int i = 0; i < 88; ++i) {
        const unsigned bit = i < 75 ? bits[size_t(i)] & 1u : 0u;
        const unsigned top = (rem >> 11) & 1u;
        rem = ((rem << 1) | bit) & 0xfffu;
        if (top) rem ^= 0xc06u;
    }
    return int(rem ^ 42u);
}

bool crcOk(const Message &bits)
{
    int received = 0;
    for (int i = 75; i < 87; ++i) received = (received << 1) | (bits[size_t(i)] & 1);
    return received == crc12(bits);
}

bool packFrame(const QString &frame, int i3bits, Message *out)
{
    if (frame.size() != 12 || out == nullptr) return false;
    Message m{};
    for (int c = 0; c < 12; ++c) {
        const int v = alphabet().indexOf(frame.at(c));
        if (v < 0 || v > 63) return false;
        for (int b = 0; b < 6; ++b) m[size_t(c * 6 + b)] = std::uint8_t((v >> (5 - b)) & 1);
    }
    for (int b = 0; b < 3; ++b) m[size_t(72 + b)] = std::uint8_t((i3bits >> (2 - b)) & 1);
    const int crc = crc12(m);
    for (int b = 0; b < 12; ++b) m[size_t(75 + b)] = std::uint8_t((crc >> (11 - b)) & 1);
    *out = m;
    return true;
}

QString unpackFrame(const Message &bits, int *i3bits)
{
    QString text;
    for (int c = 0; c < 12; ++c) {
        int v = 0;
        for (int b = 0; b < 6; ++b) v = (v << 1) | (bits[size_t(c * 6 + b)] & 1);
        text.append(alphabet().at(v));
    }
    if (i3bits) *i3bits = 4 * bits[72] + 2 * bits[73] + bits[74];
    return text;
}

Codeword encode(const Message &message)
{
    const Generator &g = generator();
    std::array<std::uint8_t, N> itmp{};
    for (int i = 0; i < M; ++i) {
        int sum = 0;
        for (int j = 0; j < K; ++j) sum += message[size_t(j)] & g.parity[size_t(i)][size_t(j)];
        itmp[size_t(i)] = std::uint8_t(sum & 1);
    }
    for (int j = 0; j < K; ++j) itmp[size_t(M + j)] = message[size_t(j)];
    Codeword cw{};
    for (int i = 0; i < N; ++i) cw[size_t(js8ldpc::kColumnOrder[i])] = itmp[size_t(i)];
    return cw;
}

int bpDecode(const Llr &llr, int maxIterations, Message *decoded, Codeword *cwOut)
{
    using namespace js8ldpc;
    float tov[N][3] = {};
    float toc[M][7] = {};
    float tanhtoc[M][7] = {};
    std::array<float, N> zn{};
    std::array<std::uint8_t, N> cw{};

    for (int j = 0; j < M; ++j)
        for (int i = 0; i < kNrw[j]; ++i) toc[j][i] = llr[size_t(kNm[j][i] - 1)];

    int ncnt = 0, nclast = 0;
    for (int iter = 0; iter <= maxIterations; ++iter) {
        for (int i = 0; i < N; ++i) zn[size_t(i)] = llr[size_t(i)] + tov[i][0] + tov[i][1] + tov[i][2];
        for (int i = 0; i < N; ++i) cw[size_t(i)] = zn[size_t(i)] > 0.0f ? 1 : 0;
        int ncheck = 0;
        for (int i = 0; i < M; ++i) {
            int synd = 0;
            for (int k = 0; k < kNrw[i]; ++k) synd += cw[size_t(kNm[i][k] - 1)];
            if (synd & 1) ++ncheck;
        }
        if (ncheck == 0) {
            Codeword systematic{};
            for (int i = 0; i < N; ++i) systematic[size_t(i)] = cw[size_t(kColumnOrder[i])];
            if (decoded) for (int i = 0; i < K; ++i) (*decoded)[size_t(i)] = systematic[size_t(M + i)];
            if (cwOut) *cwOut = cw;
            int nerr = 0;
            for (int i = 0; i < N; ++i) if ((2 * int(cw[size_t(i)]) - 1) * llr[size_t(i)] < 0.0f) ++nerr;
            return nerr;
        }
        if (iter > 0) {
            const int nd = ncheck - nclast;
            ncnt = nd < 0 ? 0 : ncnt + 1;
            if (ncnt >= 5 && iter >= 10 && ncheck > 15) return -1;
        }
        nclast = ncheck;

        for (int j = 0; j < M; ++j) {
            for (int i = 0; i < kNrw[j]; ++i) {
                const int ibj = kNm[j][i] - 1;
                float v = zn[size_t(ibj)];
                for (int kk = 0; kk < 3; ++kk) if (kMn[ibj][kk] - 1 == j) v -= tov[ibj][kk];
                toc[j][i] = v;
            }
        }
        for (int i = 0; i < M; ++i)
            for (int k = 0; k < 7; ++k) tanhtoc[i][k] = std::tanh(-toc[i][k] / 2.0f);
        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < 3; ++i) {
                const int ichk = kMn[j][i] - 1;
                float t = 1.0f;
                for (int k = 0; k < kNrw[ichk]; ++k)
                    if (kNm[ichk][k] - 1 != j) t *= tanhtoc[ichk][k];
                tov[j][i] = 2.0f * platanh(-t);
            }
        }
    }
    return -1;
}

int osdDecode(const Llr &llr, int depth, Message *decoded, Codeword *cwOut, float *dminOut)
{
    using namespace js8ldpc;
    const Generator &g = generator();

    // gen(K,N): message row r, column c; parity columns first, then identity.
    std::vector<std::vector<std::uint8_t>> genmrb(static_cast<size_t>(K), std::vector<std::uint8_t>(size_t(N), 0));
    std::vector<std::vector<std::uint8_t>> gen(static_cast<size_t>(K), std::vector<std::uint8_t>(size_t(N), 0));
    for (int r = 0; r < K; ++r) {
        for (int i = 0; i < M; ++i) gen[size_t(r)][size_t(i)] = g.parity[size_t(i)][size_t(r)];
        gen[size_t(r)][size_t(M + r)] = 1;
    }

    std::vector<float> rx(static_cast<size_t>(N)), absrx(static_cast<size_t>(N));
    std::vector<std::uint8_t> hdec(static_cast<size_t>(N));
    for (int i = 0; i < N; ++i) {
        rx[size_t(i)] = llr[size_t(kColumnOrder[i])];
        hdec[size_t(i)] = rx[size_t(i)] >= 0.0f ? 1 : 0;
        absrx[size_t(i)] = std::fabs(rx[size_t(i)]);
    }
    std::vector<int> indx(static_cast<size_t>(N));
    std::iota(indx.begin(), indx.end(), 0);
    std::stable_sort(indx.begin(), indx.end(), [&](int a, int b) { return absrx[size_t(a)] < absrx[size_t(b)]; });

    std::vector<int> indices(static_cast<size_t>(N));
    for (int i = 0; i < N; ++i) {
        const int src = indx[size_t(N - 1 - i)];
        indices[size_t(i)] = src;
        for (int r = 0; r < K; ++r) genmrb[size_t(r)][size_t(i)] = gen[size_t(r)][size_t(src)];
    }

    for (int id = 0; id < K; ++id) {
        for (int icol = id; icol < std::min(N, K + 20); ++icol) {
            if (genmrb[size_t(id)][size_t(icol)] != 1) continue;
            if (icol != id) {
                for (int r = 0; r < K; ++r) std::swap(genmrb[size_t(r)][size_t(id)], genmrb[size_t(r)][size_t(icol)]);
                std::swap(indices[size_t(id)], indices[size_t(icol)]);
            }
            for (int ii = 0; ii < K; ++ii) {
                if (ii != id && genmrb[size_t(ii)][size_t(id)] == 1)
                    for (int c = 0; c < N; ++c) genmrb[size_t(ii)][size_t(c)] ^= genmrb[size_t(id)][size_t(c)];
            }
            break;
        }
    }

    {
        std::vector<std::uint8_t> h2(static_cast<size_t>(N));
        std::vector<float> a2(static_cast<size_t>(N));
        for (int i = 0; i < N; ++i) { h2[size_t(i)] = hdec[size_t(indices[size_t(i)])]; a2[size_t(i)] = absrx[size_t(indices[size_t(i)])]; }
        hdec.swap(h2);
        absrx.swap(a2);
    }

    auto mrbencode = [&](const std::vector<std::uint8_t> &me, std::vector<std::uint8_t> &cw) {
        std::fill(cw.begin(), cw.end(), 0);
        for (int i = 0; i < K; ++i)
            if (me[size_t(i)]) for (int c = 0; c < N; ++c) cw[size_t(c)] ^= genmrb[size_t(i)][size_t(c)];
    };

    std::vector<std::uint8_t> m0(hdec.begin(), hdec.begin() + K);
    std::vector<std::uint8_t> c0(static_cast<size_t>(N)), ce(static_cast<size_t>(N)), best(static_cast<size_t>(N));
    mrbencode(m0, c0);
    int nhardmin = 0;
    float dmin = 0.0f;
    for (int i = 0; i < N; ++i) {
        const int x = c0[size_t(i)] ^ hdec[size_t(i)];
        nhardmin += x;
        dmin += x * absrx[size_t(i)];
    }
    best = c0;

    depth = std::min(depth, 5);
    if (depth > 0) {
        int nord = 1, npre1 = 0, npre2 = 0, nt = 40, ntheta = 12, ntau = 14;
        if (depth == 2) { npre1 = 1; }
        else if (depth == 3) { npre1 = 1; npre2 = 1; ntau = 14; }
        else if (depth == 4) { nord = 2; npre1 = 1; ntau = 19; }
        else if (depth == 5) { nord = 2; npre1 = 1; npre2 = 1; ntau = 19; }

        std::vector<std::uint8_t> misub(static_cast<size_t>(K)), mi(static_cast<size_t>(K)), me(static_cast<size_t>(K));
        std::vector<std::uint8_t> e2sub(static_cast<size_t>(M)), e2(static_cast<size_t>(M));
        for (int iorder = 1; iorder <= nord; ++iorder) {
            std::fill(misub.begin(), misub.end(), 0);
            for (int i = K - iorder; i < K; ++i) misub[size_t(i)] = 1;
            int iflag = K - iorder;
            float d1 = 0.0f;
            while (iflag >= 0) {
                const int iend = (iorder == nord && npre1 == 0) ? iflag : 0;
                for (int n1 = iflag; n1 >= iend; --n1) {
                    mi = misub;
                    mi[size_t(n1)] = 1;
                    for (int i = 0; i < K; ++i) me[size_t(i)] = m0[size_t(i)] ^ mi[size_t(i)];
                    int nd1Kpt = 0;
                    if (n1 == iflag) {
                        mrbencode(me, ce);
                        for (int i = 0; i < M; ++i) e2sub[size_t(i)] = ce[size_t(K + i)] ^ hdec[size_t(K + i)];
                        e2 = e2sub;
                        for (int i = 0; i < nt; ++i) nd1Kpt += e2sub[size_t(i)];
                        nd1Kpt += 1;
                        d1 = 0.0f;
                        for (int i = 0; i < K; ++i) d1 += (me[size_t(i)] ^ hdec[size_t(i)]) * absrx[size_t(i)];
                    } else {
                        for (int i = 0; i < M; ++i) e2[size_t(i)] = e2sub[size_t(i)] ^ genmrb[size_t(n1)][size_t(K + i)];
                        for (int i = 0; i < nt; ++i) nd1Kpt += e2[size_t(i)];
                        nd1Kpt += 2;
                    }
                    if (nd1Kpt > ntheta) continue;
                    mrbencode(me, ce);
                    float dd = d1;
                    if (n1 == iflag) {
                        for (int i = 0; i < M; ++i) dd += e2sub[size_t(i)] * absrx[size_t(K + i)];
                    } else {
                        dd += (ce[size_t(n1)] ^ hdec[size_t(n1)]) * absrx[size_t(n1)];
                        for (int i = 0; i < M; ++i) dd += e2[size_t(i)] * absrx[size_t(K + i)];
                    }
                    if (dd < dmin) {
                        dmin = dd;
                        best = ce;
                        nhardmin = 0;
                        for (int i = 0; i < N; ++i) nhardmin += ce[size_t(i)] ^ hdec[size_t(i)];
                    }
                }
                nextPattern(misub, iorder, iflag);
            }
        }

        if (npre2 == 1) {
            // Box the parity patterns of all message bit pairs (boxit).
            std::unordered_map<int, std::vector<std::pair<int, int>>> boxes;
            for (int i1 = K - 1; i1 >= 0; --i1) {
                for (int i2 = i1 - 1; i2 >= 0; --i2) {
                    int pat = 0;
                    for (int i = 0; i < ntau; ++i)
                        pat = (pat << 1) | (genmrb[size_t(i1)][size_t(K + i)] ^ genmrb[size_t(i2)][size_t(K + i)]);
                    boxes[pat].push_back({i1, i2});
                }
            }
            std::fill(misub.begin(), misub.end(), 0);
            for (int i = K - nord; i < K; ++i) misub[size_t(i)] = 1;
            int iflag = K - nord;
            while (iflag >= 0) {
                for (int i = 0; i < K; ++i) me[size_t(i)] = m0[size_t(i)] ^ misub[size_t(i)];
                mrbencode(me, ce);
                for (int i = 0; i < M; ++i) e2sub[size_t(i)] = ce[size_t(K + i)] ^ hdec[size_t(K + i)];
                for (int i2 = 0; i2 <= ntau; ++i2) {
                    int pat = 0;
                    for (int i = 0; i < ntau; ++i) {
                        const int ui = (i2 > 0 && i == i2 - 1) ? 1 : 0;
                        pat = (pat << 1) | (e2sub[size_t(i)] ^ ui);
                    }
                    const auto found = boxes.find(pat);
                    if (found == boxes.end()) continue;
                    for (const auto &pair : found->second) {
                        mi = misub;
                        mi[size_t(pair.first)] = 1;
                        mi[size_t(pair.second)] = 1;
                        if (std::accumulate(mi.begin(), mi.end(), 0) < nord + npre1 + npre2) break;
                        for (int i = 0; i < K; ++i) me[size_t(i)] = m0[size_t(i)] ^ mi[size_t(i)];
                        mrbencode(me, ce);
                        float dd = 0.0f;
                        int hard = 0;
                        for (int i = 0; i < N; ++i) {
                            const int x = ce[size_t(i)] ^ hdec[size_t(i)];
                            dd += x * absrx[size_t(i)];
                            hard += x;
                        }
                        if (dd < dmin) { dmin = dd; best = ce; nhardmin = hard; }
                    }
                }
                nextPattern(misub, nord, iflag);
            }
        }
    }

    std::vector<std::uint8_t> systematic(static_cast<size_t>(N));
    for (int i = 0; i < N; ++i) systematic[size_t(indices[size_t(i)])] = best[size_t(i)];
    if (decoded) for (int i = 0; i < K; ++i) (*decoded)[size_t(i)] = systematic[size_t(M + i)];
    if (cwOut) for (int i = 0; i < N; ++i) (*cwOut)[size_t(kColumnOrder[i])] = systematic[size_t(i)];
    if (dminOut) *dminOut = dmin;
    return nhardmin;
}

} // namespace Js8Ldpc
