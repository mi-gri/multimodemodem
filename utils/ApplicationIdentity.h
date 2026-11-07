#pragma once
#include "MadModemVersion.h"
#include "MadModemBuildInfo.h"
#include "utils/BuildNumber.h"
#include "utils/ProductName.h"
#include <QString>

namespace MadModemIdentity {
// Distribution builds (GitHub workflow) carry MADMODEM_BUILD_NUMBER > 0;
// local builds keep it 0 and count on their own.
inline bool isLocalBuild() { return QLatin1String(MADMODEM_BUILD_NUMBER) == QLatin1String("0"); }

// "build 4" for a release, "dev 23 (9eb977b)" for a local build, so the two
// counters are never mistaken for one another.
inline QString buildLabel()
{
    const QString number = QString::fromLatin1(MadModemBuild::number());
    if (!isLocalBuild()) return QStringLiteral("build ") + number;
    const QString revision = QString::fromLatin1(MadModemBuild::revision());
    return revision.isEmpty() ? QStringLiteral("dev ") + number
                              : QStringLiteral("dev %1 (%2)").arg(number, revision);
}

inline QString windowTitle()
{
    // Fork name in the title bar; the version number stays MadModem's.
    return QStringLiteral("%1 (%2) " MADMODEM_VERSION_STRING " — %3")
        .arg(ProductName::longName(), ProductName::shortName(), buildLabel());
}
}
