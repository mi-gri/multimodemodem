#include "../modems/ft8/FtAutoIdle.h"
#include <cstdio>
#include <cstdlib>
using M3::FtAutoIdle;
static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)
int main()
{
    const qint64 min = 60000;
    FtAutoIdle idle;
    CHECK(!idle.running() && !idle.expired(1000 * min, 30));          // never started: never expired
    idle.restart(100 * min);
    CHECK(idle.running() && !idle.expired(100 * min, 30));
    CHECK(!idle.expired(129 * min + 59000, 30));
    CHECK(idle.expired(130 * min, 30));
    CHECK(!idle.expired(1000 * min, 0));                              // 0 = never
    CHECK(idle.remainingMinutes(100 * min, 30) == 30);
    CHECK(idle.remainingMinutes(100 * min + 1, 30) == 30);
    CHECK(idle.remainingMinutes(129 * min + 30000, 30) == 1);
    CHECK(idle.remainingMinutes(130 * min, 30) == 0 && idle.remainingMinutes(100 * min, 0) == 0);
    idle.restart(125 * min);                                           // a QSO completed
    CHECK(!idle.expired(130 * min, 30) && idle.expired(155 * min, 30));
    idle.stop();
    CHECK(!idle.running() && !idle.expired(1000 * min, 30));
    if (failures == 0) std::puts("FtAutoIdle regression OK");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
