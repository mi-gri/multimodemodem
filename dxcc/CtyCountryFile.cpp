#include "CtyCountryFile.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QRegularExpression>
#include <QTextStream>
#include <algorithm>
#include <atomic>
#include <cmath>
#include <memory>
#include <mutex>
#include <vector>

namespace {
std::atomic<const CtyCountryFile *> g_current{nullptr};
std::mutex g_installMutex;
// Replaced data sets stay alive until exit: a lookup on another thread may
// still be using one. Updates are rare (manual or weekly), a few MB each.
std::vector<std::unique_ptr<const CtyCountryFile>> &retired()
{
    static std::vector<std::unique_ptr<const CtyCountryFile>> list;
    return list;
}
} // namespace

const CtyCountryFile &CtyCountryFile::instance()
{
    static const CtyCountryFile *initial = [] {
        // The newer of the bundled file and a downloaded user copy.
        auto bundled = std::unique_ptr<const CtyCountryFile>(new CtyCountryFile(bundledFilePath()));
        auto user = std::unique_ptr<const CtyCountryFile>(new CtyCountryFile(userFilePath()));
        const bool userUsable = user->isLoaded() && user->entityCount() >= kMinimumEntities &&
                                (!bundled->isLoaded() || user->version() >= bundled->version());
        const CtyCountryFile *chosen = userUsable ? user.release() : bundled.release();
        const CtyCountryFile *expected = nullptr;
        g_current.compare_exchange_strong(expected, chosen);
        return chosen;
    }();
    Q_UNUSED(initial);
    return *g_current.load(std::memory_order_acquire);
}

QString CtyCountryFile::bundledFilePath()
{
    return QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("cty.csv"));
}

QString CtyCountryFile::userFilePath()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dir.isEmpty()) dir = QDir::homePath() + QStringLiteral("/.MultiModeModem");
    return QDir(dir).filePath(QStringLiteral("cty.csv"));
}

CtyCountryFile::InstallResult CtyCountryFile::installDownloaded(const QByteArray &data)
{
    InstallResult result;
    if (data.size() < 10000 || data.size() > 8 * 1024 * 1024) {
        result.message = QStringLiteral("cty.csv download rejected: unexpected size %1 bytes").arg(data.size());
        return result;
    }
    // Parse from a private temporary file with the normal loader.
    QTemporaryFile temp;
    if (!temp.open() || temp.write(data) != data.size() || !temp.flush()) {
        result.message = QStringLiteral("cty.csv download could not be stored temporarily");
        return result;
    }
    auto candidate = std::unique_ptr<const CtyCountryFile>(new CtyCountryFile(temp.fileName()));
    result.version = candidate->version();
    result.entities = candidate->entityCount();
    if (!candidate->isLoaded() || candidate->entityCount() < kMinimumEntities || candidate->version().isEmpty()) {
        result.message = QStringLiteral("cty.csv download rejected: %1 entities, version '%2' (incomplete or not a cty.csv)")
                             .arg(candidate->entityCount()).arg(candidate->version());
        return result;
    }
    std::lock_guard<std::mutex> lock(g_installMutex);
    const CtyCountryFile &current = instance();
    if (current.isLoaded() && candidate->version() < current.version()) {
        result.message = QStringLiteral("cty.csv download is older (%1) than the data in use (%2); kept the current file")
                             .arg(candidate->version(), current.version());
        result.ok = true;
        return result;
    }
    if (current.isLoaded() && candidate->version() == current.version()) {
        result.ok = true;
        result.message = QStringLiteral("cty.csv is up to date (version %1, %2 entities)")
                             .arg(current.version()).arg(current.entityCount());
        return result;
    }
    const QString target = userFilePath();
    QDir().mkpath(QFileInfo(target).absolutePath());
    QSaveFile out(target);
    if (!out.open(QIODevice::WriteOnly) || out.write(data) != data.size() || !out.commit()) {
        result.message = QStringLiteral("cty.csv could not be saved to %1: %2").arg(target, out.errorString());
        return result;
    }
    // Same data, but recorded with its final path for the settings display.
    auto installed = std::unique_ptr<const CtyCountryFile>(new CtyCountryFile(target));
    if (!installed->isLoaded() || installed->version() != candidate->version()) {
        result.message = QStringLiteral("cty.csv saved to %1 but could not be reloaded").arg(target);
        return result;
    }
    const CtyCountryFile *previous = g_current.exchange(installed.release(), std::memory_order_acq_rel);
    if (previous != nullptr) retired().emplace_back(previous);
    result.ok = true;
    result.updated = true;
    result.message = QStringLiteral("cty.csv updated to version %1 (%2 entities, was %3)")
                         .arg(result.version).arg(result.entities)
                         .arg(previous != nullptr && !previous->version().isEmpty() ? previous->version() : QStringLiteral("unknown"));
    return result;
}

