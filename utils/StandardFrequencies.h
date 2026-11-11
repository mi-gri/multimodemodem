// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef M3_STANDARDFREQUENCIES_H
#define M3_STANDARDFREQUENCIES_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>

#include <algorithm>
#include <cmath>

/**
 * @brief The operator's working-frequency list (Settings -> Frequencies).
 *
 * Like the frequency list of WSJT-X/JTDX: each entry is a dial frequency
 * (USB) for a mode, valid in all IARU regions or one of them, optionally
 * marked as the preferred frequency of its band.  The band selectors (FT,
 * JS8) and the band scheduler take the preferred entry of the band.
 *
 * defaults() is taken from WSJT-X 3.0 (models/FrequencyList.cpp, the modes
 * MultiModeModem offers: FT8, FT4, MSK144, Q65) and JS8Call 2.2.0
 * (FrequencyList.cpp, JS8).  The other modes use customary activity
 * frequencies: IARU Region 1 HF band plan 2020 (CW QRP centres), the Feld
 * Hell Club list, common PSK31/RTTY/MFSK/SSTV calling frequencies and the
 * DWD (Hamburg/Pinneberg) radiofax schedule.  The list in use is stored in
 * the settings as JSON; an empty setting means the defaults.
 */
namespace M3StandardFrequencies {

struct Entry
{
    QString mode;            // "FT8", "FT4", "JS8", "MSK144", "Q65"
    double hz = 0.0;         // dial frequency
    QString region;          // "ALL", "R1", "R2", "R3"
    QString description;
    bool preferred = false;  // preferred frequency of this band and mode

