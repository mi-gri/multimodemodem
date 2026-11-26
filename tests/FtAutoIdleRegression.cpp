#include "../modems/ft8/FtAutoIdle.h"
#include "../modems/ft8/FtWaitList.h"
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
    for (const char *good : {"DL1ABC", "9A1A", "2E0XYZ", "GB100RSGB", "DL/G4ABC", "DL1ABC/P", "K1A"})
        if (!M3::isWaitListCall(QString::fromLatin1(good))) { std::fprintf(stderr, "FAIL wait list refuses %s\n", good); ++failures; }
    for (const char *bad : {"", "P", "M", "DL1", "QRP", "123", "DL*ABC", "A/P"})
        if (M3::isWaitListCall(QString::fromLatin1(bad))) { std::fprintf(stderr, "FAIL wait list accepts %s\n", bad); ++failures; }
    if (failures == 0) std::puts("FtAutoIdle regression OK");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
