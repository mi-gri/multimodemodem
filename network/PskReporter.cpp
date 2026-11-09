// SPDX-License-Identifier: GPL-3.0-or-later
#include "PskReporter.h"

#include <QHostInfo>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QTimer>
#include <QUdpSocket>

namespace PskReport {
namespace {

// Templates as JTDX sends them: link ids 0x50E2 (receiver) and 0x50E3 (sender).
const char *const kRxDescriptorHex =
    "0003002C50E200040000"
    "8002FFFF0000768F"   // receiver callsign
    "8004FFFF0000768F"   // receiver locator
    "8008FFFF0000768F"   // decoding software
    "8009FFFF0000768F"   // antenna information
    "0000";
const char *const kTxDescriptorHex =
    "0002003C50E30007"
    "8001FFFF0000768F"   // sender callsign
    "800500050000768F"   // frequency, 5 bytes
    "800600010000768F"   // SNR
    "800AFFFF0000768F"   // mode
    "8003FFFF0000768F"   // sender locator
    "800B00010000768F"   // information source
    "00960004";          // flow start seconds
// The same without SNR, for text modes (RTTY, PSK, MFSK, CW).
const char *const kTxNoSnrDescriptorHex =
    "0002003450E40006"
    "8001FFFF0000768F"   // sender callsign
    "800500050000768F"   // frequency, 5 bytes
    "800AFFFF0000768F"   // mode
    "8003FFFF0000768F"   // sender locator
    "800B00010000768F"   // information source
    "00960004";          // flow start seconds

void putU8(QByteArray &b, quint8 v) { b.append(char(v)); }
void putU16(QByteArray &b, quint16 v) { putU8(b, quint8(v >> 8)); putU8(b, quint8(v)); }
void putU32(QByteArray &b, quint32 v) { putU16(b, quint16(v >> 16)); putU16(b, quint16(v)); }
void putText(QByteArray &b, const QString &text)
{
    const QByteArray bytes = text.toUtf8().left(254);
    putU8(b, quint8(bytes.size()));
    b.append(bytes);
}

QByteArray spotRecord(const Spot &s)
{
    QByteArray r;
    putText(r, s.call);
    const quint64 f = quint64(s.frequencyHz) & 0xFFFFFFFFFFULL;
    putU8(r, quint8(f >> 32));
    putU32(r, quint32(f));
    if (s.hasSnr) putU8(r, quint8(qint8(qBound(-128, s.snr, 127))));
    putText(r, s.mode);
    putText(r, s.grid);
    putU8(r, 1);   // REPORTER_SOURCE_AUTOMATIC
    putU32(r, s.unixTime);
    return r;
}

void patchLength(QByteArray &b, int at, int length)
{
    b[at] = char((length >> 8) & 0xFF);
    b[at + 1] = char(length & 0xFF);
}

} // namespace

QByteArray takeDatagram(QQueue<Spot> &queue, const LocalStation &local, quint32 sequence,
                        quint32 randomId, quint32 exportTime)
{
    QByteArray d;
    putU16(d, 0x000A);   // IPFIX version
    putU16(d, 0);        // length, patched below
    putU32(d, exportTime);
    putU32(d, sequence);
    putU32(d, randomId);
    d.append(QByteArray::fromHex(kRxDescriptorHex));
    d.append(QByteArray::fromHex(kTxDescriptorHex));
    d.append(QByteArray::fromHex(kTxNoSnrDescriptorHex));

    const int rxAt = d.size();
    putU16(d, 0x50E2);
    putU16(d, 0);
    putText(d, local.call);
    putText(d, local.grid);
    putText(d, local.software);
    putText(d, local.antenna);
    putU16(d, 0);   // padding, as JTDX
    patchLength(d, rxAt + 2, d.size() - rxAt);

    const int txAt = d.size();
    const bool withSnr = queue.isEmpty() || queue.head().hasSnr;
    putU16(d, withSnr ? 0x50E3 : 0x50E4);
    putU16(d, 0);
    bool first = true;
    while (!queue.isEmpty() && queue.head().hasSnr == withSnr) {
        const QByteArray record = spotRecord(queue.head());
        // The first spot always goes in, or it would stay queued for ever.
        if (!first && d.size() + record.size() + 2 > kMaxPayloadBytes) break;
        d.append(record);
        queue.dequeue();
        first = false;
    }
    putU16(d, 0);   // padding
    patchLength(d, txAt + 2, d.size() - txAt);
    patchLength(d, 2, d.size());
    return d;
}

bool isReportable(const QString &call, const QString &grid, qint64 frequencyHz, const QString &myCall,
                  bool requireGrid)
{
    static const QRegularExpression callRe(QStringLiteral("^[A-Z0-9/]{3,13}$"));
    static const QRegularExpression gridRe(QStringLiteral("^[A-R]{2}[0-9]{2}([A-X]{2})?$"));
    const QString c = call.trimmed().toUpper();
    const QString g = grid.trimmed().toUpper();
    if (frequencyHz <= 0 || !callRe.match(c).hasMatch() || !c.contains(QRegularExpression(QStringLiteral("[0-9]"))))
        return false;
    if (!myCall.isEmpty() && c == myCall.trimmed().toUpper()) return false;
    if (g.isEmpty()) return !requireGrid;
    return gridRe.match(g).hasMatch() && g != QLatin1String("RR73");
}

} // namespace PskReport

