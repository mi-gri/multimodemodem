// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
// Reception reports to PSK Reporter (https://pskreporter.info), taken over from
// JTDX (psk_reporter.cpp by Edson Pereira PY2SDR, pskreport.h by CE3TSK): IPFIX
// datagrams over UDP to report.pskreporter.info:4739, sent in batches every
// five minutes. The datagram layout is JTDX's byte for byte, with CE3TSK's two
// fixes: at most 1200 bytes per datagram (no IP fragmentation) and a 5-byte
// frequency field (as WSJT-X), so frequencies above 4.29 GHz stay intact.

#include <QByteArray>
#include <QDateTime>
#include <QHash>
#include <QHostAddress>
#include <QObject>
#include <QQueue>
#include <QString>
#include <QVector>

class QTimer;
class QUdpSocket;
class QHostInfo;

namespace PskReport {

struct Spot
{
    QString call;
    QString grid;
    qint64 frequencyHz = 0;   // dial + audio offset
    QString mode;             // "FT8", "FT4", "JS8", "RTTY", "CW" ...
    int snr = 0;
    quint32 unixTime = 0;
    bool hasSnr = true;       // false: text modes without an SNR estimate
};

struct LocalStation
{
    QString call;
    QString grid;
    QString antenna;
    QString software;
};

constexpr int kMaxPayloadBytes = 1200;   // one datagram, IPFIX payload
constexpr quint16 kPort = 4739;

// Builds one datagram from the front of the queue: as many spots of the same
// kind (with or without SNR) as fit within kMaxPayloadBytes, at least one.
// Removes the spots it took.  Spots without SNR use a second sender template
// (0x50E4) that leaves the SNR field out, as fldigi does for text modes.
QByteArray takeDatagram(QQueue<Spot> &queue, const LocalStation &local, quint32 sequence,
                        quint32 randomId, quint32 exportTime);

// A spot worth reporting: a plain callsign (no <hash>, no own call) with a real
// locator (not RR73), and a known dial frequency.  With requireGrid false an
// empty locator is accepted (JS8 and text modes); a given one must be valid.
bool isReportable(const QString &call, const QString &grid, qint64 frequencyHz, const QString &myCall,
                  bool requireGrid = true);

} // namespace PskReport

class PskReporter : public QObject
{
    Q_OBJECT
public:
    explicit PskReporter(QObject *parent = nullptr);
    ~PskReporter() override;

    void setLocalStation(const PskReport::LocalStation &local);
    // Queues a spot; the same call on the same band and mode is reported at
    // most once per five minutes, as PSK Reporter asks.
    bool addSpot(const PskReport::Spot &spot);
    int queuedSpots() const { return m_queue.size(); }

public slots:
    void sendReport();

private:
    void lookupHost();
    void hostFound(const QHostInfo &info);

    PskReport::LocalStation m_local;
    QQueue<PskReport::Spot> m_queue;
    QHash<QString, qint64> m_lastQueuedMs;   // call|band|mode
    QHostAddress m_address;
    bool m_lookupPending = false;
    QUdpSocket *m_socket = nullptr;
    QTimer *m_timer = nullptr;
    quint32 m_sequence = 0;
    quint32 m_randomId = 0;
};
