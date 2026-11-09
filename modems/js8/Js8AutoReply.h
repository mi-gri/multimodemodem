// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef JS8AUTOREPLY_H
#define JS8AUTOREPLY_H

#include "Js8CommandBuffer.h"
#include "Js8Inbox.h"

#include <QString>
#include <QStringList>
#include <functional>

/// Station state the automatic replies draw on.
struct Js8ReplyContext
{
    QString myCall;
    QString myGrid;
    QString info;               // answer to INFO?
    QString status;             // answer to STATUS?
    bool autoReply = false;     // AUTO: transmit replies without asking
    bool heartbeatAck = false;  // answer heartbeats with HEARTBEAT SNR
    bool relay = true;          // forward ">" relays and keep MSG TO: messages
    QStringList heardRecentFirst;                         // for HEARING?
    std::function<QString(const QString &)> heardReport;  // QUERY CALL: "+05 (12m)" or empty
    QString lastTx;             // for AGN?
    Js8Inbox *inbox = nullptr;
    double dialHz = 0.0;
};

/// What the station does with one received command.
struct Js8ReplyAction
{
    QString reply;            // text to transmit; empty = nothing
    bool lowPriority = false; // heartbeat acknowledgement
    bool toMe = false;        // addressed to the own call (or relayed to it)
    bool allCall = false;
    int inboxId = -1;         // MSG stored in the own inbox
    int storedId = -1;        // MSG TO: kept for another station
    QString note;             // one line for the log
};

/**
 * @brief Directed-command replies of JS8Call 2.2.0 (processCommandActivity).
 *
 * SNR?, GRID?, INFO?, STATUS?, HEARING?, AGN?, QUERY MSGS, QUERY MSG n,
 * QUERY CALL, MSG, MSG TO:, relay ">" and heartbeat acknowledgements.
 * Messages are stored even without AUTO; replies are only produced with
 * AUTO enabled, so nothing is transmitted unattended otherwise.
 */
class Js8AutoReply
{
public:
    static Js8ReplyAction process(const Js8Command &command, const Js8ReplyContext &context);

    static bool isToStation(const QString &to, const QString &myCall);
    /// Relay path from the sender and "*DE* CALL" / "VIA CALL" marks: [from, ...].
    static QStringList parseRelayPath(const QString &from, const QString &text);
    /// JS8Call's since(): "now", "45s", "12m", "3h", "2d".
    static QString since(const QDateTime &time, const QDateTime &now);
};

#endif // JS8AUTOREPLY_H
