#ifndef FTX1WIDTH_H
#define FTX1WIDTH_H

#include <QByteArray>
#include <QString>

/**
 * Yaesu FTX-1 receive width (CAT "SH", Table 5 of the FTX-1 CAT reference).
 *
 * Hamlib 4.7 has no FTX-1 width table: it sends "SH0nn;" (missing the fixed
 * P2 digit) which the radio ignores, and reports a nominal width on read.
 * Format: set "SH P1 P2 P3P3;", read "SH P1;", answer "SH P1 P2 P3P3;" with
 * P1 = 0 (MAIN), P2 = 0 (fixed), P3 = 00..23.
 */
namespace Ftx1Width {

// SSB column (LSB/USB), index 01..23.
inline const int *ssbTable(int &count)
{
    static const int t[] = {300, 400, 600, 850, 1100, 1200, 1500, 1650, 1800, 1950, 2100, 2250,
                            2400, 2450, 2500, 2600, 2700, 2800, 2900, 3000, 3200, 3500, 4000};
    count = int(sizeof(t) / sizeof(t[0]));
    return t;
}

// CW / DATA / RTTY / PSK column, index 01..21.
inline const int *dataTable(int &count)
{
    static const int t[] = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 600,
                            800, 1200, 1400, 1700, 2000, 2400, 3000, 3200, 3500, 4000};
    count = int(sizeof(t) / sizeof(t[0]));
    return t;
}

inline bool isSsbMode(const QString &hamlibMode)
{
    return hamlibMode == QLatin1String("USB") || hamlibMode == QLatin1String("LSB");
}

inline bool isDataMode(const QString &hamlibMode)
{
    return hamlibMode == QLatin1String("PKTUSB") || hamlibMode == QLatin1String("PKTLSB");
}

/** Smallest index whose width covers @p hz (capped at the widest); 0 if mode unsupported. */
inline int indexForHz(const QString &hamlibMode, int hz)
{
    int n = 0;
    const int *t = isSsbMode(hamlibMode) ? ssbTable(n) : isDataMode(hamlibMode) ? dataTable(n) : nullptr;
    if (t == nullptr || hz <= 0) return 0;
    for (int i = 0; i < n; ++i)
        if (t[i] >= hz) return i + 1;
    return n;
}

/** Width in Hz for an index; 0 for the mode default (00) or unknown. */
inline int hzForIndex(const QString &hamlibMode, int index)
{
    int n = 0;
    const int *t = isSsbMode(hamlibMode) ? ssbTable(n) : isDataMode(hamlibMode) ? dataTable(n) : nullptr;
    if (t == nullptr || index < 1 || index > n) return 0;
    return t[index - 1];
}

inline QByteArray setCommand(int index)
{
    return QByteArray("SH00") + QByteArray::number(index).rightJustified(2, '0') + ';';
}

inline QByteArray readCommand() { return QByteArrayLiteral("SH0;"); }

/** Parses "SH00nn;" -> nn, or -1. */
inline int parseAnswer(const QByteArray &answer)
{
    const QByteArray a = answer.trimmed();
    if (a.size() != 7 || !a.startsWith("SH0") || a.at(6) != ';') return -1;
    bool ok = false;
    const int v = a.mid(4, 2).toInt(&ok);
    return ok && v >= 0 && v <= 23 ? v : -1;
}

/**
 * DATA SHIFT (menu RADIO SETTING / MODE DATA / 18, CAT "EX010418"): the DATA
 * filter is centred on this audio frequency, so a passband from 0 Hz to the
 * waterfall edge needs shift = width / 2 (10 Hz steps, 0..3000 Hz).
 */
inline int dataShiftForWidth(int widthHz)
{
    const int shift = ((widthHz / 2 + 5) / 10) * 10;
    return shift < 0 ? 0 : (shift > 3000 ? 3000 : shift);
}

inline QByteArray dataShiftSetCommand(int shiftHz)
{
    return QByteArray("EX010418") + QByteArray::number(shiftHz).rightJustified(4, '0') + ';';
}

inline QByteArray dataShiftReadCommand() { return QByteArrayLiteral("EX010418;"); }

/** Parses "EX010418nnnn;" -> nnnn Hz, or -1. */
inline int parseDataShiftAnswer(const QByteArray &answer)
{
    const QByteArray a = answer.trimmed();
    if (a.size() != 13 || !a.startsWith("EX010418") || a.at(12) != ';') return -1;
    bool ok = false;
    const int v = a.mid(8, 4).toInt(&ok);
    return ok && v >= 0 && v <= 3000 ? v : -1;
}

} // namespace Ftx1Width

#endif // FTX1WIDTH_H
