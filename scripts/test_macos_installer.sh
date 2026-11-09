#!/bin/bash
# Destructive fixtures are restricted to disposable GitHub macOS runners.
set -euo pipefail
[[ "${GITHUB_ACTIONS:-}" == true && "${RUNNER_OS:-}" == macOS ]]
root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
arch="${M3_OSX_ARCHITECTURES:?}"
version="$(tr -d '\r\n' < "$root/M3_VERSION.txt")"
package="$root/dist/macos/MultiModeModem-$version-macOS-$arch-unsigned.pkg"
app=/Applications/MultiModeModem.app
trap 'echo "macOS installer verification failed at line $LINENO" >&2; pkgutil --pkg-info de.migri.multimodemodem || true; ls -ld "$app" "$app/Contents/MacOS" "$app/Contents/MacOS/managed-install.flag" 2>/dev/null || true' ERR
test -f "$package"
# A pre-existing app would conceal update-only packaging bugs.
if [[ -e "$app" ]]; then
    echo 'ERROR: fresh-install test requires a clean runner without /Applications/MultiModeModem.app' >&2
    exit 1
fi
sudo installer -pkg "$package" -target /
test -x "$app/Contents/MacOS/MultiModeModem"
test -L "$app/Contents/MacOS/managed-install.flag"
test -f "$app/Contents/Resources/managed-install.flag"
codesign --verify --deep --strict "$app"
mkdir -p "$HOME/Library/Application Support/MultiModeModem"
printf 'preserve user data\n' > "$HOME/Library/Application Support/MultiModeModem/installer-test.txt"
# Simulate data from an old portable app stored in /Applications.
printf 'preserve portable settings\n' | sudo tee "$app/Contents/MacOS/settings.mad" >/dev/null
printf 'preserve portable logbook\n' | sudo tee "$app/Contents/MacOS/logbook.adi" >/dev/null
sudo touch "$app/Contents/obsolete-program-file"
sudo installer -pkg "$package" -target /
test -L "$app/Contents/MacOS/managed-install.flag"
test -f "$app/Contents/Resources/managed-install.flag"
test ! -e "$app/Contents/obsolete-program-file"
grep -qx 'preserve user data' "$HOME/Library/Application Support/MultiModeModem/installer-test.txt"
backups='/Library/Application Support/MultiModeModem/Portable Backups'
found=0
for backup in "$backups"/*; do
    if [[ -f "$backup/settings.mad" && -f "$backup/logbook.adi" ]]; then
        grep -qx 'preserve portable settings' "$backup/settings.mad"
        grep -qx 'preserve portable logbook' "$backup/logbook.adi"
        found=1
    fi
done
[[ $found -eq 1 ]]
codesign --verify --deep --strict "$app"
# Test the deployed native backend, not the build toolchain's offscreen plugin.
# --version constructs QApplication too, so it must load a platform plugin.
test -f "$app/Contents/PlugIns/platforms/libqcocoa.dylib"
installed_version="$(env -u QT_PLUGIN_PATH -u QT_QPA_PLATFORM_PLUGIN_PATH \
    QT_QPA_PLATFORM=cocoa "$app/Contents/MacOS/MultiModeModem" --version)"
printf '%s\n' "$installed_version"
[[ "$installed_version" == "MultiModeModem $version (based on MadModem)" ]]
echo 'PASS macOS fresh install, reinstall, stale-file removal, portable data preservation and signed-bundle integrity'