CtyCountryFile::CtyCountryFile()
    : CtyCountryFile(bundledFilePath())
{
}

CtyCountryFile::CtyCountryFile(const QString &path)
{
    loadFromFile(path);
}

QString CtyCountryFile::normalizeCallsignForDxcc(const QString &callsign)
{
    static const QRegularExpression invalid(QStringLiteral("[^A-Z0-9/]+"));
    QString c = callsign.trimmed().toUpper();
    c.remove(invalid);
    while (c.contains(QStringLiteral("//"))) {
        c.replace(QStringLiteral("//"), QStringLiteral("/"));
    }
    if (c.startsWith('/')) c.remove(0, 1);
    if (c.endsWith('/')) c.chop(1);
    return c;
}

QString CtyCountryFile::cleanPrefixToken(const QString &token)
{
    QString t = token.trimmed().toUpper();
    if (t.isEmpty()) {
        return QString();
    }

    // cty.csv tokens may contain CQ/ITU/zone overrides such as W(5)[8].  For
    // entity matching we only need the callsign/prefix part before metadata.
    const int paren = t.indexOf('(');
    const int bracket = t.indexOf('[');
    int cut = -1;
    if (paren >= 0) cut = paren;
    if (bracket >= 0 && (cut < 0 || bracket < cut)) cut = bracket;
    if (cut >= 0) t = t.left(cut);

    static const QRegularExpression invalid(QStringLiteral("[^=A-Z0-9/]+"));
    t.remove(invalid);
    return t;
}

bool CtyCountryFile::looksLikeMaidenheadGrid(const QString &grid, int minLength)
{
    const QString g = grid.trimmed().toUpper();
    if (g.size() < minLength || g.size() < 4) {
        return false;
    }
    if (g.at(0) < QLatin1Char('A') || g.at(0) > QLatin1Char('R') ||
        g.at(1) < QLatin1Char('A') || g.at(1) > QLatin1Char('R') ||
        !g.at(2).isDigit() || !g.at(3).isDigit()) {
        return false;
    }
    if (g.size() >= 6) {
        if (g.at(4) < QLatin1Char('A') || g.at(4) > QLatin1Char('X') ||
            g.at(5) < QLatin1Char('A') || g.at(5) > QLatin1Char('X')) {
            return false;
        }
    }
    return true;
}

QVector<QString> CtyCountryFile::csvFieldsPreserveTail(const QString &line, int firstFields)
{
    QVector<QString> fields;
    fields.reserve(firstFields + 1);
    int start = 0;
    for (int i = 0; i < line.size() && fields.size() < firstFields; ++i) {
        if (line.at(i) == QChar(',')) {
            fields.append(line.mid(start, i - start).trimmed());
            start = i + 1;
        }
    }
    fields.append(line.mid(start).trimmed());
    return fields;
}

