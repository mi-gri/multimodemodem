// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// Ed25519 public keys (32 bytes, hex) that may sign SHA256SUMS.txt of a
// release.  The updater installs a download only when SHA256SUMS.txt.sig
// verifies against one of them.  To rotate, add the new key here, ship one
// release signed with the old key, then sign with the new key and remove the
// old one.  scripts/publish_github_release.sh refuses to publish when the
// signing key's public half is not listed here.
namespace M3UpdateSigning {
inline constexpr const char *kPublicKeys[] = {
    "59880fafce912a0bec54e79fd91822b71d038303b93dadd5a4df449cbbd30c0e",   // m3 release key, 2026-11-10
};
}
