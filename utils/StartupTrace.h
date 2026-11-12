// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// Startup timing for measurements: with M3_STARTUP_TRACE=1 every mark prints
// "m3-startup <ms since process start> <stage>" to stderr.  Without the
// variable a mark costs one cached boolean test.

#include <QByteArray>
#include <QElapsedTimer>
#include <cstdio>

namespace M3StartupTrace {

inline QElapsedTimer &clock()
{
    static QElapsedTimer timer = [] { QElapsedTimer t; t.start(); return t; }();
    return timer;
}

inline bool enabled()
{
    static const bool on = qgetenv("M3_STARTUP_TRACE") == "1";
    return on;
}

inline void mark(const char *stage)
{
    if (!enabled()) return;
    std::fprintf(stderr, "m3-startup %8.1f %s\n", double(clock().nsecsElapsed()) / 1e6, stage);
    std::fflush(stderr);
}

} // namespace M3StartupTrace
