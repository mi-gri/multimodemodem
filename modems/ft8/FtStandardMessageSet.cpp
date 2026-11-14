#include "FtStandardMessageSet.h"

#include "FtDecodedText.h"

#include <QtGlobal>
#include <QRegularExpression>

// Compiled once instead of on every call (several run per decode).
namespace {
const QRegularExpression kRegexCqTarget(QStringLiteral("^[A-Z]{1,4}$"));
}

namespace {

// WSJT-X MainWindow::stdCall(): calls that fit the 28-bit standard field.
// Others (II4GCLB, DL/N0PNG) must be sent hashed as <CALL> or in a type 4
// message; a plain "II4GCLB DO3BOX JO30" packs as truncated free text.
const QRegularExpression kRegexStandardCall(
    QStringLiteral("^([A-Z]{0,2}|[A-Z][0-9]|[0-9][A-Z])([0-9][A-Z]{0,3})(/R|/P)?$"));

QString normCall(QString s)
{
    return s.trimmed().toUpper();
}

QString normGrid(QString s)
{
    s = s.trimmed().left(4).toUpper();
    return FtDecodedText::isGrid(s) ? s : QString();
}

QString normReport(QString s, bool acknowledged)
{
    s = s.trimmed().toUpper();
    if (s.isEmpty()) {
        s = QStringLiteral("-10");
    }
    if (acknowledged) {
        if (!s.startsWith(QLatin1Char('R'))) {
            s.prepend(QLatin1Char('R'));
        }
        const QString cleaned = FtDecodedText::cleanReport(s);
        return FtDecodedText::isReport(QStringLiteral("R") + cleaned) ? QStringLiteral("R") + cleaned : QStringLiteral("R-10");
    }
    s = FtDecodedText::cleanReport(s);
    return FtDecodedText::isReport(s) ? s : QStringLiteral("-10");
}

} // namespace

void FtStandardMessageSet::rebuild(const Inputs &inputs)
{
    m_myCall = normCall(inputs.myCall);
    m_myGrid = normGrid(inputs.myGrid);
    m_dxCall = normCall(inputs.dxCall);
    m_dxGrid = normGrid(inputs.dxGrid);
    m_report = normReport(inputs.report, false);
    m_rReport = normReport(inputs.rReport.isEmpty() ? inputs.report : inputs.rReport, true);
    m_audioFrequencyHz = inputs.audioFrequencyHz;

    const QString dx = m_dxCall.isEmpty() ? QStringLiteral("DXCALL") : m_dxCall;
    m_messages.clear();

    m_valid = !m_myCall.isEmpty() && !m_myGrid.isEmpty();
    if (!m_valid) {
        for (int i = 0; i < RowCount; ++i) {
            m_messages << QString();
        }
        return;
    }

    const QString target = inputs.cqTarget.trimmed().toUpper();
    const QString cq = kRegexCqTarget.match(target).hasMatch()
        ? QStringLiteral("CQ %1").arg(target) : QStringLiteral("CQ");
    const bool hisStd = m_dxCall.isEmpty() || isStandardCall(m_dxCall);
    const bool myStd = isStandardCall(m_myCall);
    const QString hashedDx = QStringLiteral("<%1>").arg(dx);
    const QString hashedMy = QStringLiteral("<%1>").arg(m_myCall);
    if (hisStd && myStd) {
        m_messages << QStringLiteral("%1 %2 %3").arg(cq, m_myCall, m_myGrid)
                   << QStringLiteral("%1 %2 %3").arg(dx, m_myCall, m_myGrid)
                   << QStringLiteral("%1 %2 %3").arg(dx, m_myCall, m_report)
                   << QStringLiteral("%1 %2 %3").arg(dx, m_myCall, m_rReport)
                   << QStringLiteral("%1 %2 RR73").arg(dx, m_myCall)
                   << QStringLiteral("%1 %2 73").arg(dx, m_myCall);
    } else if (myStd) {
        // WSJT-X genStdMsgs, non-standard DX call: hash it where a grid or
        // report is needed; RR73/73 go as type 4 with my call hashed.
        m_messages << QStringLiteral("%1 %2 %3").arg(cq, m_myCall, m_myGrid)
                   << QStringLiteral("%1 %2 %3").arg(hashedDx, m_myCall, m_myGrid)
                   << QStringLiteral("%1 %2 %3").arg(hashedDx, m_myCall, m_report)
                   << QStringLiteral("%1 %2 %3").arg(hashedDx, m_myCall, m_rReport)
                   << QStringLiteral("%1 %2 RR73").arg(dx, hashedMy)
                   << QStringLiteral("%1 %2 73").arg(dx, hashedMy);
    } else {
        // Non-standard own call (WSJT-X): CQ without grid, answer without
        // grid, reports with my call hashed, RR73/73 with the DX hashed.
        m_messages << QStringLiteral("%1 %2").arg(cq, m_myCall)
                   << QStringLiteral("%1 %2").arg(hashedDx, m_myCall)
                   << QStringLiteral("%1 %2 %3").arg(dx, hashedMy, m_report)
                   << QStringLiteral("%1 %2 %3").arg(dx, hashedMy, m_rReport)
                   << QStringLiteral("%1 %2 RR73").arg(hashedDx, m_myCall)
                   << QStringLiteral("%1 %2 73").arg(hashedDx, m_myCall);
    }
}

