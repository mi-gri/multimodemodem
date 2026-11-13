#include "modems/ft8/FtDecodedText.h"
#include "modems/ft8/FtQsoSession.h"
#include "modems/ft8/FtQsoSequencer.h"

#include <QCoreApplication>
#include <QDebug>
#include <QString>

namespace {

using Seq = FtQsoSequencer;

struct Expect
{
    QString name;
    QString message;
    Seq::State state = Seq::State::Idle;
    bool qsoActive = true;
    QString dxCall = QStringLiteral("PT2ND");
    Seq::Action action = Seq::Action::None;
    Seq::State nextState = Seq::State::Idle;
    int txRow = -1;
    bool txActive = false;
    QString lastTxMessage;
};

QString actionName(Seq::Action a)
{
    switch (a) {
    case Seq::Action::None: return QStringLiteral("None");
    case Seq::Action::Ignore: return QStringLiteral("Ignore");
    case Seq::Action::ArmRow: return QStringLiteral("ArmRow");
    case Seq::Action::InterruptAndArmRow: return QStringLiteral("InterruptAndArmRow");
    case Seq::Action::RetryCurrent: return QStringLiteral("RetryCurrent");
    case Seq::Action::Complete: return QStringLiteral("Complete");
    case Seq::Action::StopTx: return QStringLiteral("StopTx");
    }
    return QStringLiteral("?");
}

bool runCase(const Expect &e)
{
    Seq::Context c;
    c.myCall = QStringLiteral("IZ6NNH");
    c.dxCall = e.dxCall;
    c.qsoActive = e.qsoActive;
    c.cqRepeatActive = false;
    c.state = e.state;
    c.rxAudioFreqHz = 1500;
    c.txAudioFreqHz = 1500;
    c.txActive = e.txActive;
    c.lastTxMessage = e.lastTxMessage;

    Seq::Decode d;
    d.message = e.message;
    d.snrDb = -10;
    d.frequencyHz = 1500;

    const Seq::Decision out = Seq::evaluateDecode(d, c);
    const bool ok = out.action == e.action && out.nextState == e.nextState && out.txRow == e.txRow;
    if (!ok) {
        qWarning().noquote() << "FAIL" << e.name
                             << "msg=" << e.message
                             << "got action" << actionName(out.action)
                             << "state" << static_cast<int>(out.nextState)
                             << "row" << out.txRow
                             << "expected" << actionName(e.action)
                             << static_cast<int>(e.nextState)
                             << e.txRow;
    }
    return ok;
}

bool runParserTrapChecks()
{
    bool ok = true;
    const auto rr79 = FtDecodedText::parse(QStringLiteral("IZ6NNH PT2ND RR79"), QStringLiteral("IZ6NNH"));
    if (!rr79.grid.isEmpty() || rr79.rr73 || rr79.final73) {
        qWarning().noquote() << "FAIL parser: RR79 must not become grid/final" << rr79.grid << rr79.text;
        ok = false;
    }

    const auto compound = FtDecodedText::parse(QStringLiteral("EA/IZ6NNH PT2ND -10"), QStringLiteral("IZ6NNH"));
    if (!compound.addressedToMe || compound.senderCall != QStringLiteral("PT2ND") || compound.report != QStringLiteral("-10")) {
        qWarning().noquote() << "FAIL parser: compound addressed call" << compound.text << compound.addressedToMe << compound.senderCall << compound.report;
        ok = false;
    }

    const auto bracketed = FtDecodedText::parse(QStringLiteral("<IZ6NNH> PT2ND -10"), QStringLiteral("IZ6NNH"));
    if (!bracketed.addressedToMe || bracketed.senderCall != QStringLiteral("PT2ND") || bracketed.report != QStringLiteral("-10")) {
        qWarning().noquote() << "FAIL parser: bracketed hash call" << bracketed.text << bracketed.addressedToMe << bracketed.senderCall << bracketed.report;
        ok = false;
    }

    // Unresolved hash call as receiver: sender and grid/report still count,
    // never addressed to us.
    const auto unknownGrid = FtDecodedText::parse(QStringLiteral("<...> DL7UKT JO62"), QStringLiteral("IZ6NNH"));
    if (!unknownGrid.directed || unknownGrid.addressedToMe || unknownGrid.senderCall != QStringLiteral("DL7UKT") ||
        unknownGrid.grid != QStringLiteral("JO62") || !unknownGrid.receiverCall.isEmpty()) {
        qWarning().noquote() << "FAIL parser: <...> receiver with grid" << unknownGrid.text << unknownGrid.senderCall << unknownGrid.grid;
        ok = false;
    }
    const auto unknownReport = FtDecodedText::parse(QStringLiteral("<...> DJ1KW R-07"), QStringLiteral("DJ1KW"));
    if (unknownReport.addressedToMe || unknownReport.senderCall != QStringLiteral("DJ1KW") || unknownReport.report != QStringLiteral("-07")) {
        qWarning().noquote() << "FAIL parser: <...> receiver with report" << unknownReport.text << unknownReport.addressedToMe << unknownReport.report;
        ok = false;
    }

    // Callsign recognition: special event calls with several digits, and
    // tokens that must stay non-calls.
    for (const QString &call : {QStringLiteral("DL75WAU"), QStringLiteral("SX602N"), QStringLiteral("TM100A"),
                                QStringLiteral("GB250RN"), QStringLiteral("K1ABC"), QStringLiteral("2E0ABC"),
                                QStringLiteral("3DA0RS"), QStringLiteral("E51ABC"), QStringLiteral("DL75WAU/P"),
                                // special calls whose prefix starts with a digit
                                QStringLiteral("9A100ZG"), QStringLiteral("4X75IDF"), QStringLiteral("4Z70ISR"),
                                QStringLiteral("9A2025TS"), QStringLiteral("3Z100PL"), QStringLiteral("2E25JUB"),
                                QStringLiteral("E750A"), QStringLiteral("5B60CY"), QStringLiteral("9H50VE")}) {
        if (!FtDecodedText::isCallsign(call)) { qWarning().noquote() << "FAIL callsign rejected:" << call; ok = false; }
    }
    for (const QString &token : {QStringLiteral("1A"), QStringLiteral("599"), QStringLiteral("JO62"),
                                 QStringLiteral("RR73"), QStringLiteral("-10"), QStringLiteral("R-07"), QStringLiteral("FT8"),
                                 QStringLiteral("73"), QStringLiteral("CQ")}) {
        if (FtDecodedText::isCallsign(token)) { qWarning().noquote() << "FAIL not a callsign accepted:" << token; ok = false; }
    }
    const auto plainSpecial = FtDecodedText::parse(QStringLiteral("SX602N IW9HPG JM78"), QStringLiteral("IZ6NNH"));
    if (!plainSpecial.directed || plainSpecial.senderCall != QStringLiteral("IW9HPG") || plainSpecial.grid != QStringLiteral("JM78")) {
        qWarning().noquote() << "FAIL parser: special call without brackets" << plainSpecial.text << plainSpecial.senderCall << plainSpecial.grid;
        ok = false;
    }
    const auto cqSpecial = FtDecodedText::parse(QStringLiteral("CQ DL75WAU JO62"), QStringLiteral("IZ6NNH"));
    if (!cqSpecial.cq || cqSpecial.senderCall != QStringLiteral("DL75WAU") || cqSpecial.grid != QStringLiteral("JO62")) {
        qWarning().noquote() << "FAIL parser: CQ from special call" << cqSpecial.text << cqSpecial.senderCall << cqSpecial.grid;
        ok = false;
    }

    // Unresolved hash call as sender: receiver and payload count, nothing is
    // addressed to us (no sender to answer).
    const auto unknownSender = FtDecodedText::parse(QStringLiteral("RA6ABC <...> -08"), QStringLiteral("IZ6NNH"));
    if (!unknownSender.directed || unknownSender.receiverCall != QStringLiteral("RA6ABC") || !unknownSender.senderCall.isEmpty() ||
        unknownSender.report != QStringLiteral("-08") || unknownSender.addressedToMe) {
        qWarning().noquote() << "FAIL parser: <...> sender" << unknownSender.text << unknownSender.receiverCall << unknownSender.report;
        ok = false;
    }
    const auto unknownSenderToMe = FtDecodedText::parse(QStringLiteral("IZ6NNH <...> RR73"), QStringLiteral("IZ6NNH"));
    if (unknownSenderToMe.addressedToMe || !unknownSenderToMe.rr73 || !unknownSenderToMe.directed) {
        qWarning().noquote() << "FAIL parser: <...> sender to me must not be addressed" << unknownSenderToMe.text;
        ok = false;
    }
    const auto bothUnknown = FtDecodedText::parse(QStringLiteral("<...> <...> R 570400 IB88WR"), QStringLiteral("IZ6NNH"));
    if (bothUnknown.directed || bothUnknown.addressedToMe) {
        qWarning().noquote() << "FAIL parser: two unknown calls are not a directed message" << bothUnknown.text;
        ok = false;
    }

    // DXpedition (Fox) double message whose first call is an unresolved hash.
    const auto fox = FtDecodedText::parse(QStringLiteral("<...> RR73; KN5XZO <NA2J> -02"), QStringLiteral("IZ6NNH"));
    if (!fox.directed || fox.receiverCall != QStringLiteral("KN5XZO") || fox.senderCall != QStringLiteral("NA2J") || fox.report != QStringLiteral("-02")) {
        qWarning().noquote() << "FAIL parser: Fox double message with <...>" << fox.text << fox.receiverCall << fox.senderCall << fox.report;
        ok = false;
    }

    // Resolved hash call in brackets.
    const auto special = FtDecodedText::parse(QStringLiteral("<SX602N> IW9HPG JM78"), QStringLiteral("IZ6NNH"));
    if (!special.directed || special.receiverCall != QStringLiteral("SX602N") || special.senderCall != QStringLiteral("IW9HPG") ||
        special.grid != QStringLiteral("JM78")) {
        qWarning().noquote() << "FAIL parser: special hashed receiver" << special.text << special.receiverCall << special.senderCall << special.grid;
        ok = false;
    }
    const auto specialSender = FtDecodedText::parse(QStringLiteral("IZ6NNH <SX602N> -10"), QStringLiteral("IZ6NNH"));
    if (!specialSender.addressedToMe || specialSender.senderCall != QStringLiteral("SX602N") || specialSender.report != QStringLiteral("-10")) {
        qWarning().noquote() << "FAIL parser: special hashed sender" << specialSender.text << specialSender.senderCall << specialSender.report;
        ok = false;
    }
    const auto specialCq = FtDecodedText::parse(QStringLiteral("CQ <SX602N> KM18"), QStringLiteral("IZ6NNH"));
    if (!specialCq.cq || specialCq.senderCall != QStringLiteral("SX602N") || specialCq.grid != QStringLiteral("KM18")) {
        qWarning().noquote() << "FAIL parser: special hashed CQ" << specialCq.text << specialCq.senderCall << specialCq.grid;
        ok = false;
    }

    const auto directedCq = FtDecodedText::parse(QStringLiteral("CQ DX K1ABC FN42"), QStringLiteral("IZ6NNH"));
    if (!directedCq.cq || directedCq.senderCall != QStringLiteral("K1ABC") || directedCq.grid != QStringLiteral("FN42")) {
        qWarning().noquote() << "FAIL parser: directed CQ" << directedCq.text << directedCq.senderCall << directedCq.grid;
        ok = false;
    }
    return ok;
}

bool runLastResponseWindowChecks()
{
    bool ok = true;
    auto check = [&ok](bool value, const char *name) {
        if (!value) qWarning() << "FAIL response window:" << name;
        ok = value && ok;
    };
    for (int period : {15000, 7500}) {
        for (int attempts : {1, 3}) {
            FtQsoSession session;
            session.startQso("LZ2KSQ", "KN23", 1033);
            session.reportSent = "R-15";
            session.reportReceived = "-02";
            session.state = Seq::State::WaitingFinal73;
            session.beginRetry("LZ2KSQ IZ6NNH R-15", "SEQ", attempts);
            const quint64 epoch = session.retryGeneration;
            for (int i = 0; i < attempts; ++i) {
                session.lastTxPlan.message = session.retryMessage;
                session.lastTxPlan.slotBoundaryUtcMs = 1792310000000LL + 2LL*i*period;
                check(session.noteRetryTxCompleted(period), "TX counted once");
                check(!session.noteRetryTxCompleted(period), "duplicate callbacks do not consume attempts");
                check(session.retryRemaining == attempts-i-1, "exact configured attempt limit");
                check(session.retryReplySlotUtcMs == session.lastTxPlan.slotBoundaryUtcMs+period,
                      "last opposite RX slot remains owned");
                check(session.qsoActive && session.state == Seq::State::WaitingFinal73,
                      "final transmission does not erase QSO before listening");
            }
            Seq::Decode reply;
            reply.message = "IZ6NNH LZ2KSQ RR73";
            reply.frequencyHz = 1033;
            reply.slotStartUtcMs = session.retryReplySlotUtcMs;
            const auto decision = Seq::evaluateDecode(reply, session.makeContext("IZ6NNH",1033,1033));
            check(decision.action == Seq::Action::ArmRow && decision.txRow == 5,
                  "RR73 after last permitted TX still arms final 73");
            check(session.reportReceived == "-02", "reports survive the last RX window");
            session.beginRetry("LZ2KSQ IZ6NNH 73", "SEQ", attempts);
            check(session.retryGeneration != epoch && session.retryReplySlotUtcMs == 0,
                  "new exchange invalidates old timeout");
            check(!session.noteRetryTxCompleted(period), "old TX cannot consume new message budget");
            session.beginRetry(session.lastTxPlan.message, "SEQ", attempts);
            check(!session.noteRetryTxCompleted(period) && session.retryRemaining == attempts,
                  "rearming identical text does not recount an old TX before audio");
            const auto newEpoch = session.retryGeneration;
            session.clearQso();
            check(!session.qsoActive && session.retryGeneration != newEpoch &&
                  session.retryReplySlotUtcMs == 0, "STOP/QSO reset invalidates timeout");
        }
    }
    return ok;
}

} // namespace

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    Q_UNUSED(app);

    bool ok = runParserTrapChecks();
    ok = runLastResponseWindowChecks() && ok;
    const QList<Expect> cases = {
        {QStringLiteral("locator reply -> Tx3 report"), QStringLiteral("IZ6NNH PT2ND JN63"), Seq::State::WaitingDxReply, true, QStringLiteral("PT2ND"), Seq::Action::ArmRow, Seq::State::SendingReport, 2},
        {QStringLiteral("report reply -> Tx4 R-report"), QStringLiteral("IZ6NNH PT2ND -10"), Seq::State::WaitingReport, true, QStringLiteral("PT2ND"), Seq::Action::ArmRow, Seq::State::SendingRReport, 3},
        {QStringLiteral("R-report -> Tx5 RR73"), QStringLiteral("IZ6NNH PT2ND R-10"), Seq::State::WaitingRReport, true, QStringLiteral("PT2ND"), Seq::Action::ArmRow, Seq::State::SendingRr73, 4},
        {QStringLiteral("RR73 -> Tx6 final 73"), QStringLiteral("IZ6NNH PT2ND RR73"), Seq::State::WaitingFinal73, true, QStringLiteral("PT2ND"), Seq::Action::ArmRow, Seq::State::SendingRr73, 5},
        {QStringLiteral("RR73 during obsolete R-report TX -> interrupt and arm one 73"), QStringLiteral("IZ6NNH PT2ND RR73"), Seq::State::SendingRReport, true, QStringLiteral("PT2ND"), Seq::Action::InterruptAndArmRow, Seq::State::SendingRr73, 5, true, QStringLiteral("PT2ND IZ6NNH R-10")},
        {QStringLiteral("plain 73 during obsolete R-report TX -> interrupt and arm one 73"), QStringLiteral("IZ6NNH PT2ND 73"), Seq::State::SendingRReport, true, QStringLiteral("PT2ND"), Seq::Action::InterruptAndArmRow, Seq::State::SendingRr73, 5, true, QStringLiteral("PT2ND IZ6NNH R-10")},
        {QStringLiteral("repeated RR73 while final 73 is on air -> no additional TX"), QStringLiteral("IZ6NNH PT2ND RR73"), Seq::State::SendingRr73, true, QStringLiteral("PT2ND"), Seq::Action::None, Seq::State::SendingRr73, -1, true, QStringLiteral("PT2ND IZ6NNH 73")},
        {QStringLiteral("RR73 after final 73 finished -> complete"), QStringLiteral("IZ6NNH PT2ND RR73"), Seq::State::SendingRr73, true, QStringLiteral("PT2ND"), Seq::Action::Complete, Seq::State::Completed, -1, false, QStringLiteral("PT2ND IZ6NNH 73")},
        {QStringLiteral("own transmitted echo ignored"), QStringLiteral("PT2ND IZ6NNH -10"), Seq::State::WaitingReport, true, QStringLiteral("PT2ND"), Seq::Action::Ignore, Seq::State::WaitingReport, -1},
        {QStringLiteral("contest exchange starts report phase"), QStringLiteral("IZ6NNH PT2ND 1A WI"), Seq::State::WaitingDxReply, true, QStringLiteral("PT2ND"), Seq::Action::ArmRow, Seq::State::SendingReport, 2},
        {QStringLiteral("contest R grid ack -> RR73"), QStringLiteral("IZ6NNH PT2ND R JN63"), Seq::State::WaitingRReport, true, QStringLiteral("PT2ND"), Seq::Action::ArmRow, Seq::State::SendingRr73, 4},
    };

    for (const Expect &e : cases) {
        ok = runCase(e) && ok;
    }

    if (ok) {
        qInfo().noquote() << "FtSequencerSelfTest: all regression cases passed";
        return 0;
    }
    return 1;
}
