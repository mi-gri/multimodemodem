// SPDX-License-Identifier: GPL-3.0-or-later
#include "Js8Message.h"

#include "../../third_party/js8call_gpl/DecodedFrame.h"
#include "../../third_party/js8call_gpl/Jsc.h"
#include "../../third_party/js8call_gpl/Varicode.h"

#include <QMutex>
#include <QMutexLocker>
#include <QRegularExpression>

namespace {
QMutex &varicodeMutex()
{
    static QMutex mutex;
    return mutex;
}
} // namespace

QVector<Js8Message::Frame> Js8Message::buildFrames(const QString &myCall, const QString &myGrid,
                                                   const QString &selectedCall, const QString &text,
                                                   Js8Mode::Submode submode, bool forceIdentify, bool forceData)
{
    QVector<Frame> frames;
    const QString clean = normalizeText(text);
    if (clean.isEmpty() || myCall.trimmed().isEmpty()) return frames;
    QMutexLocker locker(&varicodeMutex());
    const auto built = js8::Varicode::buildMessageFrames(myCall.trimmed().toUpper(), myGrid.trimmed().left(4).toUpper(),
                                                         selectedCall.trimmed().toUpper(), clean, forceIdentify,
                                                         forceData, int(submode));
    for (const auto &pair : built) {
        if (pair.first.size() != 12) continue;
        frames.append({pair.first, pair.second});
    }
    return frames;
}

QString Js8Message::preview(const QVector<Frame> &frames, Js8Mode::Submode submode)
{
    QString out;
    for (const Frame &f : frames) out += unpack(f.frame, f.bits, submode).text;
    return out;
}

Js8Message::Unpacked Js8Message::unpack(const QString &frame, int bits, Js8Mode::Submode submode)
{
    Unpacked u;
    u.first = (bits & First) != 0;
    u.last = (bits & Last) != 0;
    QMutexLocker locker(&varicodeMutex());
    js8::DecodedFrame d(frame, bits, int(submode));
    u.text = d.message();
    u.frameType = d.frameType();
    u.heartbeat = d.isHeartbeat();
    u.cq = d.isHeartbeat() && d.isAlt();
    u.compound = d.isCompound();
    if (d.isHeartbeat()) {
        u.from = d.compoundCall();
        u.grid = d.extra().trimmed();
    } else if (d.frameType() == js8::Varicode::FrameCompound) {
        u.from = d.compoundCall();
        u.grid = d.extra().trimmed();
    } else if (d.frameType() == js8::Varicode::FrameCompoundDirected) {
        const QStringList parts = d.directedMessage();
        u.directed = true;
        u.from = parts.value(0);   // "<....>": the call follows in a compound frame
        u.to = parts.value(1);
        u.command = parts.value(2);
        u.extra = parts.value(3);
    } else if (d.isDirectedMessage()) {
        const QStringList parts = d.directedMessage();
        u.directed = true;
        u.from = parts.value(0);
        u.to = parts.value(1);
        u.command = parts.value(2);
        u.extra = parts.value(3);
    }
    return u;
}

QString Js8Message::heartbeatText(const QString &myGrid)
{
    return QStringLiteral("HEARTBEAT %1").arg(myGrid.trimmed().left(4).toUpper()).trimmed();
}

QString Js8Message::cqText(const QString &myGrid)
{
    return QStringLiteral("CQ CQ CQ %1").arg(myGrid.trimmed().left(4).toUpper()).trimmed();
}

bool Js8Message::isValidCallsign(const QString &call)
{
    QMutexLocker locker(&varicodeMutex());
    return js8::Varicode::isValidCallsign(call.trimmed().toUpper(), nullptr);
}

bool Js8Message::dictionaryAvailable()
{
    return js8::JSC::tablesLoaded();
}

QString Js8Message::normalizeText(const QString &text)
{
    QString t = text.toUpper();
    t.replace(QRegularExpression(QStringLiteral("[\\r\\n\\t]+")), QStringLiteral(" "));
    return t.trimmed();
}

QString Js8Message::formatSnr(int snrDb)
{
    return js8::Varicode::formatSNR(snrDb);
}

bool Js8Message::isCommandBuffered(const QString &command)
{
    QMutexLocker locker(&varicodeMutex());
    return js8::Varicode::isCommandBuffered(command);
}

int Js8Message::commandChecksumBits(const QString &command)
{
    QMutexLocker locker(&varicodeMutex());
    return js8::Varicode::isCommandChecksumed(command);
}

bool Js8Message::checksumValid(const QString &checksum, const QString &text, int bits)
{
    if (bits == 32) return js8::Varicode::checksum32Valid(checksum, text);
    if (bits == 16) return js8::Varicode::checksum16Valid(checksum, text);
    return true;
}

QString Js8Message::checksum(const QString &text, int bits)
{
    if (bits == 32) return js8::Varicode::checksum32(text);
    if (bits == 16) return js8::Varicode::checksum16(text);
    return QString();
}

QString Js8Message::rstrip(const QString &text) { return js8::Varicode::rstrip(text); }
QString Js8Message::lstrip(const QString &text) { return js8::Varicode::lstrip(text); }

QStringList Js8Message::parseCallsigns(const QString &text)
{
    QMutexLocker locker(&varicodeMutex());
    return js8::Varicode::parseCallsigns(text);
}

QString Js8Message::baseCallsign(const QString &call)
{
    QString c = call.trimmed().toUpper();
    const int slash = c.indexOf(QLatin1Char('/'));
    if (slash >= 0) {
        const int right = c.size() - slash - 1;
        c = right >= slash ? c.mid(slash + 1) : c.left(slash);
    }
    return c;
}
