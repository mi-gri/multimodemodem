// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// The system ends the program (shutdown, log-off, UPS): the transmitter must not stay keyed. This turns the Unix
// termination signals into a call in the Qt event loop. Windows ends sessions through Qt's session manager signals,
// which main() connects itself.
#include <QObject>

#include <functional>

#ifdef Q_OS_UNIX
#include <QSocketNotifier>

#include <csignal>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace M3SystemEnd {

#ifdef Q_OS_UNIX
inline int signalSockets[2] = {-1, -1};

// Async-signal-safe: one byte wakes the event loop.
inline void onTerminationSignal(int)
{
    const char byte = 1;
    const ssize_t written = ::write(signalSockets[1], &byte, 1);
    (void)written;
}
#endif

// Calls @p quit once, in the thread of @p context, on SIGTERM, SIGINT or SIGHUP. The handlers are one-shot: a repeated
// signal ends a program that hangs. A signal that is ignored (nohup) stays ignored.
inline bool watchTerminationSignals(QObject *context, std::function<void()> quit)
{
#ifdef Q_OS_UNIX
    if (signalSockets[0] >= 0 || ::socketpair(AF_UNIX, SOCK_STREAM, 0, signalSockets) != 0) return false;
    auto *notifier = new QSocketNotifier(signalSockets[0], QSocketNotifier::Read, context);
    QObject::connect(notifier, &QSocketNotifier::activated, context, [notifier, quit] {
        notifier->setEnabled(false);
        char byte = 0;
        const ssize_t got = ::read(signalSockets[0], &byte, 1);
        (void)got;
        quit();
    });
    for (const int number : {SIGTERM, SIGINT, SIGHUP}) {
        struct sigaction previous = {};
        if (::sigaction(number, nullptr, &previous) != 0 || previous.sa_handler == SIG_IGN) continue;
        struct sigaction action = {};
        action.sa_handler = onTerminationSignal;
        sigemptyset(&action.sa_mask);
        action.sa_flags = SA_RESTART | static_cast<int>(SA_RESETHAND);
        ::sigaction(number, &action, nullptr);
    }
    return true;
#else
    Q_UNUSED(context)
    Q_UNUSED(quit)
    return false;
#endif
}

} // namespace M3SystemEnd