bool CtyCountryFile::loadFromFile(const QString &path)
{
    m_sourcePath = path;
    QFile file(path);
    if (!file.exists()) {
        m_errorString = QStringLiteral("cty.csv not found next to executable: %1").arg(path);
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_errorString = file.errorString();
        return false;
    }

    QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    in.setCodec("UTF-8");
#endif

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }
        if (line.endsWith(';')) {
            line.chop(1);
        }

        const QVector<QString> f = csvFieldsPreserveTail(line, 9);
        if (f.size() < 10) {
            continue;
        }

        Entity e;
        e.primaryPrefix = f.value(0).trimmed().toUpper();
        e.name = f.value(1).trimmed();
        e.dxcc = f.value(2).trimmed();
        e.continent = f.value(3).trimmed().toUpper();
        e.cqZone = f.value(4).toInt();
        e.ituZone = f.value(5).toInt();
        e.latitude = f.value(6).toDouble();
        // country-files.com cty.csv stores longitude with the ham-radio
        // convention: positive west, negative east.  MultiModeModem map geometry uses
        // the normal GIS convention: positive east.  Invert once at load time,
        // otherwise European DXCC centroids fall on the wrong side of Greenwich.
        e.longitude = -f.value(7).toDouble();
        e.utcOffset = f.value(8).toDouble();
        e.referenceGrid = gridFromLonLat(e.longitude, e.latitude, 6);

        const QStringList tokens = f.value(9).split(QRegularExpression(QStringLiteral("\\s+")), Qt::SkipEmptyParts);
        for (const QString &raw : tokens) {
            QString t = cleanPrefixToken(raw);
            if (t.isEmpty()) {
                continue;
            }
            const auto cq = QRegularExpression(QStringLiteral("\\(([0-9]+)\\)")).match(raw);
            const auto itu = QRegularExpression(QStringLiteral("\\[([0-9]+)\\]")).match(raw);
            if (cq.hasMatch() || itu.hasMatch()) m_zoneOverrides.insert(t, qMakePair(cq.captured(1).toInt(), itu.captured(1).toInt()));
            if (t.startsWith('=')) {
                t.remove(0, 1);
                // country-files.com marks the file version as "=VERyyyymmdd".
                if (t.size() == 11 && t.startsWith(QLatin1String("VER")) &&
                    std::all_of(t.cbegin() + 3, t.cend(), [](QChar c) { return c.isDigit(); })) {
                    m_version = t.mid(3);
                    continue;
                }
                if (!t.isEmpty()) {
                    e.exactCalls.append(t);
                }
            } else {
                e.prefixes.append(t);
            }
        }
        if (!e.primaryPrefix.isEmpty() && !e.prefixes.contains(e.primaryPrefix)) {
            e.prefixes.prepend(e.primaryPrefix);
        }

        const int idx = m_entities.size();
        m_entities.append(e);
        for (const QString &exact : e.exactCalls) {
            m_exactIndex.insert(exact, idx);
        }
        for (const QString &prefix : e.prefixes) {
            if (!prefix.isEmpty()) {
                m_prefixIndex.append(qMakePair(prefix, idx));
            }
        }
    }

    std::sort(m_prefixIndex.begin(), m_prefixIndex.end(), [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
        if (a.first.size() != b.first.size()) return a.first.size() > b.first.size();
        return a.first < b.first;
    });
    // Longest-prefix lookup by hash: the first entry of each prefix in the
    // sorted list wins, exactly as the former linear scan did.
    m_prefixHash.clear();
    m_maxPrefixLength = 0;
    for (const auto &p : m_prefixIndex) {
        if (!m_prefixHash.contains(p.first)) m_prefixHash.insert(p.first, p.second);
        m_maxPrefixLength = qMax(m_maxPrefixLength, p.first.size());
    }

    m_loaded = !m_entities.isEmpty();
    if (!m_loaded) {
        m_errorString = QStringLiteral("cty.csv loaded but no country entities were parsed: %1").arg(path);
    }
    return m_loaded;
}


