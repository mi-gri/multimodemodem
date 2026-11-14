// SPDX-License-Identifier: GPL-3.0-or-later
//
// C++ port of the WSPR channel code from WSJT-X lib/wsprd (fano.c, tab.c,
// wsprd_utils.c, wsprsim_utils.c).  Copyright 1994 Phil Karn KA9Q,
// 2001-2018 Joe Taylor K1JT, 2014-2018 Steven Franke K9AN.  GNU GPL v3.
#include "WsprCodec.h"

#include "nhash.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

namespace M3Wspr {

const std::array<unsigned char, kSymbols> kSyncVector = {
    1,1,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,
    0,1,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,1,0,1,
    0,0,0,0,0,0,1,0,1,1,0,0,1,1,0,1,0,0,0,1,
    1,0,1,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,0,1,
    0,0,1,0,1,1,0,0,0,1,1,0,1,0,1,0,0,0,1,0,
    0,0,0,0,1,0,0,1,0,0,1,1,1,0,1,1,0,0,1,1,
    0,1,0,0,0,1,1,1,0,0,0,0,0,1,0,1,0,0,1,1,
    0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,1,1,0,
    0,0};

namespace {

// Layland-Lushbaugh code (wsprd fano.c, LL).
constexpr uint32_t kPoly1 = 0xf2d05351u;
constexpr uint32_t kPoly2 = 0xe4613c47u;

const float kMetric6dB[256] = {
#include "WsprMetricTable.inc"
};

inline unsigned parity32(uint32_t v)
{
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v &= 0xf;
    return (0x6996u >> v) & 1u;
}

// Rate 1/2 symbol pair of the encoder state: POLY1 bit in 2, POLY2 bit in 1.
inline unsigned encodeState(uint64_t state)
{
    const uint32_t s = static_cast<uint32_t>(state);
    return (parity32(s & kPoly1) << 1) | parity32(s & kPoly2);
}

// Bit-reversal interleaver index sequence (wsprd interleave/deinterleave).
const std::array<unsigned char, kSymbols> &interleaveOrder()
{
    static const std::array<unsigned char, kSymbols> order = [] {
        std::array<unsigned char, kSymbols> o{};
        int p = 0;
        for (unsigned i = 0; p < kSymbols; ++i) {
            const unsigned char j = static_cast<unsigned char>(
                ((static_cast<unsigned long long>(i & 0xff) * 0x80200802ULL) & 0x0884422110ULL) *
                0x0101010101ULL >> 32);
            if (j < kSymbols) o[static_cast<size_t>(p++)] = j;
        }
        return o;
    }();
    return order;
}

std::string twoDigits(int value)
{
    char buffer[16];
    std::snprintf(buffer, sizeof buffer, "%2d", value);
    return std::string(buffer).substr(0, 2);
}

bool powerPlausible(int dbm)
{
    const int nu = dbm % 10;
    return nu == 0 || nu == 3 || nu == 7 || nu == 10;
}

} // namespace

MetricTable makeMetricTable(float bias)
{
    MetricTable table{};
    for (int i = 0; i < 256; ++i) {
        table[0][static_cast<size_t>(i)] = static_cast<int>(std::lround(10.0f * (kMetric6dB[i] - bias)));
        table[1][static_cast<size_t>(i)] = static_cast<int>(std::lround(10.0f * (kMetric6dB[255 - i] - bias)));
    }
    return table;
}

void encode(const unsigned char *data, unsigned char *symbols)
{
    uint64_t state = 0;
    for (int byte = 0; byte < kDataBytes; ++byte) {
        for (int i = 7; i >= 0; --i) {
            state = (state << 1) | ((data[byte] >> i) & 1u);
            const unsigned sym = encodeState(state);
            *symbols++ = static_cast<unsigned char>(sym >> 1);
            *symbols++ = static_cast<unsigned char>(sym & 1);
        }
    }
}

bool fano(const unsigned char *symbols, const MetricTable &mettab, int delta,
          unsigned int maxCyclesPerBit, unsigned char *data,
          unsigned int *metric, unsigned int *cycles)
{
    struct Node {
        uint64_t encstate = 0;
        long gamma = 0;
        int metrics[4] = {0, 0, 0, 0};
        int tm[2] = {0, 0};
        int i = 0;
    };
    const unsigned nbits = kDataBits;
    std::vector<Node> nodes(nbits + 1);
    Node *const first = nodes.data();
    Node *const lastnode = &nodes[nbits - 1];
    Node *const tail = &nodes[nbits - 31];

    const unsigned char *s = symbols;
    for (Node *np = first; np <= lastnode; ++np) {
        np->metrics[0] = mettab[0][s[0]] + mettab[0][s[1]];
        np->metrics[1] = mettab[0][s[0]] + mettab[1][s[1]];
        np->metrics[2] = mettab[1][s[0]] + mettab[0][s[1]];
        np->metrics[3] = mettab[1][s[0]] + mettab[1][s[1]];
        s += 2;
    }

    Node *np = first;
    np->encstate = 0;
    unsigned lsym = encodeState(np->encstate);
    int m0 = np->metrics[lsym];
    int m1 = np->metrics[3 ^ lsym];
    if (m0 > m1) {
        np->tm[0] = m0;
        np->tm[1] = m1;
    } else {
        np->tm[0] = m1;
        np->tm[1] = m0;
        np->encstate++;
    }
    np->i = 0;
    const unsigned long maxcycles = static_cast<unsigned long>(maxCyclesPerBit) * nbits;
    long t = 0;
    np->gamma = 0;

    unsigned long i = 1;
    for (; i <= maxcycles; ++i) {
        const long ngamma = np->gamma + np->tm[np->i];
        if (ngamma >= t) {
            if (np->gamma < t + delta) {
                while (ngamma >= t + delta) t += delta;
            }
            np[1].gamma = ngamma;
            np[1].encstate = np->encstate << 1;
            if (++np == lastnode + 1) break;
            lsym = encodeState(np->encstate);
            if (np >= tail) {
                np->tm[0] = np->metrics[lsym];
            } else {
                m0 = np->metrics[lsym];
                m1 = np->metrics[3 ^ lsym];
                if (m0 > m1) {
                    np->tm[0] = m0;
                    np->tm[1] = m1;
                } else {
                    np->tm[0] = m1;
                    np->tm[1] = m0;
                    np->encstate++;
                }
            }
            np->i = 0;
            continue;
        }
        for (;;) {
            if (np == first || np[-1].gamma < t) {
                t -= delta;
                if (np->i != 0) {
                    np->i = 0;
                    np->encstate ^= 1;
                }
                break;
            }
            if (--np < tail && np->i != 1) {
                np->i++;
                np->encstate ^= 1;
                break;
            }
        }
    }
    if (metric != nullptr) *metric = static_cast<unsigned int>(np->gamma);
    const Node *out = &nodes[7];
    for (unsigned b = 0; b < nbits / 8; ++b) {
        data[b] = static_cast<unsigned char>(out->encstate);
        out += 8;
    }
    // nbits/8 = 10 bytes; the 11th byte holds only tail bits.
    data[kDataBytes - 1] = 0;
    if (cycles != nullptr) *cycles = static_cast<unsigned int>(i + 1);
    return i < maxcycles;
}

void interleave(unsigned char *sym)
{
    unsigned char tmp[kSymbols];
    const auto &order = interleaveOrder();
    for (int p = 0; p < kSymbols; ++p) tmp[order[static_cast<size_t>(p)]] = sym[p];
    std::memcpy(sym, tmp, kSymbols);
}

void deinterleave(unsigned char *sym)
{
    unsigned char tmp[kSymbols];
    const auto &order = interleaveOrder();
    for (int p = 0; p < kSymbols; ++p) tmp[p] = sym[order[static_cast<size_t>(p)]];
    std::memcpy(sym, tmp, kSymbols);
}

void channelSymbolsForData(const unsigned char *data, unsigned char *symbols)
{
    unsigned char bits[kDataBytes * 8 * 2] = {};
    encode(data, bits);
    interleave(bits);
    for (int i = 0; i < kSymbols; ++i)
        symbols[i] = static_cast<unsigned char>(2 * bits[i] + kSyncVector[static_cast<size_t>(i)]);
}

int HashTable::hashOf(const std::string &call)
{
    return static_cast<int>(nhash(call.data(), call.size(), 146u) & 32767u);
}

void HashTable::insert(const std::string &call, const std::string &grid)
{
    if (call.empty()) return;
    const size_t h = static_cast<size_t>(hashOf(call));
    m_calls[h] = call.substr(0, 12);
    if (!grid.empty()) m_grids[h] = grid.substr(0, 4);
}

void HashTable::clear()
{
    for (auto &c : m_calls) c.clear();
    for (auto &g : m_grids) g.clear();
}

void unpack50(const unsigned char *dat, int32_t &n1, int32_t &n2)
{
    n1 = (dat[0] << 20) + (dat[1] << 12) + (dat[2] << 4) + ((dat[3] >> 4) & 15);
    n2 = ((dat[3] & 15) << 18) + (dat[4] << 10) + (dat[5] << 2) + ((dat[6] >> 6) & 3);
}

bool unpackCall(int32_t ncall, std::string &call)
{
    static const char c[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    if (ncall < 0 || ncall >= 262177560) return false;
    int32_t n = ncall;
    char tmp[7];
    tmp[5] = c[n % 27 + 10]; n /= 27;
    tmp[4] = c[n % 27 + 10]; n /= 27;
    tmp[3] = c[n % 27 + 10]; n /= 27;
    tmp[2] = c[n % 10];      n /= 10;
    tmp[1] = c[n % 36];      n /= 36;
    tmp[0] = c[n];
    tmp[6] = '\0';
    int i = 0;
    while (i < 5 && tmp[i] == ' ') ++i;
    // wsprd ends the call at its first space.
    std::string s(&tmp[i]);
    const size_t space = s.find(' ');
    call = space == std::string::npos ? s : s.substr(0, space);
    return true;
}

bool unpackGrid(int32_t ngrid, std::string &grid)
{
    static const char c[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    ngrid >>= 7;
    if (ngrid < 0 || ngrid >= 32400) {
        grid = "XXXX";
        return false;
    }
    const int dlat = (ngrid % 180) - 90;
    int dlong = (ngrid / 180) * 2 - 180 + 2;
    if (dlong < -180) dlong += 360;
    if (dlong > 180) dlong += 360;
    const int nlong = static_cast<int>(60.0 * (180.0 - dlong) / 5.0);
    int n1 = nlong / 240;
    int n2 = (nlong - 240 * n1) / 24;
    grid.assign(4, ' ');
    grid[0] = c[10 + n1];
    grid[2] = c[n2];
    const int nlat = static_cast<int>(60.0 * (dlat + 90) / 2.5);
    n1 = nlat / 240;
    n2 = (nlat - 240 * n1) / 24;
    grid[1] = c[10 + n1];
    grid[3] = c[n2];
    return true;
}

bool unpackPrefix(int32_t nprefix, std::string &call)
{
    const std::string base = call;
    if (nprefix < 60000) {
        char pfx[4] = {0, 0, 0, 0};
        int32_t n = nprefix;
        for (int i = 2; i >= 0; --i) {
            const int nc = n % 37;
            if (nc >= 0 && nc <= 9) pfx[i] = static_cast<char>(nc + 48);
            else if (nc >= 10 && nc <= 35) pfx[i] = static_cast<char>(nc + 55);
            else pfx[i] = ' ';
            n /= 37;
        }
        const char *p = std::strrchr(pfx, ' ');
        call = std::string(p ? p + 1 : pfx) + "/" + base;
        return true;
    }
    // wsprd keeps the suffix code in a char: emulate its 8-bit wrap.
    const int nc = static_cast<signed char>(static_cast<unsigned char>((nprefix - 60000) & 0xff));
    if (nc >= 0 && nc <= 9) {
        call = base + "/" + static_cast<char>(nc + 48);
    } else if (nc >= 10 && nc <= 35) {
        call = base + "/" + static_cast<char>(nc + 55);
    } else if (nc >= 36 && nc <= 125) {
        call = base + "/" + static_cast<char>((nc - 26) / 10 + 48) + static_cast<char>((nc - 26) % 10 + 48);
    } else {
        return false;
    }
    return true;
}

namespace {

int callCharCode(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch == ' ') return 36;
    if (ch >= 'A' && ch <= 'Z') return ch - 'A' + 10;
    return -1;
}

// wsprsim pack_call: 6-character field with the digit at position 2.
bool packCall(const std::string &call, uint32_t &n)
{
    const size_t len = call.size();
    if (len < 3 || len > 6) return false;
    std::string c6(6, ' ');
    if (call[2] >= '0' && call[2] <= '9') {
        for (size_t i = 0; i < len; ++i) c6[i] = call[i];
    } else if (call[1] >= '0' && call[1] <= '9' && len <= 5) {
        for (size_t i = 0; i < len; ++i) c6[i + 1] = call[i];
    } else {
        return false;
    }
    int code[6];
    for (int i = 0; i < 6; ++i) code[i] = callCharCode(c6[static_cast<size_t>(i)]);
    // [0]: letter/digit/space, [1]: letter/digit, [2]: digit, [3..5]: letter/space
    if (code[0] < 0 || code[1] < 0 || code[1] > 35 || code[2] < 0 || code[2] > 9) return false;
    for (int i = 3; i < 6; ++i)
        if (code[i] < 10) return false;      // digits are not allowed in the suffix
    uint32_t v = static_cast<uint32_t>(code[0]);
    v = v * 36 + static_cast<uint32_t>(code[1]);
    v = v * 10 + static_cast<uint32_t>(code[2]);
    v = v * 27 + static_cast<uint32_t>(code[3] - 10);
    v = v * 27 + static_cast<uint32_t>(code[4] - 10);
    v = v * 27 + static_cast<uint32_t>(code[5] - 10);
    n = v;
    return true;
}

bool validGrid(const std::string &g, size_t len)
{
    if (g.size() != len) return false;
    if (g[0] < 'A' || g[0] > 'R' || g[1] < 'A' || g[1] > 'R') return false;
    if (g[2] < '0' || g[2] > '9' || g[3] < '0' || g[3] > '9') return false;
    if (len == 6 && (g[4] < 'A' || g[4] > 'X' || g[5] < 'A' || g[5] > 'X')) return false;
    return true;
}

void packData(uint32_t n, uint32_t m, unsigned char *data)
{
    std::memset(data, 0, kDataBytes);
    data[0] = static_cast<unsigned char>(0xff & (n >> 20));
    data[1] = static_cast<unsigned char>(0xff & (n >> 12));
    data[2] = static_cast<unsigned char>(0xff & (n >> 4));
    data[3] = static_cast<unsigned char>(((n & 0x0f) << 4) + ((m >> 18) & 0x0f));
    data[4] = static_cast<unsigned char>(0xff & (m >> 10));
    data[5] = static_cast<unsigned char>(0xff & (m >> 2));
    data[6] = static_cast<unsigned char>((m & 0x03) << 6);
}

bool fail(std::string *error, const char *text)
{
    if (error != nullptr) *error = text;
    return false;
}

} // namespace

int validPowerDbm(int dbm)
{
    static const int nu[10] = {0, -1, 1, 0, -1, 2, 1, 0, -1, 1};
    const int p = std::max(0, std::min(60, dbm));
    return p + nu[p % 10];
}

bool pack(const std::string &rawMessage, unsigned char *data, std::string *error)
{
    std::string message;
    for (char ch : rawMessage) message += static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    std::vector<std::string> f;
    {
        size_t i = 0;
        while (i < message.size()) {
            while (i < message.size() && message[i] == ' ') ++i;
            size_t j = i;
            while (j < message.size() && message[j] != ' ') ++j;
            if (j > i) f.push_back(message.substr(i, j - i));
            i = j;
        }
    }
    if (f.size() < 2 || f.size() > 3) return fail(error, "WSPR message needs call, [grid,] power");
    for (char ch : f.back())
        if (ch < '0' || ch > '9') return fail(error, "power must be a number in dBm");
    if (f.back().size() > 2) return fail(error, "power must be 0..60 dBm");
    const int power = validPowerDbm(std::stoi(f.back()));

    if (f.size() == 3 && f[0].size() > 2 && f[0].front() == '<' && f[0].back() == '>') {
        // Type 3: hash of the (compound) call, 6-character grid.
        const std::string call = f[0].substr(1, f[0].size() - 2);
        if (call.empty() || call.size() > 12) return fail(error, "invalid hashed callsign");
        for (char ch : call)
            if (callCharCode(ch) < 0 && ch != '/') return fail(error, "invalid character in callsign");
        if (!validGrid(f[1], 6)) return fail(error, "type 3 needs a 6-character locator");
        const int ntype = -(power + 1);
        const uint32_t ihash = static_cast<uint32_t>(HashTable::hashOf(call));
        const uint32_t m = 128u * ihash + static_cast<uint32_t>(ntype + 64);
        const std::string grid6 = f[1].substr(1, 5) + f[1].substr(0, 1);
        uint32_t n = 0;
        if (!packCall(grid6, n)) return fail(error, "locator cannot be packed");
        packData(n, m, data);
        return true;
    }
    if (f.size() == 3) {
        // Type 1: plain call, 4-character grid.
        if (f[0].find('/') != std::string::npos) return fail(error, "a compound call is sent without locator (type 2)");
        uint32_t n = 0;
        if (!packCall(f[0], n)) return fail(error, "callsign does not fit the WSPR type 1 format");
        if (!validGrid(f[1], 4)) return fail(error, "type 1 needs a 4-character locator");
        const std::string &g = f[1];
        uint32_t m = static_cast<uint32_t>((179 - 10 * (g[0] - 'A') - (g[2] - '0')) * 180 + 10 * (g[1] - 'A') + (g[3] - '0'));
        m = m * 128 + static_cast<uint32_t>(power + 64);
        packData(n, m, data);
        return true;
    }
    // Type 2: compound call and power.
    const std::string &call = f[0];
    const size_t slash = call.find('/');
    if (slash == std::string::npos || slash == 0 || slash + 1 >= call.size() ||
        call.find('/', slash + 1) != std::string::npos)
        return fail(error, "type 2 needs one '/' in the callsign");
    const std::string left = call.substr(0, slash);
    const std::string right = call.substr(slash + 1);
    uint32_t n = 0;
    int ng = 0, nadd = 0;
    if (right.size() == 1) {
        if (!packCall(left, n)) return fail(error, "base call does not fit the WSPR format");
        const char c = right[0];
        int m = 0;
        if (c >= '0' && c <= '9') m = c - '0';
        else if (c >= 'A' && c <= 'Z') m = c - 'A' + 10;
        else return fail(error, "invalid suffix");
        ng = 60000 - 32768 + m;
        nadd = 1;
    } else if (right.size() == 2) {
        if (!packCall(left, n)) return fail(error, "base call does not fit the WSPR format");
        if (right[0] < '0' || right[0] > '9' || right[1] < '0' || right[1] > '9')
            return fail(error, "a two-character suffix must be two digits");
        ng = 60000 + 26 + 10 * (right[0] - '0') + (right[1] - '0');
        nadd = 1;
    } else {
        if (left.size() > 3) return fail(error, "prefix longer than 3 characters");
        if (!packCall(right, n)) return fail(error, "base call does not fit the WSPR format");
        int m = left.size() == 1 ? 37 * 36 + 36 : (left.size() == 2 ? 36 : 0);
        for (char c : left) {
            int nc = 36;
            if (c >= '0' && c <= '9') nc = c - '0';
            else if (c >= 'A' && c <= 'Z') nc = c - 'A' + 10;
            else return fail(error, "invalid prefix");
            m = 37 * m + nc;
        }
        nadd = 0;
        if (m > 32768) {
            m -= 32768;
            nadd = 1;
        }
        ng = m;
    }
    const int ntype = power + 1 + nadd;
    const uint32_t m = static_cast<uint32_t>(128 * ng + ntype + 64);
    packData(n, m, data);
    return true;
}

bool unpack(const unsigned char *data, HashTable &hashes, Message &out)
{
    out = Message();
    int32_t n1 = 0, n2 = 0;
    unpack50(data, n1, n2);
    std::string callsign;
    std::string grid;
    if (!unpackCall(n1, callsign)) return false;
    if (!unpackGrid(n2, grid)) return false;
    const int ntype = (n2 & 127) - 64;

    if (ntype >= 0 && ntype <= 62) {
        const int nu = ntype % 10;
        if (nu == 0 || nu == 3 || nu == 7) {
            out.type = 1;
            out.powerDbm = ntype;
            out.callsign = callsign;
            out.grid = grid;
            out.text = callsign + " " + grid + " " + twoDigits(ntype);
            out.plausible = true;
            hashes.insert(callsign, grid);
        } else {
            int nadd = nu;
            if (nu > 3) nadd = nu - 3;
            if (nu > 7) nadd = nu - 7;
            const int32_t n3 = n2 / 128 + 32768 * (nadd - 1);
            if (!unpackPrefix(n3, callsign)) return false;
            const int ndbm = ntype - nadd;
            out.type = 2;
            out.powerDbm = ndbm;
            out.callsign = callsign;
            out.text = callsign + " " + twoDigits(ndbm);
            out.plausible = powerPlausible(ndbm);
            if (out.plausible) hashes.insert(callsign, std::string());
        }
        return true;
    }
    if (ntype < 0) {
        const int ndbm = -(ntype + 1);
        // wsprd: grid6 = callsign[5] + callsign[0..4] of the 6-char field.
        std::string grid6;
        if (callsign.size() > 5) grid6 += callsign[5];
        grid6 += callsign.substr(0, 5);
        const auto at = [&](size_t k) { return k < grid6.size() ? static_cast<unsigned char>(grid6[k]) : 0u; };
        bool plausible = powerPlausible(ndbm) && std::isalpha(at(0)) && std::isalpha(at(1)) &&
                         std::isdigit(at(2)) && std::isdigit(at(3));
        const int ihash = (n2 - ntype - 64) / 128;
        const std::string &known = hashes.callAt(ihash);
        out.type = 3;
        out.powerDbm = ndbm;
        out.grid = grid6;
        out.callsign = known;
        const std::string shown = known.empty() ? std::string("<...>") : "<" + known + ">";
        out.text = shown + " " + grid6 + " " + twoDigits(ndbm);
        if (ntype == -64) plausible = false;
        out.plausible = plausible;
        return true;
    }
    // ntype 63: wsprd prints nothing useful.
    return false;
}

} // namespace M3Wspr
