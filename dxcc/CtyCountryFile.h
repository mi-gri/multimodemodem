#ifndef CTYCOUNTRYFILE_H
#define CTYCOUNTRYFILE_H

#include <QByteArray>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QVector>

/**
 * @brief Lightweight loader for AD1C/K1EA country-files.com cty.csv.
 *
 * The bundled file lives next to the executable; a downloaded copy in the
 * user data folder (Help -> Update country file) is used instead when it is
 * at least as new. instance() returns the current data set; installing a new
 * file swaps it atomically and keeps the previous one alive, so lookups that
 * run on other threads (decoder, logbook index) never see freed data.
 */
class CtyCountryFile
{
public:
    struct Entity
    {
        QString primaryPrefix;
        QString name;
        QString dxcc;
        QString continent;
        int cqZone = 0;
        int ituZone = 0;
        double latitude = 0.0;
        double longitude = 0.0;
        double utcOffset = 0.0;
        QString referenceGrid;
        QStringList prefixes;
        QStringList exactCalls;
    };

    struct LookupResult
    {
        bool valid = false;
        Entity entity;
        QString matchedToken;
        bool exactMatch = false;
    };

    static const CtyCountryFile &instance();

    /// country-files.com big CTY in CSV form (the variant m³ ships).
    static QString downloadUrl() { return QStringLiteral("https://www.country-files.com/bigcty/cty.csv"); }
    static QString bundledFilePath();
    static QString userFilePath();

    struct InstallResult
    {
        bool ok = false;        ///< file valid (installed or already current)
        bool updated = false;   ///< a newer version is now in use
        QString version;        ///< version of the checked file (VERyyyymmdd)
        int entities = 0;
        QString message;        ///< English one-line summary for the log
    };
    /// Validates downloaded cty.csv data and, if it is complete and not older
    /// than the data in use, stores it in the user data folder and switches to it.
    static InstallResult installDownloaded(const QByteArray &data);
    /// Minimum number of DXCC entities a complete cty.csv must contain.
    static constexpr int kMinimumEntities = 300;

    bool isLoaded() const { return m_loaded; }
    QString version() const { return m_version; }   ///< e.g. "20261114", empty if unknown
    QString sourcePath() const { return m_sourcePath; }
    QString errorString() const { return m_errorString; }
    int entityCount() const { return m_entities.size(); }

    LookupResult lookupCallsign(const QString &callsign) const;
    LookupResult lookupEntityNameOrPrefix(const QString &query) const;
    /// Country-filter token -> DXCC primary prefix: a primary prefix itself
    /// or any prefix listed for an entity (R9, RA9 -> UA9). Empty if unknown.
    QString primaryPrefixForFilterToken(const QString &token) const;

    /**
     * Returns the best map/grid reference MultiModeModem can derive from the
     * available data.  Exact 6/8-character station locators are preserved.
     * Empty or coarse 4-character locators are refined with the cty.csv
     * DXCC centroid only when that reference is consistent with the decoded
     * 4-character square, so the San Marino fix applies to every small DXCC
     * entity without moving ordinary QSOs to an unrelated country centroid.
     */
    QString refinedGridForCallsign(const QString &callsign,
                                   const QString &grid,
                                   int precision = 6) const;
    static QString normalizeCallsignForDxcc(const QString &callsign);
    static QString gridFromLonLat(double longitude, double latitude, int precision = 4);

private:
    CtyCountryFile();
    explicit CtyCountryFile(const QString &path);

    bool loadFromFile(const QString &path);
    static QString cleanPrefixToken(const QString &token);
    static bool looksLikeMaidenheadGrid(const QString &grid, int minLength = 4);
    static QVector<QString> csvFieldsPreserveTail(const QString &line, int firstFields);

    QVector<Entity> m_entities;
    QHash<QString, int> m_exactIndex;
    QHash<QString, QPair<int,int>> m_zoneOverrides;
    QVector<QPair<QString, int>> m_prefixIndex;
    QHash<QString, int> m_prefixHash;    // prefix -> entity, for longest-prefix lookup
    int m_maxPrefixLength = 0;
    bool m_loaded = false;
    QString m_sourcePath;
    QString m_errorString;
    QString m_version;
};

#endif // CTYCOUNTRYFILE_H
