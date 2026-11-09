// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef JS8LDPC_H
#define JS8LDPC_H

#include <array>
#include <cstdint>
#include <QString>

/**
 * @brief LDPC(174,87) code, CRC12 and frame layout of JS8.
 *
 * C++ port of encode174.f90, bpdecode174.f90, osd174.f90, chkcrc12a.f90 and
 * the bit packing of genjs8.f90 / extractmessage174.f90 of JS8Call 2.2.0.
 * A frame carries 12 characters of the 64-symbol alphabet (72 bits) and the
 * three transmission-type bits (first/last/data).
 */
namespace Js8Ldpc {

constexpr int N = 174;
constexpr int K = 87;
constexpr int M = N - K;

using Message = std::array<std::uint8_t, K>;     // 75 data bits + 12 CRC bits
using Codeword = std::array<std::uint8_t, N>;
using Llr = std::array<float, N>;

/// "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-+/?."
const QString &alphabet();

/// CRC12 (poly 0xc06, augmented, xor 42) over the first 75 bits.
int crc12(const Message &bits);
bool crcOk(const Message &bits);

/// Builds the 87 message bits; false if frame is not 12 alphabet characters.
bool packFrame(const QString &frame, int i3bits, Message *out);
/// Returns the 12 characters and the three type bits of decoded bits.
QString unpackFrame(const Message &bits, int *i3bits);

Codeword encode(const Message &message);

/// Belief propagation; returns hard errors (>= 0) or -1 when it fails.
int bpDecode(const Llr &llr, int maxIterations, Message *decoded, Codeword *cw);
/// Ordered-statistics decoding with depth 1..5 as in osd174.f90.
int osdDecode(const Llr &llr, int depth, Message *decoded, Codeword *cw, float *dmin);

} // namespace Js8Ldpc

#endif // JS8LDPC_H
