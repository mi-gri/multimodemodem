#include "../modems/ft8/FtDoubleMessage.h"
#include <cstdio>
#include <cstdlib>
using namespace M3::FtDoubleMessage;
static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)
int main()
{
    CHECK(isStandardCall("DK1ABC") && isStandardCall("W9XYZ") && isStandardCall("K1ABC") && isStandardCall("9A1AA") && isStandardCall("PT7AA"));
    CHECK(!isStandardCall("DL75WAU") && !isStandardCall("DL1ABC/P") && !isStandardCall("<DK1ABC>") && !isStandardCall("SP1234KK") && !isStandardCall(""));
    CHECK(fieldReportDb(-12) == -12 && fieldReportDb(-3) == -4 && fieldReportDb(3) == 4 && fieldReportDb(-40) == -30 && fieldReportDb(40) == 30 && fieldReportDb(0) == 0);
    CHECK(reportText(-4) == "-04" && reportText(2) == "+02" && reportText(0) == "+00" && reportText(-30) == "-30");
    CHECK(build("DK1ABC", "DL2XYZ", "DL75WAU", -12) == "DK1ABC RR73; DL2XYZ <DL75WAU> -12");
    CHECK(build("dk1abc", "dl2xyz", "do3box", 3) == "DK1ABC RR73; DL2XYZ <DO3BOX> +04");
    CHECK(build("DK1ABC", "DK1ABC", "DL75WAU", -1).isEmpty());          // the same station twice
    CHECK(build("DK1ABC", "DL75WAU", "DO3BOX", -1).isEmpty());          // a call the field cannot carry
    CHECK(build("DK1ABC", "DL2XYZ", "", -1).isEmpty());
    CHECK(sameParity(15000 * 10, 15000 * 12, 15000) && !sameParity(15000 * 10, 15000 * 11, 15000) && !sameParity(0, 15000, 15000));
    if (failures == 0) std::puts("FtDoubleMessage regression OK");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