PskReporter::PskReporter(QObject *parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
    , m_timer(new QTimer(this))
    , m_randomId(QRandomGenerator::global()->generate())
{
    connect(m_timer, &QTimer::timeout, this, &PskReporter::sendReport);
    m_timer->start(5 * 60 * 1000);
    lookupHost();
}

PskReporter::~PskReporter()
{
    sendReport();   // what is queued at shutdown
}

void PskReporter::setLocalStation(const PskReport::LocalStation &local)
{
    const bool changed = local.call != m_local.call || local.grid != m_local.grid ||
                         local.antenna != m_local.antenna;
    if (changed && !m_queue.isEmpty()) sendReport();   // queued spots belong to the old station
    m_local = local;
}

bool PskReporter::addSpot(const PskReport::Spot &spot)
{
    const qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
    const QString key = spot.call.toUpper() + QLatin1Char('|') + QString::number(spot.frequencyHz / 1000000) +
                        QLatin1Char('|') + spot.mode;
    const auto it = m_lastQueuedMs.constFind(key);
    if (it != m_lastQueuedMs.constEnd() && nowMs - it.value() < 5 * 60 * 1000) return false;
    m_lastQueuedMs.insert(key, nowMs);
    if (m_lastQueuedMs.size() > 5000) {
        for (auto i = m_lastQueuedMs.begin(); i != m_lastQueuedMs.end();)
            i = (nowMs - i.value() >= 5 * 60 * 1000) ? m_lastQueuedMs.erase(i) : std::next(i);
    }
    m_queue.enqueue(spot);
    return true;
}

void PskReporter::sendReport()
{
    if (m_queue.isEmpty()) return;
    if (m_address.isNull()) {
        lookupHost();   // keep the spots for the next round
        return;
    }
    const quint32 now = quint32(QDateTime::currentSecsSinceEpoch());
    while (!m_queue.isEmpty()) {
        const QByteArray datagram = PskReport::takeDatagram(m_queue, m_local, ++m_sequence, m_randomId, now);
        m_socket->writeDatagram(datagram, m_address, PskReport::kPort);
    }
}

void PskReporter::lookupHost()
{
    if (m_lookupPending) return;
    m_lookupPending = true;
    QHostInfo::lookupHost(QStringLiteral("report.pskreporter.info"), this,
                          [this](const QHostInfo &info) { hostFound(info); });
}

void PskReporter::hostFound(const QHostInfo &info)
{
    m_lookupPending = false;
    for (const QHostAddress &address : info.addresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) { m_address = address; return; }
    }
    if (!info.addresses().isEmpty()) m_address = info.addresses().constFirst();
}
