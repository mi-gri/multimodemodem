# Version identity — MultiModeModem 0.26.12

Scheme since November 2026: **0.YY.MM** — year (two digits) and month, e.g. 0.26.12 for
November 2026, 0.27.1 for January 2027. Three parts because macOS allows only three in the
visible version. Releases within one month share the version and differ by the build number
(GitHub distribution run) or, locally, the dev number; the update check compares the build
number when the versions are equal.

`M3_VERSION.txt`, `M3Version.h`, the CMake project version, Windows
resources, the UI default title and six Qt Help projects carry the public version.
`scripts/ci_release_version_guard.sh` validates them together before packaging.

CMake generates `M3BuildInfo.h` from the distribution workflow number and
commit. The main window displays `MultiModeModem 0.26.12 — build N`, including after a
language change. Local builds show `dev N (commit)`: N counts builds after a source
change (`cmake/M3BuildCounter.cmake`, counter in the build folder) and starts again at
1 when `M3_VERSION.txt` names a new version. `--version` remains numeric for installer
checks; `--build-info` additionally reports build and commit.

The source revision identifies the source package, independently of the public
version and GitHub distribution build number. Never invent a GitHub build number
for a source-only ZIP. See [distribution](DISTRIBUTION.md).
