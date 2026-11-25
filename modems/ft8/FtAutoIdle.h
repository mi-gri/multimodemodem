// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// Idle clock of the FT Auto mode: when no QSO was completed (and none is running) for the set time, the stage
// pauses itself, so a quiet band can not keep the transmitter calling CQ for hours. Pure logic.
#include <QtGlobal>

namespace M3 {

class FtAutoIdle
{
public:
    // Auto mode switched on, stage selected again, a QSO completed or running, the operator transmitted.
    void restart(qint64 nowMs) { m_sinceMs = nowMs; }
    void stop() { m_sinceMs = 0; }
    bool running() const { return m_sinceMs > 0; }

    // minutes = 0 never expires.
    bool expired(qint64 nowMs, int minutes) const
    {
        return minutes > 0 && m_sinceMs > 0 && nowMs - m_sinceMs >= static_cast<qint64>(minutes) * 60000;
    }

    // Whole minutes left (rounded up), 0 when expired or not running.
    int remainingMinutes(qint64 nowMs, int minutes) const
    {
        if (minutes <= 0 || m_sinceMs <= 0) return 0;
        const qint64 left = static_cast<qint64>(minutes) * 60000 - (nowMs - m_sinceMs);
        return left <= 0 ? 0 : static_cast<int>((left + 59999) / 60000);
    }

private:
    qint64 m_sinceMs = 0;
};

} // namespace M3
