// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef M3_STANDARDFREQUENCIES_H
#define M3_STANDARDFREQUENCIES_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QVector>

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
 * (FrequencyList.cpp, JS8).  The list in use is stored in the settings as
 * JSON; an empty setting means the defaults.
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
            QStringLiteral("Q65")};
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
        if (e.mode != m || bandOf(e.hz).toLower() != b || !regionMatches(e.region, region)) continue;
        if (e.preferred) return e.hz;
        if (first <= 0.0) first = e.hz;
    }
    return first;
}

inline double dialHz(const QString &mode, const QString &band)
{
    return dialHz(detail::state().entries, detail::state().region, mode, band);
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

} // namespace M3StandardFrequencies

#endif // M3_STANDARDFREQUENCIES_H
