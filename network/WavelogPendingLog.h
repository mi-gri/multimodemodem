#ifndef WAVELOGPENDINGLOG_H
#define WAVELOGPENDINGLOG_H

#include <QString>
#include <QStringList>

/**
 * @brief Second ADIF file with the QSOs whose Wavelog/Cloudlog upload failed.
 *
 * A QSO is added on its first failed attempt and removed as soon as an upload
 * succeeds (or Wavelog reports it as a duplicate, i.e. it is already there).
 * The file is a normal ADIF log, so it can also be imported by hand. Records
 * are matched by CALL, QSO_DATE, TIME_ON, BAND and MODE. Writes are atomic and
 * the file is owner-only (it is part of the station log).
 */
namespace WavelogPendingLog {

QString defaultPath();
/// Adds @p adifRecord unless a record for the same QSO is present. True if added.
bool add(const QString &path, const QString &adifRecord, QString *error = nullptr);
/// Removes the record for the same QSO. True if one was removed.
bool remove(const QString &path, const QString &adifRecord, QString *error = nullptr);
/// All pending records ("...<EOR>"), oldest first.
QStringList records(const QString &path);
/// "CALL|QSO_DATE|TIME_ON|BAND|MODE" of an ADIF record (upper case).
QString qsoKey(const QString &adifRecord);
QString fieldValue(const QString &adifRecord, const QString &field);

} // namespace WavelogPendingLog

#endif // WAVELOGPENDINGLOG_H
