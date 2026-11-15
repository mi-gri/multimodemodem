#include "WavelogPendingLog.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>
#include <QStandardPaths>
#include <QTextStream>

namespace WavelogPendingLog {
namespace {

const QString kHeader = QStringLiteral(
    "MultiModeModem: QSOs not yet uploaded to Wavelog/Cloudlog.\n"
    "Entries are removed automatically after a successful upload; the file can\n"
    "also be imported into Wavelog/Cloudlog by hand.\n"
    "<ADIF_VER:5>3.1.4 <PROGRAMID:14>MultiModeModem <EOH>\n");

// Splits ADIF text into records ending in <EOR>, honouring field lengths so
// that "<EOR>" inside a comment cannot end a record.
QStringList splitRecords(const QString &text)
{
    QStringList out;
    int pos = text.indexOf(QLatin1String("<EOH>"), 0, Qt::CaseInsensitive);
    pos = pos < 0 ? 0 : pos + 5;
    QString current;
    while (pos < text.size()) {
        const int lt = text.indexOf(QLatin1Char('<'), pos);
        if (lt < 0) break;
        const int gt = text.indexOf(QLatin1Char('>'), lt);
        if (gt < 0) break;
        const QString tag = text.mid(lt + 1, gt - lt - 1);
        const QStringList parts = tag.split(QLatin1Char(':'));
        const QString name = parts.value(0).trimmed().toUpper();
        if (name == QLatin1String("EOR")) {
            current += QStringLiteral("<EOR>");
            if (!fieldValue(current, QStringLiteral("CALL")).isEmpty()) out << current.trimmed();
            current.clear();
            pos = gt + 1;
            continue;
        }
        bool ok = false;
        const int length = parts.size() >= 2 ? parts.at(1).toInt(&ok) : 0;
        const int valueEnd = gt + 1 + (ok ? qMax(0, length) : 0);
        current += text.mid(lt, valueEnd - lt) + QLatin1Char(' ');
        pos = valueEnd;
    }
    return out;
}

bool writeAll(const QString &path, const QStringList &records, QString *error)
{
    QDir().mkpath(QFileInfo(path).absolutePath());
    if (records.isEmpty()) {
        if (QFile::exists(path) && !QFile::remove(path)) {
            if (error) *error = QStringLiteral("cannot remove %1").arg(path);
            return false;
        }
        return true;
    }
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (error) *error = file.errorString();
        return false;
    }
    file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    QTextStream stream(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    stream.setCodec("UTF-8");
#endif
    stream << kHeader;
    for (const QString &record : records) stream << record << '\n';
    stream.flush();
    if (!file.commit()) {
        if (error) *error = file.errorString();
        return false;
    }
    return true;
}

} // namespace

QString defaultPath()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dir.isEmpty()) dir = QDir::homePath() + QStringLiteral("/.MultiModeModem");
    return QDir(dir).filePath(QStringLiteral("wavelog_pending.adi"));
}

QString fieldValue(const QString &adifRecord, const QString &field)
{
    const QString wanted = field.toUpper();
    int pos = 0;
    while (pos < adifRecord.size()) {
        const int lt = adifRecord.indexOf(QLatin1Char('<'), pos);
        if (lt < 0) break;
        const int gt = adifRecord.indexOf(QLatin1Char('>'), lt);
        if (gt < 0) break;
        const QStringList parts = adifRecord.mid(lt + 1, gt - lt - 1).split(QLatin1Char(':'));
        bool ok = false;
        const int length = parts.size() >= 2 ? parts.at(1).toInt(&ok) : 0;
        if (ok && parts.at(0).trimmed().toUpper() == wanted) return adifRecord.mid(gt + 1, length).trimmed();
        pos = gt + 1 + (ok ? qMax(0, length) : 0);
    }
    return QString();
}

QString qsoKey(const QString &adifRecord)
{
    QStringList parts;
    for (const char *field : {"CALL", "QSO_DATE", "TIME_ON", "BAND", "MODE"})
        parts << fieldValue(adifRecord, QString::fromLatin1(field)).toUpper();
    // TIME_ON may be HHMM or HHMMSS: compare minutes.
    parts[2] = parts.at(2).left(4);
    return parts.join(QLatin1Char('|'));
}

QStringList records(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return {};
    return splitRecords(QString::fromUtf8(file.readAll()));
}

bool add(const QString &path, const QString &adifRecord, QString *error)
{
    const QString key = qsoKey(adifRecord);
    QStringList list = records(path);
    for (const QString &record : list)
        if (qsoKey(record) == key) return false;
    QString record = adifRecord.trimmed();
    if (!record.endsWith(QLatin1String("<EOR>"), Qt::CaseInsensitive)) record += QStringLiteral(" <EOR>");
    list << record;
    return writeAll(path, list, error);
}

bool remove(const QString &path, const QString &adifRecord, QString *error)
{
    if (!QFile::exists(path)) return false;
    const QString key = qsoKey(adifRecord);
    QStringList list = records(path);
    const int before = list.size();
    for (int i = list.size() - 1; i >= 0; --i)
        if (qsoKey(list.at(i)) == key) list.removeAt(i);
    if (list.size() == before) return false;
    return writeAll(path, list, error);
}

} // namespace WavelogPendingLog
