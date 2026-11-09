#pragma once

// The build number shown in the title bar and the logs.
// Distribution builds (CI) use M3_BUILD_NUMBER from the environment.
// Local builds count up by themselves: every build after a source change gets
// the next number (cmake/M3BuildCounter.cmake, counter in the build dir).
// The update check keeps using the official M3_BUILD_NUMBER macro, so a
// local count never looks like a newer or older published release.
namespace M3Build {
const char *number();
// Short Git commit of a local build ("+" when tracked files were modified);
// empty for distribution builds and when Git is not available.
const char *revision();
}
