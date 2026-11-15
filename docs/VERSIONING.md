# Version identity — MultiModeModem 0.6.7

`M3_VERSION.txt`, `M3Version.h`, the CMake project version, Windows
resources, the UI default title and six Qt Help projects carry the public version.
`scripts/ci_release_version_guard.sh` validates them together before packaging.

CMake generates `M3BuildInfo.h` from the distribution workflow number and
commit. The main window displays `MultiModeModem 0.6.7 — build N`, including after a
language change. Local builds show `dev N (commit)`: N counts builds after a source
change (`cmake/M3BuildCounter.cmake`, counter in the build folder) and starts again at
1 when `M3_VERSION.txt` names a new version. `--version` remains numeric for installer
checks; `--build-info` additionally reports build and commit.

The source revision identifies the source package, independently of the public
version and GitHub distribution build number. Never invent a GitHub build number
for a source-only ZIP. See [distribution](DISTRIBUTION.md).
