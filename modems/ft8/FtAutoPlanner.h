// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// Auto mode stages of the FT8/FT4 automation. The planner only decides WHAT is
// due next (answer other stations' CQs or call CQ); whether anything may be
// transmitted at all stays with the gates of MainWindow (autoTxPermitted, STOP
// latch, block lists ...). Pure logic, no Qt, no I/O.
//
//   1 Respond      only answer CQ / RR73 / 73 of other stations
//   2 CallCq       only call CQ and work the stations that answer
//   3 Alternate    answer N QSOs, then call CQ until N QSOs, and so on
//   4 CqIfNoTarget answer; call CQ only when no target was there for M slots
//
// Stages 3 and 4 also leave a phase that produces nothing: the answering phase
// after M slots without a target, the CQ phase when a whole CQ series stayed
// unanswered. Otherwise a quiet band would stall the automation.
namespace M3 {

class FtAutoPlanner
{
public:
    enum class Phase { Answer, Cq };

    struct Config {
        int stage = 1;            // 1..4
        int alternateCount = 3;   // stage 3: completed QSOs per phase (1..20)
        int noTargetSlots = 2;    // stages 3/4: slots without a target before CQ (1..20)
    };

    void configure(const Config &config)
    {
        m_config.stage = clamp(config.stage, 1, 4);
        m_config.alternateCount = clamp(config.alternateCount, 1, 20);
        m_config.noTargetSlots = clamp(config.noTargetSlots, 1, 20);
        reset();
    }

    // Auto mode switched on, stage changed: start with the stage's first phase.
    void reset()
    {
        m_phase = m_config.stage == 2 ? Phase::Cq : Phase::Answer;
        m_qsosInPhase = 0;
        m_idleSlots = 0;
    }

    int stage() const { return m_config.stage; }
    Phase phase() const { return m_phase; }
    int qsosInPhase() const { return m_qsosInPhase; }
    int idleSlots() const { return m_idleSlots; }

    // A QSO was completed. Returns true when the phase changed.
    bool qsoCompleted()
    {
        m_idleSlots = 0;
        ++m_qsosInPhase;
        switch (m_config.stage) {
        case 3:
            if (m_qsosInPhase >= m_config.alternateCount) return flip();
            return false;
        case 4:
            if (m_phase == Phase::Cq) return enter(Phase::Answer);   // back to answering
            return false;
        default:
            return false;
        }
    }

    // A CQ series ended without any reply. Stage 2 stays in CQ (the caller
    // starts a new series); stages 3/4 go back to answering.
    bool cqSeriesUnanswered()
    {
        m_idleSlots = 0;
        if (m_config.stage == 3 || m_config.stage == 4) {
            if (m_phase == Phase::Cq) return enter(Phase::Answer);
        }
        return false;
    }

    // One RX slot passed while idle. targetWaiting: a CQ or caller could be
    // answered; busy: a QSO or transmission is running (does not count).
    bool slotPassed(bool targetWaiting, bool busy)
    {
        if (m_phase != Phase::Answer || (m_config.stage != 3 && m_config.stage != 4)) return false;
        if (busy || targetWaiting) {
            m_idleSlots = 0;
            return false;
        }
        if (++m_idleSlots >= m_config.noTargetSlots) return enter(Phase::Cq);
        return false;
    }

private:
    static int clamp(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

    bool enter(Phase phase)
    {
        const bool changed = phase != m_phase;
        m_phase = phase;
        m_qsosInPhase = 0;
        m_idleSlots = 0;
        return changed;
    }
    bool flip() { return enter(m_phase == Phase::Answer ? Phase::Cq : Phase::Answer); }

    Config m_config;
    Phase m_phase = Phase::Answer;
    int m_qsosInPhase = 0;
    int m_idleSlots = 0;
};

} // namespace M3
