// SPDX-License-Identifier: GPL-3.0-or-later
// Reply rules after JS8Call 2.2.0 mainwindow.cpp (processCommandActivity,
// parseRelayPathCallsigns, since), GPL-3.0-or-later.
#include "Js8AutoReply.h"

#include "Js8Message.h"

#include <QRegularExpression>

namespace {
const QString kCallPattern = QStringLiteral(
    R"((?<prefix>[A-Z0-9]{1,4}\/)?(?<base>([0-9A-Z])?([0-9A-Z])([0-9])([A-Z])?([A-Z])?([A-Z])?)(?<suffix>\/[A-Z0-9]{1,4})?)");

Js8StoredMessage storedFrom(const Js8Command &c, const Js8ReplyContext &ctx, const QString &to,
                            const QString &path, const QString &text, Js8StoredMessage::Type type)
{
    Js8StoredMessage m;
    m.type = type;
    m.utc = c.utc.isValid() ? c.utc : QDateTime::currentDateTimeUtc();
    m.from = c.from;
    m.to = to;
    m.path = path;
    m.text = text;
    m.grid = c.grid;
    m.snrDb = c.snrDb;
    m.dialHz = ctx.dialHz;
    m.offsetHz = c.offsetHz;
    m.submode = int(c.submode);
    return m;
}

Js8ReplyAction processImpl(const Js8Command &c, const Js8ReplyContext &ctx, const QString &relayPath, int depth);

// Relay ">" addressed to this station.
Js8ReplyAction processRelay(const Js8Command &c, const Js8ReplyContext &ctx, Js8ReplyAction action, int depth)
{
    static const QRegularExpression callTo(QStringLiteral("^(?<callsign>\\b") + kCallPattern + QStringLiteral("(?<type>[> ]))\\b"));
    QString text = c.text;
    const QRegularExpressionMatch match = callTo.match(text);
    if (match.hasMatch()) {
        // Another hop: forward as "NEXT>TEXT *DE* SENDER".
        if (match.captured(QStringLiteral("type")) != QStringLiteral(">"))
            text.replace(match.capturedStart(QStringLiteral("type")), match.capturedLength(QStringLiteral("type")), QStringLiteral(">"));
        action.reply = QStringLiteral("%1 *DE* %2").arg(text, c.from);
        action.note = QStringLiteral("JS8 relay from %1 to %2").arg(c.from, match.captured(QStringLiteral("callsign")).trimmed());
        return action;
    }
    if (c.text.startsWith(QStringLiteral("ACK"))) return action;

    // Final hop: acknowledge, or answer a relayed command.
    const QStringList calls = Js8AutoReply::parseRelayPath(c.from, c.text);
    const QString path = calls.join(QLatin1Char('>'));
    action.reply = QStringLiteral("%1 ACK").arg(path);
    action.note = QStringLiteral("JS8 relayed message via %1: %2").arg(path, c.text);

    QStringList words = c.text.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    if (words.isEmpty() || depth > 2) return action;
    QString first = words.first();
    if (!first.startsWith(QLatin1Char(' '))) first.prepend(QLatin1Char(' '));
    words.removeFirst();
    if (first == QStringLiteral(" MSG") && !words.isEmpty()) {
        if (words.first() == QStringLiteral("TO:")) { first = QStringLiteral(" MSG TO:"); words.removeFirst(); }
        else if (words.first().startsWith(QStringLiteral("TO:"))) { first = QStringLiteral(" MSG TO:"); words[0] = words.first().mid(3); }
    } else if (first == QStringLiteral(" QUERY") && !words.isEmpty()) {
        if (words.first() == QStringLiteral("MSGS") || words.first() == QStringLiteral("MSGS?")) { first = QStringLiteral(" QUERY MSGS"); words.removeFirst(); }
        else if (words.first() == QStringLiteral("CALL")) { first = QStringLiteral(" QUERY CALL"); words.removeFirst(); }
    }
    static const QStringList autoreply = {QStringLiteral(" SNR?"), QStringLiteral(" HEARING?"), QStringLiteral(" GRID?"),
                                          QStringLiteral(" STATUS?"), QStringLiteral(" MSG"), QStringLiteral(" MSG TO:"),
                                          QStringLiteral(" QUERY"), QStringLiteral(" QUERY MSGS"), QStringLiteral(" QUERY CALL"),
                                          QStringLiteral(" INFO?"), QStringLiteral(" AGN?")};
    if (!autoreply.contains(first)) return action;
    Js8Command inner = c;
    inner.command = first;
    inner.text = words.join(QLatin1Char(' '));
    Js8ReplyAction relayed = processImpl(inner, ctx, path, depth + 1);
    relayed.toMe = true;
    if (relayed.note.isEmpty()) relayed.note = action.note;
    return relayed;
}

Js8ReplyAction processImpl(const Js8Command &c, const Js8ReplyContext &ctx, const QString &relayPath, int depth)
{
    Js8ReplyAction action;
    const QString cmd = c.command;
    if (c.from.isEmpty() || c.from.contains(QStringLiteral("....")) || c.to.contains(QStringLiteral("....")))
        return action;
    const bool toMe = Js8AutoReply::isToStation(c.to, ctx.myCall);
    const bool allCall = c.to == QStringLiteral("@ALLCALL");
    const bool heartbeat = cmd == QStringLiteral(" HEARTBEAT") || cmd == QStringLiteral(" HB");
    action.toMe = toMe;
    action.allCall = allCall;
    if (!toMe && !allCall && !heartbeat) return action;
    if (Js8AutoReply::isToStation(c.from, ctx.myCall)) return action;   // own echo

    // Replies to relayed questions go back along the relay path.
    const bool msgOrQuery = cmd.startsWith(QStringLiteral(" MSG")) || cmd.startsWith(QStringLiteral(" QUERY"));
    const QString replyTo = !relayPath.isEmpty() && !msgOrQuery ? relayPath : c.from;
    QString who = c.from;
    QString replyPath = c.from;
    if (relayPath.contains(QLatin1Char('>'))) {
        who = relayPath.split(QLatin1Char('>')).last();
        replyPath = relayPath;
    }

    if ((cmd == QStringLiteral(" SNR?") || cmd == QStringLiteral("?")) && !allCall) {
        action.reply = QStringLiteral("%1 SNR %2").arg(replyTo, Js8Message::formatSnr(c.snrDb));
    } else if (cmd == QStringLiteral(" INFO?") && !allCall) {
        if (!ctx.info.trimmed().isEmpty()) action.reply = QStringLiteral("%1 INFO %2").arg(replyTo, ctx.info.trimmed());
    } else if (cmd == QStringLiteral(" STATUS?") && !allCall) {
        if (!ctx.status.trimmed().isEmpty()) action.reply = QStringLiteral("%1 STATUS %2").arg(replyTo, ctx.status.trimmed());
    } else if (cmd == QStringLiteral(" GRID?") && !allCall) {
        if (!ctx.myGrid.trimmed().isEmpty()) action.reply = QStringLiteral("%1 GRID %2").arg(replyTo, ctx.myGrid.trimmed().toUpper());
    } else if (cmd == QStringLiteral(" HEARING?") && !allCall) {
        QStringList calls;
        for (const QString &call : ctx.heardRecentFirst) {
            if (calls.size() >= 4) break;
            if (call == c.from) continue;
            calls << call;
        }
        calls.prepend(QStringLiteral("%1 HEARING").arg(replyTo));
        action.reply = calls.join(QLatin1Char(' '));
    } else if (cmd == QStringLiteral(">") && !allCall && toMe) {
        if (!ctx.relay) return action;
        return processRelay(c, ctx, action, depth);
    } else if (cmd == QStringLiteral(" MSG TO:") && !allCall && toMe) {
        if (!ctx.relay || ctx.inbox == nullptr) return action;
        QStringList segs = c.text.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        if (segs.isEmpty()) return action;
        const QString to = segs.takeFirst();
        const QString text = segs.join(QLatin1Char(' ')).trimmed();
        const QStringList calls = Js8AutoReply::parseRelayPath(c.from, text);
        const QString path = calls.join(QLatin1Char('>'));
        action.storedId = ctx.inbox->add(storedFrom(c, ctx, Js8Message::baseCallsign(to), path, text,
                                                    Js8StoredMessage::Type::Store));
        action.reply = QStringLiteral("%1 ACK").arg(calls.size() > 1 ? path : c.from);
        action.note = QStringLiteral("JS8 message from %1 stored for %2").arg(c.from, to);
    } else if (cmd == QStringLiteral(" AGN?") && !allCall) {
        if (!ctx.lastTx.trimmed().isEmpty()) action.reply = Js8Message::rstrip(ctx.lastTx);
    } else if (heartbeat) {
        if (!ctx.autoReply || !ctx.heartbeatAck) return action;
        QString extra;
        if (ctx.inbox != nullptr) {
            const int id = ctx.inbox->nextStoredIdFor(c.from);
            if (id > 0) extra = QStringLiteral("MSG ID %1").arg(id);
        }
        action.reply = QStringLiteral("%1 HEARTBEAT SNR %2 %3").arg(c.from, Js8Message::formatSnr(c.snrDb), extra).trimmed();
        action.lowPriority = true;
    } else if (cmd == QStringLiteral(" MSG") && !allCall && toMe) {
        const QStringList calls = Js8AutoReply::parseRelayPath(c.from, c.text);
        const QString path = calls.join(QLatin1Char('>'));
        if (ctx.inbox != nullptr)
            action.inboxId = ctx.inbox->add(storedFrom(c, ctx, ctx.myCall, path, c.text, Js8StoredMessage::Type::Unread));
        action.reply = QStringLiteral("%1 ACK").arg(calls.size() > 1 ? path : c.from);
        action.note = QStringLiteral("JS8 message from %1: %2").arg(c.from, c.text);
    } else if (cmd == QStringLiteral(" QUERY") && !allCall && toMe) {
        QStringList segs = c.text.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        if (segs.size() >= 2 && segs.first() == QStringLiteral("MSG") && ctx.inbox != nullptr) {
            bool ok = false;
            const int id = segs.at(1).toInt(&ok);
            Js8StoredMessage m;
            if (ok && ctx.inbox->takeStoredFor(id, who, &m)) {
                action.reply = QStringLiteral("%1 MSG %2 FROM %3").arg(replyPath, m.text.trimmed(), m.from.trimmed());
                action.note = QStringLiteral("JS8 stored message %1 delivered to %2").arg(id).arg(who);
            }
        }
    } else if ((cmd == QStringLiteral(" QUERY MSGS") || cmd == QStringLiteral(" QUERY MSGS?")) && ctx.autoReply) {
        const int id = ctx.inbox != nullptr ? ctx.inbox->nextStoredIdFor(who) : -1;
        if (id > 0) action.reply = QStringLiteral("%1 YES MSG ID %2").arg(replyPath).arg(id);
        else if (!allCall) action.reply = QStringLiteral("%1 NO").arg(replyPath);
    } else if (cmd == QStringLiteral(" QUERY CALL") && ctx.autoReply) {
        const QStringList calls = Js8Message::parseCallsigns(c.text);
        if (!calls.isEmpty() && ctx.heardReport) {
            const QString report = ctx.heardReport(calls.first()).trimmed();
            if (!report.isEmpty()) action.reply = QStringLiteral("%1 YES %2").arg(replyPath, report);
        }
    }

    // @ALLCALL questions are answered only in AUTO mode.
    if (allCall && !ctx.autoReply) action.reply.clear();
    return action;
}
} // namespace

