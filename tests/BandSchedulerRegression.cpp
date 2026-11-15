// QSY plan (band scheduler): file format, one-time entries, order, all modes.
#include "dialogs/BandSchedulerDialog.h"
#include "utils/StandardFrequencies.h"

#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <cstdio>

static int g_fails = 0;
static void check(bool ok, const char *name)
{
    if (!ok) ++g_fails;
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", name);
}

int main(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc, argv);

    // Every mode the plan offers maps to a frequency-list mode, so it can run.
    bool allModes = true;
    for (const QString &mode : BandSchedulerDialog::defaultModes()) {
        if (M3StandardFrequencies::listModeForName(mode).isEmpty()) {
            allModes = false;
            std::printf("  no list mode for %s\n", qPrintable(mode));
        }
    }
    check(allModes, "every plan mode has a frequency-list mode");
    check(BandSchedulerDialog::defaultModes().contains(QStringLiteral("WSPR")), "WSPR can be planned");

    ScheduledQsyEntry daily;
    daily.timeUtc = QTime(14, 15);
    daily.mode = QStringLiteral("FT8");
    daily.band = QStringLiteral("15m");
    daily.frequencyHz = 21074000.0;
    ScheduledQsyEntry once = daily;
    once.timeUtc = QTime(9, 0);
    once.mode = QStringLiteral("RTTY Text");
    once.band = QStringLiteral("30m");
    once.frequencyHz = 10098450.0;
    once.dateUtc = QDate(2026, 11, 15);

    const QString json = BandSchedulerDialog::serializeEntries({daily, once});
    const QList<ScheduledQsyEntry> back = BandSchedulerDialog::deserializeEntries(json);
    check(back.size() == 2, "round trip keeps both entries");
    check(back.size() == 2 && !back.at(0).dateUtc.isValid() && back.at(1).dateUtc == QDate(2026, 11, 15),
          "date survives the round trip; no date = daily");
    check(!json.contains(QStringLiteral("\"date_utc\":\"\"")) &&
          QJsonDocument::fromJson(json.toUtf8()).array().at(0).toObject().value(QStringLiteral("date_utc")).isUndefined(),
          "daily entries store no date (old files stay readable)");

    check(once.runsOn(QDate(2026, 11, 15)) && !once.runsOn(QDate(2026, 11, 16)), "dated entry runs only on its date");
    check(daily.runsOn(QDate(2026, 11, 15)) && daily.runsOn(QDate(2027, 1, 1)), "undated entry runs every day");

    // A broken date must drop the entry, never turn it into a daily one.
    const QList<ScheduledQsyEntry> broken = BandSchedulerDialog::deserializeEntries(QStringLiteral(
        "[{\"time_utc\":\"10:00\",\"date_utc\":\"2026-13-40\",\"mode\":\"CW Morse\",\"band\":\"40m\",\"frequency_hz\":7030000},"
        "{\"time_utc\":\"25:99\",\"mode\":\"FT8\",\"frequency_hz\":7074000},"
        "{\"time_utc\":\"11:00\",\"mode\":\"WSPR\",\"band\":\"80m\",\"frequency_hz\":0},"
        "{\"time_utc\":\"12:00\",\"mode\":\"WSPR\",\"band\":\"80m\",\"frequency_hz\":3568600}]"));
    check(broken.size() == 1 && broken.at(0).mode == QStringLiteral("WSPR"), "broken date, time or frequency is dropped");
    check(BandSchedulerDialog::deserializeEntries(QStringLiteral("{not json")).isEmpty(), "invalid file gives no entries");

    // Order in the dialog: daily entries first, then dated ones by date and time.
    ScheduledQsyEntry later = once;
    later.timeUtc = QTime(8, 0);
    later.dateUtc = QDate(2026, 11, 16);
    BandSchedulerDialog dialog({later, once, daily}, nullptr);
    const QList<ScheduledQsyEntry> sorted = dialog.entries();
    check(sorted.size() == 3 && !sorted.at(0).dateUtc.isValid() && sorted.at(1).dateUtc == QDate(2026, 11, 15) &&
          sorted.at(2).dateUtc == QDate(2026, 11, 16), "daily first, then by date");

    std::printf("%s\n", g_fails ? "RESULT FAIL" : "RESULT PASS");
    return g_fails;
}
