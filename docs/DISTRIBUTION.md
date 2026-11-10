# GitHub distribution — MultiModeModem 0.6.5

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

A branch build creates a numbered prerelease (`build-0.6.5-N`). Tag `v0.6.5` creates
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

## Distribution repositories

GitHub DEB/RPM downloads support direct installation. PPA Launchpad and Fedora Copr
are options for additional update repositories. Official Debian/Fedora inclusion
requires rebuildable source packages, packaging review and ongoing maintenance;
a binary DEB or RPM alone is not a submission. Source packaging for those repositories
is separate work. No external repository is configured by these sources.
