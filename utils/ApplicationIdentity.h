#pragma once
#include "M3Version.h"
#include "M3BuildInfo.h"
#include "utils/BuildNumber.h"
#include "utils/InstallationPaths.h"
#include "utils/ProductName.h"
#include <QString>

namespace M3Identity {
// Distribution builds (GitHub workflow) carry M3_BUILD_NUMBER > 0;
// local builds keep it 0 and count on their own.
inline bool isLocalBuild() { return QLatin1String(M3_BUILD_NUMBER) == QLatin1String("0"); }

// "build 4" for a release, "dev 23 (9eb977b)" for a local build, so the two
// counters are never mistaken for one another.
inline QString buildLabel()
{
    const QString number = QString::fromLatin1(M3Build::number());
    if (!isLocalBuild()) return QStringLiteral("build ") + number;
    const QString revision = QString::fromLatin1(M3Build::revision());
    return revision.isEmpty() ? QStringLiteral("dev ") + number
                              : QStringLiteral("dev %1 (%2)").arg(number, revision);
}

inline QString windowTitle()
{
    // Fork name in the title bar; the version number stays MadModem's.
    // A configuration chosen with --config is named in the title bar.
    const QString config = M3Installation::activeConfig();
    const QString title = QStringLiteral("%1 (%2) " M3_VERSION_STRING " — %3")
        .arg(ProductName::longName(), ProductName::shortName(), buildLabel());
    return config.isEmpty() ? title
                            : title + QStringLiteral(" — [%1]").arg(M3Installation::configLabel(config));
}
}
