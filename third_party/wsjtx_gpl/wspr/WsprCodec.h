// SPDX-License-Identifier: GPL-3.0-or-later
//
// WSPR channel code for m³: K=32 r=1/2 convolutional code with Fano
// sequential decoding, interleaving and the 50-bit message format.
//
// C++ port of WSJT-X lib/wsprd (fano.c, tab.c, wsprd_utils.c,
// wsprsim_utils.c), Copyright 1994 Phil Karn KA9Q, 2001-2018 Joe Taylor K1JT,
// 2014-2018 Steven Franke K9AN, GNU GPL v3.  File I/O, globals and
// fixed-size string handling were replaced; the algorithms are unchanged.
#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace M3Wspr {

constexpr int kSymbols = 162;
constexpr int kDataBits = 81;           // 50 message bits + 31 tail bits
constexpr int kDataBytes = 11;

// Sync vector (one bit per channel symbol).
extern const std::array<unsigned char, kSymbols> kSyncVector;

// Fano metric table [sent bit][received soft symbol 0..255] for @p bias.
using MetricTable = std::array<std::array<int, 256>, 2>;
MetricTable makeMetricTable(float bias = 0.45f);

// Convolutional encoder: @p data (kDataBytes) -> 2*8*kDataBytes bits.
void encode(const unsigned char *data, unsigned char *symbols);

// Fano decoder on deinterleaved soft symbols (0..255, 128 = erasure).
// Returns true on success; @p metric and @p cycles as in wsprd.
bool fano(const unsigned char *symbols, const MetricTable &mettab, int delta,
          unsigned int maxCyclesPerBit, unsigned char *data,
          unsigned int *metric, unsigned int *cycles);

void interleave(unsigned char *sym);
void deinterleave(unsigned char *sym);

// Channel symbols (0..3) for 11 decoded data bytes: encode, interleave,
// add the sync vector.  Used to subtract a decoded signal.
void channelSymbolsForData(const unsigned char *data, unsigned char *symbols);

// Callsign hash table of type 1/2 messages, used by type 3 (<hashed> call).
class HashTable
{
public:
    static int hashOf(const std::string &call);
    void insert(const std::string &call, const std::string &grid);
    const std::string &callAt(int hash) const { return m_calls[static_cast<size_t>(hash & 32767)]; }
    const std::string &gridAt(int hash) const { return m_grids[static_cast<size_t>(hash & 32767)]; }
    void clear();

private:
    std::array<std::string, 32768> m_calls;
    std::array<std::string, 32768> m_grids;
};

struct Message
{
    std::string text;        // "K1ABC FN42 37", "PJ4/K1ABC 37", "<K1ABC> FN42AX 37"
    std::string callsign;    // without hash brackets when known
    std::string grid;        // 4 or 6 characters, empty for type 2
    int powerDbm = 0;
    int type = 0;            // 1, 2, 3
    bool plausible = false;  // power and grid pass wsprd's sanity checks
};

// Unpacks 11 data bytes; updates @p hashes for type 1/2 messages.
// Returns false for undecodable bit patterns.
bool unpack(const unsigned char *data, HashTable &hashes, Message &out);

// Raw fields of the 50 message bits (n1: callsign, n2: grid/power).
void unpack50(const unsigned char *data, int32_t &n1, int32_t &n2);
bool unpackCall(int32_t ncall, std::string &call);
bool unpackGrid(int32_t ngrid, std::string &grid);
bool unpackPrefix(int32_t nprefix, std::string &call);

} // namespace M3Wspr