CtyCountryFile::LookupResult CtyCountryFile::lookupEntityNameOrPrefix(const QString &query) const
{
    LookupResult result;
    const QString q = query.trimmed();
    if (q.isEmpty() || m_entities.isEmpty()) {
        return result;
    }

    QString norm = q.toUpper();
    norm.replace(QChar('_'), QChar(' '));
    norm.replace(QChar('-'), QChar(' '));
    norm = norm.simplified();

    auto canonicalAlias = [](const QString &value) -> QString {
        const QString v = value.trimmed().toUpper().simplified();
        if (v == QLatin1String("GERMANY") || v == QLatin1String("DEUTSCHLAND") || v == QLatin1String("DL")) {
            return QStringLiteral("FED. REP. OF GERMANY");
        }
        if (v == QLatin1String("USA") || v == QLatin1String("UNITED STATES") || v == QLatin1String("UNITED STATES OF AMERICA")) {
            return QStringLiteral("UNITED STATES");
        }
        if (v == QLatin1String("UK") || v == QLatin1String("UNITED KINGDOM") || v == QLatin1String("GREAT BRITAIN")) {
            return QStringLiteral("ENGLAND");
        }
        if (v == QLatin1String("SAN MARINO")) {
            return QStringLiteral("SAN MARINO");
        }
        if (v == QLatin1String("CZECH REPUBLIC") || v == QLatin1String("CZECHIA")) {
            return QStringLiteral("CZECH REPUBLIC");
        }
        if (v == QLatin1String("IVORY COAST") || v == QLatin1String("COTE D IVOIRE") || v == QLatin1String("CÔTE D'IVOIRE")) {
            return QStringLiteral("COTE D'IVOIRE");
        }
        return v;
    };

    const QString aliasNorm = canonicalAlias(norm);
    auto use = [&](int idx, const QString &token, bool exact) {
        if (idx >= 0 && idx < m_entities.size()) {
            result.valid = true;
            result.entity = m_entities.at(idx);
            result.matchedToken = token;
            result.exactMatch = exact;
            const auto zones = m_zoneOverrides.value((exact ? QStringLiteral("=") : QString()) + token);
            if (zones.first > 0) result.entity.cqZone = zones.first;
            if (zones.second > 0) result.entity.ituZone = zones.second;
        }
    };

    // Exact country/DXCC/name matching must happen before prefix matching.
    // Otherwise free text such as "Germany" can be misread as a G* callsign/prefix.
    for (int i = 0; i < m_entities.size(); ++i) {
        const Entity &e = m_entities.at(i);
        const QString nameNorm = canonicalAlias(e.name);
        if (nameNorm == aliasNorm ||
            e.name.compare(q, Qt::CaseInsensitive) == 0 ||
            e.dxcc.compare(q, Qt::CaseInsensitive) == 0 ||
            e.primaryPrefix.compare(norm, Qt::CaseInsensitive) == 0) {
            use(i, e.name, true);
            return result;
        }
    }

    // If the input looks like a human country name, try contains matching before callsign lookup.
    const bool looksLikeCountryText = q.contains(QRegularExpression(QStringLiteral("[\\s_.-]"))) || q.size() > 3;
    if (looksLikeCountryText) {
        for (int i = 0; i < m_entities.size(); ++i) {
            const Entity &e = m_entities.at(i);
            if (e.name.contains(q, Qt::CaseInsensitive) || aliasNorm.contains(canonicalAlias(e.name)) || canonicalAlias(e.name).contains(aliasNorm)) {
                use(i, e.name, false);
                return result;
            }
        }
    }

    // Finally allow ordinary prefix/callsign matching, so EA, DL, K, I, or real
    // callsigns resolve through the same DXCC rules as the decode table.
    result = lookupCallsign(q);
    if (result.valid) {
        return result;
    }

    for (int i = 0; i < m_entities.size(); ++i) {
        const Entity &e = m_entities.at(i);
        if (e.name.contains(q, Qt::CaseInsensitive)) {
            use(i, e.name, false);
            return result;
        }
    }
    return result;
}