bool Js8AutoReply::isToStation(const QString &to, const QString &myCall)
{
    const QString t = to.trimmed().toUpper();
    const QString me = myCall.trimmed().toUpper();
    if (t.isEmpty() || me.isEmpty()) return false;
    return t == me || t == Js8Message::baseCallsign(me);
}

QStringList Js8AutoReply::parseRelayPath(const QString &from, const QString &text)
{
    static const QRegularExpression de(QStringLiteral("\\s([*]DE[*]|VIA)\\s(?<callsign>\\b") + kCallPattern + QStringLiteral(")\\b"));
    QStringList calls;
    auto it = de.globalMatch(text);
    while (it.hasNext()) calls.prepend(it.next().captured(QStringLiteral("callsign")));
    calls.prepend(from);
    return calls;
}

QString Js8AutoReply::since(const QDateTime &time, const QDateTime &now)
{
    qint64 delta = time.toUTC().secsTo(now.toUTC());
    if (delta < 15) return QStringLiteral("now");
    const qint64 seconds = delta % 60;
    delta /= 60;
    const qint64 minutes = delta % 60;
    delta /= 60;
    const qint64 hours = delta % 24;
    const qint64 days = delta / 24;
    if (days) return QStringLiteral("%1d").arg(days);
    if (hours) return QStringLiteral("%1h").arg(hours);
    if (minutes) return QStringLiteral("%1m").arg(minutes);
    if (seconds) return QStringLiteral("%1s").arg(seconds - seconds % 15);
    return QString();
}

Js8ReplyAction Js8AutoReply::process(const Js8Command &command, const Js8ReplyContext &context)
{
    return processImpl(command, context, QString(), 0);
}
