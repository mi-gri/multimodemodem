#include "../modems/ft8/FtAutoPlanner.h"
#include <cstdio>
#include <cstdlib>

using M3::FtAutoPlanner;
using Phase = FtAutoPlanner::Phase;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

static FtAutoPlanner make(int stage, int n = 3, int m = 2)
{
    FtAutoPlanner p;
    p.configure({stage, n, m});
    return p;
}

int main()
{
    { // 1: always answering, never CQ
        auto p = make(1);
        CHECK(p.phase() == Phase::Answer);
        for (int i = 0; i < 50; ++i) CHECK(!p.slotPassed(false, false));
        CHECK(!p.qsoCompleted());
        CHECK(!p.cqSeriesUnanswered());
        CHECK(p.phase() == Phase::Answer);
    }
    { // 2: always CQ
        auto p = make(2);
        CHECK(p.phase() == Phase::Cq);
        CHECK(!p.qsoCompleted());
        CHECK(!p.cqSeriesUnanswered());
        CHECK(!p.slotPassed(false, false));
        CHECK(p.phase() == Phase::Cq);
    }
    { // 3: alternate after N QSOs
        auto p = make(3, 2, 5);
        CHECK(p.phase() == Phase::Answer);
        CHECK(!p.qsoCompleted());
        CHECK(p.qsoCompleted());               // 2 -> CQ
        CHECK(p.phase() == Phase::Cq);
        CHECK(!p.qsoCompleted());
        CHECK(p.qsoCompleted());               // 2 -> Answer
        CHECK(p.phase() == Phase::Answer);
        // fallbacks: idle answering and an unanswered CQ series leave the phase
        CHECK(!p.slotPassed(false, false) && !p.slotPassed(false, false) && !p.slotPassed(false, false) && !p.slotPassed(false, false));
        CHECK(p.slotPassed(false, false));      // 5th idle slot
        CHECK(p.phase() == Phase::Cq);
        CHECK(p.cqSeriesUnanswered());
        CHECK(p.phase() == Phase::Answer);
    }
    { // 4: answer, CQ only without a target
        auto p = make(4, 3, 2);
        CHECK(p.phase() == Phase::Answer);
        CHECK(!p.slotPassed(false, false));
        CHECK(!p.slotPassed(true, false));      // a target resets the count
        CHECK(!p.slotPassed(false, false));
        CHECK(!p.slotPassed(false, true));      // busy does not count and resets
        CHECK(!p.slotPassed(false, false));
        CHECK(p.slotPassed(false, false));      // 2 idle slots -> CQ
        CHECK(p.phase() == Phase::Cq);
        CHECK(p.qsoCompleted());                // answered CQ -> back to answering
        CHECK(p.phase() == Phase::Answer);
        CHECK(p.slotPassed(false, false) == false);
        CHECK(p.slotPassed(false, false));
        CHECK(p.cqSeriesUnanswered());
        CHECK(p.phase() == Phase::Answer);
        CHECK(!p.slotPassed(true, false));
    }
    { // limits
        FtAutoPlanner p;
        p.configure({9, 0, 99});
        CHECK(p.stage() == 4);
        p.configure({0, 0, 0});
        CHECK(p.stage() == 1);
    }
    if (failures == 0) std::puts("FtAutoPlanner regression OK");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