bool FtStandardMessageSet::isStandardCall(const QString &call)
{
    return kRegexStandardCall.match(call.trimmed().toUpper()).hasMatch();
}

QString FtStandardMessageSet::message(int row) const
{
    if (row < 0 || row >= m_messages.size()) {
        return QString();
    }
    return m_messages.at(row).trimmed().toUpper();
}

int FtStandardMessageSet::findRow(const QString &message) const
{
    const QString target = message.trimmed().toUpper();
    if (target.isEmpty()) {
        return -1;
    }
    for (int row = 0; row < m_messages.size(); ++row) {
        if (m_messages.at(row).trimmed().toUpper() == target) {
            return row;
        }
    }
    return -1;
}

FtTxPlan FtStandardMessageSet::makePlan(int row, const QString &tag) const
{
    FtTxPlan plan;
    plan.row = row;
    plan.message = message(row);
    plan.tag = tag.trimmed().isEmpty() ? QStringLiteral("SEQ") : tag.trimmed().toUpper();
    plan.dxCall = m_dxCall;
    plan.dxGrid = m_dxGrid;
    plan.reportSent = (row == Tx4RReport) ? m_rReport : m_report;
    plan.reportReceived.clear();
    plan.audioFrequencyHz = m_audioFrequencyHz;
    plan.autoSequence = (plan.tag == QStringLiteral("SEQ") || plan.tag == QStringLiteral("RETRY") || plan.tag == QStringLiteral("CQ"));
    plan.retry = (plan.tag == QStringLiteral("RETRY"));
    plan.finalMessage = (row == Tx5Rr73 || row == Tx6Final73);
    return plan;
}

FtTxPlan FtStandardMessageSet::makePlanForMessage(const QString &messageText, const QString &tag) const
{
    const int row = findRow(messageText);
    if (row >= 0) {
        FtTxPlan plan = makePlan(row, tag);
        plan.message = messageText.trimmed().toUpper();
        return plan;
    }

    FtTxPlan plan;
    plan.row = -1;
    plan.message = messageText.trimmed().toUpper();
    plan.tag = tag.trimmed().isEmpty() ? QStringLiteral("TX") : tag.trimmed().toUpper();
    plan.dxCall = m_dxCall;
    plan.dxGrid = m_dxGrid;
    plan.reportSent = m_report;
    plan.audioFrequencyHz = m_audioFrequencyHz;
    plan.autoSequence = (plan.tag == QStringLiteral("SEQ") || plan.tag == QStringLiteral("RETRY") || plan.tag == QStringLiteral("CQ"));
    plan.retry = (plan.tag == QStringLiteral("RETRY"));
    return plan;
}
