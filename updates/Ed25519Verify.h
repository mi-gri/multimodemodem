// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QByteArray>

/**
 * @brief Ed25519 signature verification (RFC 8032, pure Ed25519).
 *
 * Verification only, ported from TweetNaCl (public domain, Bernstein et al.)
 * with SHA-512 from QCryptographicHash.  Unlike TweetNaCl, a signature whose
 * scalar S is not below the group order is refused (RFC 8032, 5.1.7), so a
 * signature cannot be altered into a second valid one.  Used for the update
 * checksum list; no secret keys are handled, so timing is not a concern.
 */
namespace M3Ed25519 {
/// @p publicKey 32 bytes, @p signature 64 bytes (R || S); anything else fails.
bool verify(const QByteArray &signature, const QByteArray &message, const QByteArray &publicKey);
}