CtyCountryFile::LookupResult CtyCountryFile::lookupCallsign(const QString &callsign) const
{
    LookupResult result;
    const QString call = normalizeCallsignForDxcc(callsign);
    if (call.isEmpty() || m_entities.isEmpty()) {
        return result;
    }

    auto useIndex = [&](int idx, const QString &token, bool exact) {
        if (idx >= 0 && idx < m_entities.size()) {
            result.valid = true;
            result.entity = m_entities.at(idx);
            result.matchedToken = token;
            result.exactMatch = exact;
        }
    };

    if (m_exactIndex.contains(call)) {
        useIndex(m_exactIndex.value(call), call, true);
        return result;
    }

    // Portable calls are tricky.  Try the whole call first, then try individual
    // components so EA/IZ6NNH or IZ6NNH/P still resolve sensibly.
    QStringList candidates;
    const QStringList parts = call.split('/', Qt::SkipEmptyParts);
    if (parts.size() > 1) {
        const QString suffix = parts.last();
        static const QRegularExpression digitsOnly(QStringLiteral("^[0-9]+$"));
        if (suffix != "P" && suffix != "M" && suffix != "MM" && suffix != "AM" && suffix != "QRP" &&
            !digitsOnly.match(suffix).hasMatch() && m_prefixHash.contains(suffix)) {
            candidates << suffix;
        }
    }
    candidates << call;
    for (const QString &part : parts) {
        if (!candidates.contains(part)) {
            candidates << part;
        }
    }

    for (const QString &candidate : candidates) {
        for (int length = qMin(candidate.size(), m_maxPrefixLength); length > 0; --length) {
            const auto it = m_prefixHash.constFind(candidate.left(length));
            if (it != m_prefixHash.constEnd()) {
                useIndex(it.value(), it.key(), false);
                return result;
            }
        }
    }
    return result;
}

QString CtyCountryFile::refinedGridForCallsign(const QString &callsign, const QString &grid, int precision) const
{
    const int outLen = qBound(4, precision, 8);
    QString g = grid.trimmed().toUpper();

    // A real station locator is always better than a country-file centroid.
    if (looksLikeMaidenheadGrid(g, 6)) {
        return g.left(qMin(g.size(), outLen));
    }

    const LookupResult cty = lookupCallsign(callsign);
    const QString ref = cty.valid ? cty.entity.referenceGrid.trimmed().toUpper() : QString();
    const bool refOk = looksLikeMaidenheadGrid(ref, 6);

    if (g.isEmpty() || !looksLikeMaidenheadGrid(g.left(4), 4)) {
        return refOk ? ref.left(qMin(ref.size(), outLen)) : QString();
    }

    g = g.left(4);

    // Generic small-DXCC refinement: when the only locator we have is the
    // coarse 4-character square and the cty.csv reference point is inside the
    // same square, use the cty.csv 6-character locator.  This fixes San Marino,
    // Vatican, Monaco, Gibraltar, Malta-sized entities, etc. without using a
    // special-case DXCC number and without moving a station to a different grid.
    if (refOk && ref.left(4) == g) {
        return ref.left(qMin(ref.size(), outLen));
    }

    return g;
}

QString CtyCountryFile::gridFromLonLat(double longitude, double latitude, int precision)
{
    if (!std::isfinite(longitude) || !std::isfinite(latitude) || latitude < -90.0 || latitude > 90.0) {
        return QString();
    }
    while (longitude < -180.0) longitude += 360.0;
    while (longitude >= 180.0) longitude -= 360.0;

    double lon = longitude + 180.0;
    double lat = latitude + 90.0;
    QString grid;
    grid.reserve(qMax(4, precision));

    const int a = static_cast<int>(std::floor(lon / 20.0));
    const int b = static_cast<int>(std::floor(lat / 10.0));
    grid.append(QChar('A' + qBound(0, a, 17)));
    grid.append(QChar('A' + qBound(0, b, 17)));
    lon -= a * 20.0;
    lat -= b * 10.0;

    const int c = static_cast<int>(std::floor(lon / 2.0));
    const int d = static_cast<int>(std::floor(lat / 1.0));
    grid.append(QChar('0' + qBound(0, c, 9)));
    grid.append(QChar('0' + qBound(0, d, 9)));
    lon -= c * 2.0;
    lat -= d * 1.0;

    if (precision >= 6) {
        const int e = static_cast<int>(std::floor(lon / (2.0 / 24.0)));
        const int f = static_cast<int>(std::floor(lat / (1.0 / 24.0)));
        grid.append(QChar('A' + qBound(0, e, 23)));
        grid.append(QChar('A' + qBound(0, f, 23)));
    }
    return grid.left(qMax(4, precision)).toUpper();
}
