# GitHub distribution — MultiModeModem 0.6.6

Copy the complete source directory contents into the repository root, including
`.github`. To clean up documents left after overlaying an older source tree,
optionally run `python3 scripts/clean_legacy_docs.py --apply` before committing.
The script removes only explicitly listed retired documents and preserves current
manuals, source code, upstream notices and unrelated files. Replacing files alone
does not remove old tracked documents from GitHub. Documentation cleanup is a
repository maintenance operation and does not block compilation or packaging.

**Build installers** runs on main/master pushes, version tags and manual dispatch.
It builds Windows UCRT64, Ubuntu 24.04, Fedora 43 and both macOS architectures.
After all jobs succeed, it publishes portable archives, EXE/RUN/DEB/RPM/PKG
installers and SHA256SUMS. Native Linux packages are the same payloads wrapped by RUN.

A branch build creates a numbered prerelease (`build-0.6.6-N`). Tag `v0.6.6` creates
a version release. The tag must match all source metadata; the source-check job
rejects mixed versions. Run the workflow on the new commit, not an older run.
Only the publish job requests `contents: write`; the normal GitHub token is used.
Repository policy must allow release writes. Immutable releases require a new version.

`github.run_number` becomes the compiled build number on every platform; the source
commit is embedded too. Updating a release preserves operator notes and refreshes
its machine-readable build/commit identity. Build numbers are not reset per version.

The separate **Full regression suite (manual)** runs the extended Qt5/Qt6 checks.
Native installer/runtime checks belong to the distribution jobs. Signing with
Windows/Apple publisher credentials is not configured.

## Update signing

The publish job signs `SHA256SUMS.txt` with an Ed25519 key and uploads the raw
64-byte signature as `SHA256SUMS.txt.sig`. m3 installs an update only when that
signature verifies against a public key compiled into the program
(`updates/UpdateSigningKeys.h`); otherwise it only opens the release page. A
release copied or altered without the private key therefore cannot be installed
through the updater.

- **Secret:** repository secret `M3_UPDATE_SIGNING_KEY` holds the private key in
  PEM form. Without it, or with a key whose public half is not listed in
  `UpdateSigningKeys.h`, the publish job stops before anything is uploaded.
- **Create a key:** `openssl genpkey -algorithm ed25519 -out key.pem`; the public
  key in hex is `openssl pkey -in key.pem -pubout -outform DER | tail -c 32 | od -An -tx1 | tr -d ' \n'`.
  Keep an offline backup of the private key: losing it means clients only accept
  releases after a manual installation of a build with a new key.
- **Rotate:** add the new public key, publish one release signed with the old
  key, then switch the secret to the new key and remove the old public key.
- **Check by hand:** `openssl pkeyutl -verify -pubin -inkey pub.pem -rawin -in SHA256SUMS.txt -sigfile SHA256SUMS.txt.sig`.

Versions built before this check do not verify the signature; the protection
applies once a signing-aware m3 is installed.

## Licenses of bundled libraries

MultiModeModem is GPL-3.0-or-later (`LICENSE.md`). Every published package must
let users obtain the corresponding source (GPLv3 section 6). The m³ source is
the Git tag or commit embedded in the build (`--build-info`); the GitHub
release links it. The packages also bundle libraries built by others:

| Package | Bundled libraries | Source of the exact versions |
|---|---|---|
| Windows (MSYS2 UCRT64) | Qt 5 (LGPL-3.0), FFTW3 (GPL-2.0+), OpenSSL 3 (Apache-2.0), libusb (LGPL-2.1+), zlib, ANGLE (BSD-3), GCC runtime (GPL-3.0 with runtime exception), winpthreads (MIT) | MSYS2 package sources: https://github.com/msys2/MINGW-packages and the source tarballs at https://repo.msys2.org/mingw/sources/ |
| Windows, Linux | Hamlib (LGPL-2.1+), linked statically | built from `third_party/hamlib_lgpl/source` in this repository |
| macOS (Homebrew) | Qt 6 (LGPL-3.0), Hamlib (LGPL-2.1+), FFTW3 (GPL-2.0+) | Homebrew formulae name the upstream source URL and checksum: https://github.com/Homebrew/homebrew-core |
| Linux portable / RUN | Qt 5, FFTW3 and their dependencies from Ubuntu 24.04 or Fedora 43 | `apt-get source` / `dnf download --source` for the versions of that distribution |

Qt is linked dynamically, so users can replace it (LGPL). Hamlib is linked
statically on Windows and Linux; its complete source and build script are in
this repository, so the program can be rebuilt against a changed Hamlib. The
libraries' license files stay next to them where the deployment tools copy
them; keep them when repackaging. When publishing a release, note the MSYS2,
Homebrew and distribution snapshot (date) in the release text, so the exact
versions can be found later. This section is a summary, not legal advice.

## Distribution repositories

GitHub DEB/RPM downloads support direct installation. PPA Launchpad and Fedora Copr
are options for additional update repositories. Official Debian/Fedora inclusion
requires rebuildable source packages, packaging review and ongoing maintenance;
a binary DEB or RPM alone is not a submission. Source packaging for those repositories
is separate work. No external repository is configured by these sources.