    bool operator==(const Entry &o) const
    {
        return mode == o.mode && qRound64(hz) == qRound64(o.hz) && region == o.region &&
               description == o.description && preferred == o.preferred;
    }
};

inline QStringList modes()
{
    return {QStringLiteral("FT8"), QStringLiteral("FT4"), QStringLiteral("JS8"), QStringLiteral("MSK144"),
            QStringLiteral("Q65"), QStringLiteral("RTTY"), QStringLiteral("PSK"), QStringLiteral("MFSK"),
            QStringLiteral("CW"), QStringLiteral("HELL"), QStringLiteral("SSTV"), QStringLiteral("WEFAX")};
}

/// List mode of a MultiModeModem mode name ("RTTY Text" -> "RTTY",
/// "Q65-60A" -> "Q65"); empty for modes without frequencies (Radio Telescope).
inline QString listModeForName(const QString &modeName)
{
    const QString u = modeName.trimmed().toUpper();
    if (u.startsWith(QLatin1String("FT4"))) return QStringLiteral("FT4");
    if (u.startsWith(QLatin1String("FT8"))) return QStringLiteral("FT8");
    if (u.startsWith(QLatin1String("JS8"))) return QStringLiteral("JS8");
    if (u.startsWith(QLatin1String("MSK144"))) return QStringLiteral("MSK144");
    if (u.startsWith(QLatin1String("Q65"))) return QStringLiteral("Q65");
    if (u.startsWith(QLatin1String("RTTY"))) return QStringLiteral("RTTY");
    if (u.startsWith(QLatin1String("PSK"))) return QStringLiteral("PSK");
    if (u.startsWith(QLatin1String("MFSK"))) return QStringLiteral("MFSK");
    if (u.startsWith(QLatin1String("CW"))) return QStringLiteral("CW");
    if (u.contains(QLatin1String("HELL"))) return QStringLiteral("HELL");
    if (u.startsWith(QLatin1String("SSTV"))) return QStringLiteral("SSTV");
    if (u.contains(QLatin1String("WEFAX")) || u.contains(QLatin1String("FAX"))) return QStringLiteral("WEFAX");
    return QString();
}

inline QStringList regions()
{
    return {QStringLiteral("ALL"), QStringLiteral("R1"), QStringLiteral("R2"), QStringLiteral("R3")};
}

/// Amateur band of a dial frequency ("20m"); empty outside the bands.
inline QString bandOf(double hz)
{
    struct Range { double low; double high; const char *name; };
    static const Range ranges[] = {
        {1.8e6, 2.0e6, "160m"}, {3.5e6, 4.0e6, "80m"}, {5.0e6, 5.5e6, "60m"}, {7.0e6, 7.3e6, "40m"},
        {10.1e6, 10.15e6, "30m"}, {14.0e6, 14.35e6, "20m"}, {18.068e6, 18.168e6, "17m"},
        {21.0e6, 21.45e6, "15m"}, {24.89e6, 24.99e6, "12m"}, {28.0e6, 29.7e6, "10m"},
        {50.0e6, 54.0e6, "6m"}, {70.0e6, 71.0e6, "4m"}, {144.0e6, 148.0e6, "2m"},
        {222.0e6, 225.0e6, "1.25m"}, {420.0e6, 450.0e6, "70cm"}, {902.0e6, 928.0e6, "33cm"},
        {1240.0e6, 1300.0e6, "23cm"}, {2300.0e6, 2450.0e6, "13cm"},
    };
    for (const Range &r : ranges)
        if (hz >= r.low && hz <= r.high) return QString::fromLatin1(r.name);
    return QString();
}

/// Group of frequencies outside the amateur bands (weather RTTY and fax,
/// utility stations, receive only).
inline QString otherGroup() { return QStringLiteral("other"); }

/// Band of @p hz, or otherGroup() outside the amateur bands.
inline QString groupOf(double hz)
{
    const QString band = bandOf(hz);
    return band.isEmpty() ? otherGroup() : band;
}

/// Bands in frequency order, for band selectors.
inline QStringList allBands()
{
    return {QStringLiteral("160m"), QStringLiteral("80m"), QStringLiteral("60m"), QStringLiteral("40m"),
            QStringLiteral("30m"), QStringLiteral("20m"), QStringLiteral("17m"), QStringLiteral("15m"),
            QStringLiteral("12m"), QStringLiteral("10m"), QStringLiteral("6m"), QStringLiteral("4m"),
            QStringLiteral("2m"), QStringLiteral("1.25m"), QStringLiteral("70cm"), QStringLiteral("33cm"),
            QStringLiteral("23cm"), QStringLiteral("13cm")};
}

inline QVector<Entry> defaults()
{
    struct Raw { double hz; const char *mode; const char *region; const char *description; bool preferred; };
    static const Raw raw[] = {
        // WSJT-X 3.0 default_frequency_list (FT8, FT4, MSK144, Q65).
        {1840000, "FT8", "ALL", "", true},
        {3567000, "FT8", "ALL", "DXpedition", false},
        {3573000, "FT8", "ALL", "", true},
        {3575000, "FT4", "ALL", "", true},
        {3568000, "FT4", "R3", "", false},
        {5357000, "FT8", "ALL", "", true},
        {5357000, "FT4", "ALL", "", true},
        {7056000, "FT8", "ALL", "DXpedition", false},
        {7074000, "FT8", "ALL", "", true},
        {7047500, "FT4", "ALL", "", true},
        {10131000, "FT8", "ALL", "DXpedition", false},
        {10136000, "FT8", "ALL", "", true},
        {10140000, "FT4", "ALL", "", true},
        {14074000, "FT8", "ALL", "", true},
        {14090000, "FT8", "ALL", "DXpedition", false},
        {14080000, "FT4", "ALL", "", true},
        {18095000, "FT8", "ALL", "DXpedition", false},
        {18100000, "FT8", "ALL", "", true},
        {18104000, "FT4", "ALL", "", true},
        {21074000, "FT8", "ALL", "", true},
        {21091000, "FT8", "ALL", "DXpedition", false},
        {21140000, "FT4", "ALL", "", true},
        {24911000, "FT8", "ALL", "DXpedition", false},
        {24915000, "FT8", "ALL", "", true},
        {24919000, "FT4", "ALL", "", true},
        {28074000, "FT8", "ALL", "", true},
        {28091000, "FT8", "ALL", "DXpedition", false},
        {28180000, "FT4", "ALL", "", true},
        {50211000, "Q65", "ALL", "EME", false},
        {50275000, "Q65", "ALL", "", true},
        {50380000, "MSK144", "R1", "", false},
        {50260000, "MSK144", "R2", "", false},
        {50260000, "MSK144", "R3", "", false},
        {50313000, "FT8", "ALL", "", true},
        {50318000, "FT4", "ALL", "", true},
        {50323000, "FT8", "ALL", "intercontinental", false},
        {70154000, "FT8", "R1", "", true},
        {70230000, "MSK144", "R1", "", false},
        {144116000, "Q65", "ALL", "EME", false},
        {144170000, "FT4", "ALL", "", true},
        {144174000, "FT8", "ALL", "", true},
        {144180000, "Q65", "ALL", "", true},
        {144360000, "MSK144", "R1", "", false},
        {144150000, "MSK144", "R2", "", false},
        {222065000, "Q65", "R2", "", false},
        {222174000, "FT8", "R2", "", true},
        {432174000, "FT8", "ALL", "", true},
        {432360000, "MSK144", "ALL", "", false},
        {432065000, "Q65", "ALL", "EME", false},
        {902065000, "Q65", "R2", "", false},
        {1296065000, "Q65", "ALL", "", false},
        {1296174000, "FT8", "ALL", "", true},
        {2301065000, "Q65", "ALL", "", false},
        {2304065000, "Q65", "ALL", "", false},
        // JS8Call 2.2.0 FrequencyList.cpp (JS8).
        {1842000, "JS8", "ALL", "", true},
        {3578000, "JS8", "ALL", "", true},
        {7078000, "JS8", "ALL", "", true},
        {10130000, "JS8", "ALL", "", true},
        {14078000, "JS8", "ALL", "", true},
        {18104000, "JS8", "ALL", "", true},
        {21078000, "JS8", "ALL", "", true},
        {24922000, "JS8", "ALL", "", true},
        {28078000, "JS8", "ALL", "", true},
        {50318000, "JS8", "ALL", "", true},
        {144178000, "JS8", "ALL", "", true},
        // RTTY: activity frequencies (lower end of the usual RTTY segments).
        {3590000, "RTTY", "ALL", "", true},
        {7040000, "RTTY", "ALL", "", true},
        {14080000, "RTTY", "ALL", "", true},
        {21080000, "RTTY", "ALL", "", true},
        {28080000, "RTTY", "ALL", "", true},
        // PSK31: usual calling frequencies (dial, USB).
        {1838000, "PSK", "ALL", "", true},
        {3580000, "PSK", "ALL", "", true},
        {7040000, "PSK", "R1", "", true},
        {7070000, "PSK", "R2", "", true},
        {7070000, "PSK", "R3", "", true},
        {10142000, "PSK", "ALL", "", true},
        {14070000, "PSK", "ALL", "", true},
        {18100000, "PSK", "ALL", "", true},
        {21070000, "PSK", "ALL", "", true},
        {24920000, "PSK", "ALL", "", true},
        {28120000, "PSK", "ALL", "", true},
        {28070000, "PSK", "ALL", "", false},
        {50290000, "PSK", "ALL", "", true},
        {144144000, "PSK", "ALL", "", true},
        // MFSK (MFSK16): usual activity frequencies.
        {1838000, "MFSK", "ALL", "", true},
        {3580000, "MFSK", "ALL", "", true},
        {7037000, "MFSK", "ALL", "", true},
        {10147000, "MFSK", "ALL", "", true},
        {14072000, "MFSK", "ALL", "", true},
        {18105000, "MFSK", "ALL", "", true},
        {21080000, "MFSK", "ALL", "", true},
        {24929000, "MFSK", "ALL", "", true},
        {28080000, "MFSK", "ALL", "", true},
        // CW: IARU Region 1 QRP centres of activity (HF band plan 2020).
        {1836000, "CW", "ALL", "QRP", true},
        {3560000, "CW", "ALL", "QRP", true},
        {7030000, "CW", "ALL", "QRP", true},
        {10116000, "CW", "ALL", "QRP (customary)", true},
        {14060000, "CW", "ALL", "QRP", true},
        {18086000, "CW", "ALL", "QRP", true},
        {21060000, "CW", "ALL", "QRP", true},
        {24906000, "CW", "ALL", "QRP", true},
        {28060000, "CW", "ALL", "QRP", true},
        {50090000, "CW", "ALL", "calling", true},
        {144050000, "CW", "ALL", "calling", true},
        // Feld Hell: Feld Hell Club frequencies.
        {1843000, "HELL", "ALL", "Feld Hell Club", true},
        {3578000, "HELL", "ALL", "Feld Hell Club", true},
        {7080000, "HELL", "ALL", "Feld Hell Club", true},
        {10137000, "HELL", "ALL", "Feld Hell Club", true},
        {14063000, "HELL", "ALL", "Feld Hell Club", true},
        {18098000, "HELL", "ALL", "Feld Hell Club", true},
        {21063000, "HELL", "ALL", "Feld Hell Club", true},
        {24924000, "HELL", "ALL", "Feld Hell Club", true},
        {28063000, "HELL", "ALL", "Feld Hell Club", true},
        {50286000, "HELL", "ALL", "Feld Hell Club", true},
        // DWD (Deutscher Wetterdienst) maritime weather RTTY, 50 Bd, 450 Hz
        // shift, receive only.  Dial USB 2.35 kHz below the assigned
        // frequency, so the signal centre is at 2350 Hz with the "DWD"
        // preset (Mark 2125 Hz).
        {144950, "RTTY", "ALL", "DWD DDH47 (147.3 kHz)", false},
        {4580650, "RTTY", "ALL", "DWD DDK2 (4583.0 kHz)", true},
        {7643650, "RTTY", "ALL", "DWD DDH7 (7646.0 kHz)", false},
        {10098450, "RTTY", "ALL", "DWD DDK9 (10100.8 kHz)", false},
        {11036650, "RTTY", "ALL", "DWD DDH9 (11039.0 kHz)", false},
        {14464950, "RTTY", "ALL", "DWD DDH8 (14467.3 kHz)", false},
        // SSTV calling frequencies (80/40 m in LSB).
        {3735000, "SSTV", "R1", "LSB", true},
        {3845000, "SSTV", "R2", "LSB", true},
        {7165000, "SSTV", "R1", "LSB", true},
        {7171000, "SSTV", "R2", "LSB", true},
        {14230000, "SSTV", "ALL", "", true},
        {14233000, "SSTV", "ALL", "digital SSTV", false},
        {21340000, "SSTV", "ALL", "", true},
        {28680000, "SSTV", "ALL", "", true},
        {50680000, "SSTV", "ALL", "", true},
        // Weather fax: DWD Hamburg/Pinneberg, dial USB 1.9 kHz below the centre.
        {3853100, "WEFAX", "ALL", "DWD DDH3 (3855.0 kHz)", false},
        {7878100, "WEFAX", "ALL", "DWD DDK3 (7880.0 kHz)", true},
        {13880600, "WEFAX", "ALL", "DWD DDK6 (13882.5 kHz)", false},
    };
    QVector<Entry> out;
    for (const Raw &r : raw)
        out.append({QString::fromLatin1(r.mode), r.hz, QString::fromLatin1(r.region), QString::fromLatin1(r.description), r.preferred});
    return out;
}

inline QString toJson(const QVector<Entry> &entries)
{
    QJsonArray list;
    for (const Entry &e : entries) {
        QJsonObject o;
        o.insert(QStringLiteral("mode"), e.mode);
        o.insert(QStringLiteral("hz"), double(qRound64(e.hz)));
        o.insert(QStringLiteral("region"), e.region);
        if (!e.description.isEmpty()) o.insert(QStringLiteral("description"), e.description);
        if (e.preferred) o.insert(QStringLiteral("preferred"), true);
        list << o;
    }
    return QString::fromUtf8(QJsonDocument(list).toJson(QJsonDocument::Compact));
}

/// Parses a stored list; invalid entries are skipped.  @p ok is false for
/// unreadable JSON (the caller then keeps the defaults).
inline QVector<Entry> fromJson(const QString &json, bool *ok = nullptr)
{
    QVector<Entry> out;
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (ok != nullptr) *ok = doc.isArray();
    for (const QJsonValue &v : doc.array()) {
        const QJsonObject o = v.toObject();
        Entry e;
        e.mode = o.value(QStringLiteral("mode")).toString().trimmed().toUpper();
        e.hz = std::round(o.value(QStringLiteral("hz")).toDouble());
        e.region = o.value(QStringLiteral("region")).toString(QStringLiteral("ALL")).trimmed().toUpper();
        if (!regions().contains(e.region)) e.region = QStringLiteral("ALL");
        e.description = o.value(QStringLiteral("description")).toString().trimmed();
        e.preferred = o.value(QStringLiteral("preferred")).toBool(false);
        if (e.mode.isEmpty() || e.hz < 100000.0 || e.hz > 11.0e9) continue;
        out << e;
    }
    return out;
}

/// Same entries regardless of their order (the editor sorts its rows).
inline bool sameEntries(QVector<Entry> a, QVector<Entry> b)
{
    if (a.size() != b.size()) return false;
    const auto less = [](const Entry &x, const Entry &y) {
        if (x.mode != y.mode) return x.mode < y.mode;
        if (qRound64(x.hz) != qRound64(y.hz)) return qRound64(x.hz) < qRound64(y.hz);
        if (x.region != y.region) return x.region < y.region;
        if (x.description != y.description) return x.description < y.description;
        return x.preferred < y.preferred;
    };
    std::sort(a.begin(), a.end(), less);
    std::sort(b.begin(), b.end(), less);
    return a == b;
}

/// @p entries plus every default entry they lack (same mode, frequency and
/// region).  An added entry is not preferred where the operator already has
/// a preferred entry for that mode, band and region.  @p added receives the
/// number of entries added.
inline QVector<Entry> withMissingDefaults(QVector<Entry> entries, int *added = nullptr)
{
    const auto key = [](const Entry &e) { return e.mode + QLatin1Char('|') + QString::number(qRound64(e.hz)) + QLatin1Char('|') + e.region; };
    const auto slot = [](const Entry &e) { return e.mode + QLatin1Char('|') + groupOf(e.hz) + QLatin1Char('|') + e.region; };
    QSet<QString> present, preferredSlots;
    for (const Entry &e : entries) {
        present.insert(key(e));
        if (e.preferred) preferredSlots.insert(slot(e));
    }
    int count = 0;
    for (Entry e : defaults()) {
        if (present.contains(key(e))) continue;
        if (e.preferred && preferredSlots.contains(slot(e))) e.preferred = false;
        entries << e;
        ++count;
    }
    if (added) *added = count;
    return entries;
}

/// The operator's list from the settings.  @p knownModes are the modes the
/// list was saved with (empty for lists saved before that was recorded: the
/// modes it contains).  Modes added to MultiModeModem since then get their
/// default entries, so a saved list does not hide new modes.  An empty or
/// unreadable @p json gives the defaults; @p ok is false if it was unreadable.
inline QVector<Entry> fromSettings(const QString &json, QStringList knownModes, bool *ok = nullptr)
{
    if (ok) *ok = true;
    if (json.trimmed().isEmpty()) return defaults();
    bool readable = false;
    QVector<Entry> entries = fromJson(json, &readable);
    if (!readable || entries.isEmpty()) {
        if (ok) *ok = readable;
        return defaults();
    }
    if (knownModes.isEmpty())
        for (const Entry &e : entries)
            if (!knownModes.contains(e.mode)) knownModes << e.mode;
    for (const Entry &e : defaults())
        if (!knownModes.contains(e.mode)) entries << e;
    return entries;
}

namespace detail {
struct State
{
    QVector<Entry> entries = defaults();
    QString region = QStringLiteral("ALL");
};
inline State &state()
{
    static State s;
    return s;
}
} // namespace detail

/// Installs the list in use (GUI thread); an empty list restores the defaults.
inline void setCurrent(const QVector<Entry> &entries, const QString &region)
{
    detail::state().entries = entries.isEmpty() ? defaults() : entries;
    detail::state().region = regions().contains(region) ? region : QStringLiteral("ALL");
}

inline QVector<Entry> current() { return detail::state().entries; }
inline QString currentRegion() { return detail::state().region; }

inline bool regionMatches(const QString &entryRegion, const QString &selected)
{
    return selected == QLatin1String("ALL") || entryRegion == QLatin1String("ALL") || entryRegion == selected;
}

/// Dial frequency for @p mode on @p band from @p entries: the preferred
/// entry, otherwise the first one listed.  0 if there is none.
inline double dialHz(const QVector<Entry> &entries, const QString &region, const QString &mode, const QString &band)
{
    const QString m = mode.trimmed().toUpper();
    const QString b = band.trimmed().toLower();
    double first = 0.0;
    for (const Entry &e : entries) {
        if (e.mode != m || groupOf(e.hz).toLower() != b || !regionMatches(e.region, region)) continue;
        if (e.preferred) return e.hz;
        if (first <= 0.0) first = e.hz;
    }
    return first;
}

inline double dialHz(const QString &mode, const QString &band)
{
    return dialHz(detail::state().entries, detail::state().region, mode, band);
}

/// The preferred (else first) entry of @p mode in the current region,
/// regardless of band; 0 if none.  For broadcast modes such as WEFAX.
inline double preferredAnyBandHz(const QString &mode)
{
    const QString m = mode.trimmed().toUpper();
    double first = 0.0;
    for (const Entry &e : detail::state().entries) {
        if (e.mode != m || !regionMatches(e.region, detail::state().region)) continue;
        if (e.preferred) return e.hz;
        if (first <= 0.0) first = e.hz;
    }
    return first;
}

/// True if @p hz lies within @p toleranceHz of an entry of @p mode.
inline bool nearEntry(const QString &mode, double hz, double toleranceHz)
{
    const QString m = mode.trimmed().toUpper();
    for (const Entry &e : detail::state().entries)
        if (e.mode == m && regionMatches(e.region, detail::state().region) && std::fabs(e.hz - hz) <= toleranceHz) return true;
    return false;
}

/// True if the list has any entry for @p mode.
inline bool hasStandard(const QString &mode)
{
    const QString m = mode.trimmed().toUpper();
    for (const Entry &e : detail::state().entries)
        if (e.mode == m) return true;
    return false;
}

/// Bands with a frequency for @p mode in the list in use, low to high.
inline QStringList bands(const QString &mode)
{
    QStringList out;
    for (const QString &band : allBands())
        if (dialHz(mode, band) > 0.0) out << band;
    return out;
}

/// bands() plus otherGroup() when @p mode has entries outside the bands.
inline QStringList groups(const QString &mode)
{
    QStringList out = bands(mode);
    if (dialHz(mode, otherGroup()) > 0.0) out << otherGroup();
    return out;
}

/// Entries of @p mode in @p group (a band or otherGroup()) for the current
/// region, low to high.  An empty @p group means all groups.
inline QVector<Entry> entriesFor(const QString &mode, const QString &group)
{
    const QString m = mode.trimmed().toUpper();
    const QString g = group.trimmed().toLower();
    QVector<Entry> out;
    for (const Entry &e : detail::state().entries)
        if (e.mode == m && regionMatches(e.region, detail::state().region) && (g.isEmpty() || groupOf(e.hz).toLower() == g))
            out << e;
    std::sort(out.begin(), out.end(), [](const Entry &a, const Entry &b) { return a.hz < b.hz; });
    return out;
}

/// Parses an entered dial frequency: "14.078 MHz", "14078 kHz", "14078000",
/// "4583" (kHz), "7,074" (MHz).  Without a unit, values below 1000 are MHz,
/// below 10^7 kHz, otherwise Hz.  Returns 0 if invalid.
inline double parseFrequency(const QString &input)
{
    QString text = input.trimmed().toLower();
    text.remove(QLatin1Char(' '));
    double scale = 0.0;
    if (text.endsWith(QLatin1String("mhz"))) { scale = 1e6; text.chop(3); }
    else if (text.endsWith(QLatin1String("khz"))) { scale = 1e3; text.chop(3); }
    else if (text.endsWith(QLatin1String("hz"))) { scale = 1.0; text.chop(2); }
    text.replace(QLatin1Char(','), QLatin1Char('.'));
    if (text.count(QLatin1Char('.')) > 1) {   // 14.078.000: grouped Hz
        text.remove(QLatin1Char('.'));
        if (scale == 0.0) scale = 1.0;
    }
    bool number = false;
    const double value = text.toDouble(&number);
    if (!number || value <= 0.0) return 0.0;
    if (scale == 0.0) scale = value < 1000.0 ? 1e6 : (value < 1e7 ? 1e3 : 1.0);
    const double hz = std::round(value * scale);
    return hz >= 10000.0 && hz <= 11.0e9 ? hz : 0.0;
}

} // namespace M3StandardFrequencies

#endif // M3_STANDARDFREQUENCIES_H
