// SPDX-License-Identifier: GPL-3.0-or-later
// Band TX policy (Settings > Bands), the manual dial frequency without CAT,
// the quick band buttons and the band order of the band selectors.

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "modems/ft8/Ft8Mode.h"
#include "modems/js8/Js8Mode.h"
#include "modems/wspr/WsprMode.h"
#include "rig/HamlibController.h"
#include "runtime/SessionDiagnostics.h"
#include "settings/AppSettings.h"
#include "utils/BandPolicy.h"
#include "utils/CockpitTheme.h"
#include "utils/StandardFrequencies.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSettings>
#include <QToolButton>

namespace {
}

// Identifies "where the rig is" for the receive-only confirmation: the amateur
// band, outside the bands a 100 kHz bucket (one confirmation never covers a
// QSY from 4.6 MHz to 27.5 MHz).
QString MainWindow::txBandKey(double hz)
{
    const QString band = M3StandardFrequencies::bandOf(hz);
    return band.isEmpty() ? QStringLiteral("other:%1").arg(qRound64(hz / 100000.0)) : band;
}

// Dial frequency the next transmission goes out on: CAT (the RX dial while an
// FT split is prepared) or, without CAT, the frequency the operator entered.
// 0: unknown - then nothing transmits.
double MainWindow::txReferenceDialHz() const
{
    if (m_settings.hamlibCatEnabled) {
        if (m_rigController == nullptr || !m_rigController->isConnected() || m_lastRigFrequencyHz <= 0.0) return 0.0;
        return m_lastRxDialHz > 0.0 ? m_lastRxDialHz : m_lastRigFrequencyHz;
    }
    return m_manualDialHz;
}

QString MainWindow::currentTxBand() const
{
    return M3StandardFrequencies::bandOf(txReferenceDialHz());
}

bool MainWindow::txBandPermitted(bool automatic) const
{
    const double hz = txReferenceDialHz();
    if (hz <= 0.0) return false;
    const QString band = M3StandardFrequencies::bandOf(hz);
    if (M3BandPolicy::txAllowedOn(m_settings.bandsConfigured, m_settings.txBands, band)) return true;
    if (automatic) return false;
    return m_txBandOverride == txBandKey(hz);
}

QString MainWindow::txBandRefusalReason() const
{
    const double hz = txReferenceDialHz();
    if (hz <= 0.0) {
        return m_settings.hamlibCatEnabled
            ? uiText("tx_band_no_cat_frequency", "CAT reports no dial frequency (rig not connected). Without a known frequency m³ does not transmit.")
            : uiText("tx_band_no_manual_frequency", "Without CAT, enter the dial frequency first (field \"Frequency\" above the tabs). Without a known frequency m³ does not transmit.");
    }
    const QString band = M3StandardFrequencies::bandOf(hz);
    if (band.isEmpty())
        return uiText("tx_band_outside", "%1 is outside the amateur bands.").arg(formatRigFrequency(hz));
    return uiText("tx_band_rx_only", "TX is not allowed on %1 (receive only, Settings → Bands).").arg(band);
}

// Manual transmissions: asks once per band when the band is receive only;
// refuses without a known frequency.  Automatic transmissions never get here
// with a forbidden band (autoTxPermitted).
bool MainWindow::confirmTxBand(const QString &what)
{
    if (txBandPermitted(false)) return true;
    const QString label = what.trimmed().isEmpty() ? QStringLiteral("TX") : what.trimmed();
    const double hz = txReferenceDialHz();
    const QString reason = txBandRefusalReason();
    if (hz <= 0.0) {
        appendLog(QStringLiteral("%1 blocked: %2").arg(label, reason));
        SessionDiagnostics::instance().record("tx.band.blocked", {{"what", label}, {"reason", "no_frequency"}});
        QMessageBox::warning(this, uiText("tx_blocked_title", "TX/PTT blocked"), reason);
        if (!m_settings.hamlibCatEnabled && m_editManualFrequency != nullptr) {
            m_editManualFrequency->setFocus();
            m_editManualFrequency->selectAll();
        }
        return false;
    }
    const QString band = M3StandardFrequencies::bandOf(hz);
    QMessageBox box(QMessageBox::Warning, uiText("tx_band_confirm_title", "TX on a receive-only band"),
                    reason + QStringLiteral("\n\n") + uiText("tx_band_confirm_question", "Transmit anyway?"),
                    QMessageBox::Yes | QMessageBox::No, this);
    box.setInformativeText(uiText("tx_band_confirm_info", "The answer holds until the band changes. Automatic transmissions stay off on this band."));
    box.setDefaultButton(QMessageBox::No);
    box.setObjectName(QStringLiteral("txBandConfirm"));
    const bool yes = box.exec() == QMessageBox::Yes;
    SessionDiagnostics::instance().record("tx.band.confirm", {{"what", label}, {"band", band}, {"frequency_hz", hz}, {"confirmed", yes}});
    if (!yes) {
        appendLog(QStringLiteral("%1 not sent: %2").arg(label, reason));
        return false;
    }
    m_txBandOverride = txBandKey(hz);
    appendLog(uiText("tx_band_confirmed_log", "TX on %1 confirmed by the operator (band is receive only); valid until the band changes.")
                  .arg(band.isEmpty() ? formatRigFrequency(hz) : band));
    return true;
}

void MainWindow::logAutoTxBandRefusal(const QString &key) const
{
    // Once per band/reason, not on every timer tick.
    if (m_autoTxBlockLoggedKey == key) return;
    m_autoTxBlockLoggedKey = key;
    auto *self = const_cast<MainWindow *>(this);
    self->appendLog(uiText("tx_band_auto_blocked_log", "Automatic TX paused: %1").arg(txBandRefusalReason()));
    SessionDiagnostics::instance().record("tx.band.auto_blocked", {{"key", key}});
}

// Everything that would transmit on: a band change during a transmission or a
// running sequence never carries it over to the new band, allowed or not.
void MainWindow::abortTxForSwr(double swr)
{
    const bool ftActive = m_ftTxWorkerRunning || m_ft8PendingTxArmed || m_hasDeferredFt8TxPlan ||
                          m_pendingFt8PttKeyed || m_ftSession.qsoActive || m_ftSession.cqRepeatActive;
    const bool wsprOn = m_chkWsprTx != nullptr && m_chkWsprTx->isChecked();
    const QString text = uiText("tx_swr_abort_log", "SWR %1 above the limit %2: transmission stopped; automatic operation stays stopped until you start it again. Check antenna and tuner.")
                             .arg(swr, 0, 'f', 1).arg(m_settings.txSwrAbortLimit, 0, 'f', 1);
    appendLog(text);
    SessionDiagnostics::instance().record("tx.swr_abort", {{"swr", swr}, {"limit", m_settings.txSwrAbortLimit}});
    // Nothing automatic keys up again on the same antenna: FT stop latch, beacon off,
    // JS8 automation held (as by its watchdog) until the next operator action.
    ++m_ftCqResumeGen;
    m_ft8OperatorStopLatched = true;
    m_js8WatchdogTripped = true;
    if (wsprOn) wsprStopBeacon(QStringLiteral("high SWR"), false);
    if (!m_js8TxQueue.isEmpty()) m_js8TxQueue.clear();
    if (m_nativeWeakSignalTxPending) cancelNativeWeakSignalPeriodTx(QStringLiteral("high SWR"));
    if (ftActive) {
        stopFt8ShellImpl(false);
        stopFt8Sequencer(QStringLiteral("high SWR"));
    }
    if (m_txRunning || m_txPreparationPending || m_cwSerialActive) stopImageTx();
    updateTxControlState();
    auto *box = new QMessageBox(QMessageBox::Warning, uiText("tx_swr_abort_title", "High SWR"), text, QMessageBox::Ok, this);
    box->setAttribute(Qt::WA_DeleteOnClose);
    box->setModal(false);
    box->show();
}

void MainWindow::abortTxForBandChange(const QString &from, const QString &to)
{
    ++m_ftCqResumeGen;
    m_txBandOverride.clear();
    m_autoTxBlockLoggedKey.clear();
    pauseAutoStage(QStringLiteral("band change"));   // new situation: the operator selects the Auto stage again
    const bool ftActive = m_ftTxWorkerRunning || m_ft8PendingTxArmed || m_hasDeferredFt8TxPlan ||
                          m_pendingFt8PttKeyed || m_ftSession.qsoActive || m_ftSession.cqRepeatActive;
    const bool wsprOn = m_chkWsprTx != nullptr && m_chkWsprTx->isChecked();
    const bool active = m_txRunning || m_txPreparationPending || m_lastRigPttOn || ftActive ||
                        m_nativeWeakSignalTxPending || !m_js8TxQueue.isEmpty() || wsprOn || m_cwSerialActive;
    if (!active) return;
    const QString fromText = from.isEmpty() ? QStringLiteral("?") : from;
    const QString toText = to.isEmpty() ? QStringLiteral("?") : to;
    appendLog(uiText("tx_band_change_abort_log", "Band change %1 → %2 during TX: transmission stopped, nothing continues on the new band.")
                  .arg(fromText, toText));
    SessionDiagnostics::instance().record("tx.band.change_abort", {{"from", from}, {"to", to}});
    if (wsprOn) wsprStopBeacon(QStringLiteral("band change"), false);
    if (!m_js8TxQueue.isEmpty()) m_js8TxQueue.clear();
    if (m_nativeWeakSignalTxPending) cancelNativeWeakSignalPeriodTx(QStringLiteral("band change"));
    if (ftActive) {
        stopFt8ShellImpl(false);
        stopFt8Sequencer(QString());
    }
    if (m_txRunning || m_txPreparationPending || m_cwSerialActive) stopImageTx();
    updateTxControlState();
}

void MainWindow::setManualDialFrequency(double hz, const QString &source)
{
    if (m_settings.hamlibCatEnabled) return;
    if (hz < 100000.0 || hz > 11000000000.0) {
        appendLog(QStringLiteral("Invalid frequency (%1).").arg(source));
        return;
    }
    if (m_txRunning || m_txPreparationPending) {
        // Like a QSY: the dial is not changed under a running transmission.
        appendLog(QStringLiteral("Frequency change refused: transmitting."));
        if (m_editManualFrequency != nullptr && m_manualDialHz > 0.0)
            m_editManualFrequency->setText(QString::number(m_manualDialHz / 1e6, 'f', 6));
        return;
    }
    m_manualDialHz = hz;
    QSettings(AppSettings::settingsFilePath(), QSettings::IniFormat).setValue(QStringLiteral("Bands/lastManualDialHz"), hz);
    if (m_editManualFrequency != nullptr) {
        const QSignalBlocker block(m_editManualFrequency);
        m_editManualFrequency->setText(QString::number(hz / 1e6, 'f', 6));
    }
    appendLog(uiText("manual_frequency_log", "Dial frequency entered (no CAT, %1): %2 — band %3.")
                  .arg(source, formatRigFrequency(hz), M3StandardFrequencies::bandOf(hz).isEmpty() ? QStringLiteral("--") : M3StandardFrequencies::bandOf(hz)));
    handleRigFrequencyChanged(hz);   // bands, QSO form and log follow as with CAT
    updateRigReadout();
}

bool MainWindow::qsyOrSetManual(double hz, const QString &reason)
{
    if (m_settings.hamlibCatEnabled) return tuneRigTo(hz, reason);
    setManualDialFrequency(hz, reason);
    return m_manualDialHz == hz;
}

double MainWindow::quickBandDialHz(const QString &modeName, const QString &band) const
{
    if (Ft8Mode::isFamilyMode(modeName)) return ftStandardFrequencyHz(modeName, band);
    if (Js8Mode::isMode(modeName)) return M3StandardFrequencies::dialHz(QStringLiteral("JS8"), band);
    const QString listMode = M3StandardFrequencies::listModeForName(modeName);
    if (listMode.isEmpty() || M3StandardFrequencies::isServiceMode(listMode)) return 0.0;
    return M3StandardFrequencies::dialHz(listMode, band);
}

void MainWindow::quickBandSelected(const QString &band)
{
    const QString mode = ui->cmbMode->currentText();
    const double hz = quickBandDialHz(mode, band);
    if (hz <= 0.0) return;
    if (m_txRunning || m_txPreparationPending) {
        appendLog(QStringLiteral("QSY to %1 refused: transmitting.").arg(band));
        updateQuickBandButtons();
        return;
    }
    // The CAT readback (or the manual frequency) moves the band selectors.
    qsyOrSetManual(hz, uiText("quick_band_reason", "band button %1").arg(band));
    updateQuickBandButtons();
}

void MainWindow::setupManualFrequencyAndQuickBands(QGridLayout *layout, QWidget *readout)
{
    // Row 3: manual dial frequency, only without CAT.
    m_manualFrequencyWidget = new QWidget(readout);
    auto *manualRow = new QHBoxLayout(m_manualFrequencyWidget);
    manualRow->setContentsMargins(0, 4, 0, 0);
    auto *manualLabel = new QLabel(uiText("manual_frequency_label", "Frequency (no CAT):"), m_manualFrequencyWidget);
    m_editManualFrequency = new QLineEdit(m_manualFrequencyWidget);
    m_editManualFrequency->setObjectName(QStringLiteral("manualDialFrequency"));
    m_editManualFrequency->setPlaceholderText(QStringLiteral("3.573"));
    m_editManualFrequency->setToolTip(uiText("manual_frequency_tip", "Without CAT m³ does not know the rig's dial frequency. Enter it here (MHz, kHz or Hz) and press Enter; the bands, the QSO form and the log follow it. TX is only possible with a confirmed frequency. After a restart it has to be confirmed again."));
    const double last = QSettings(AppSettings::settingsFilePath(), QSettings::IniFormat).value(QStringLiteral("Bands/lastManualDialHz")).toDouble();
    if (last > 0.0) m_editManualFrequency->setText(QString::number(last / 1e6, 'f', 6));   // suggestion, not confirmed
    auto *mhz = new QLabel(QStringLiteral("MHz"), m_manualFrequencyWidget);
    m_lblManualFrequencyState = new QLabel(m_manualFrequencyWidget);
    manualRow->addWidget(manualLabel);
    manualRow->addWidget(m_editManualFrequency, 1);
    manualRow->addWidget(mhz);
    manualRow->addWidget(m_lblManualFrequencyState);
    connect(m_editManualFrequency, &QLineEdit::returnPressed, this, [this]() {
        const double hz = M3StandardFrequencies::parseFrequency(m_editManualFrequency->text());
        if (hz <= 0.0) {
            appendLog(QStringLiteral("Invalid frequency: %1").arg(m_editManualFrequency->text()));
            return;
        }
        setManualDialFrequency(hz, uiText("manual_frequency_source", "entered"));
    });
    layout->addWidget(m_manualFrequencyWidget, 3, 0, 1, 4);

    // Row 4: quick band buttons (preferred bands).
    m_quickBandRow = new QWidget(readout);
    m_quickBandRow->setObjectName(QStringLiteral("quickBandRow"));
    auto *row = new QHBoxLayout(m_quickBandRow);
    row->setContentsMargins(0, 4, 0, 0);
    row->setSpacing(3);
    layout->addWidget(m_quickBandRow, 4, 0, 1, 4);
}

void MainWindow::rebuildQuickBandButtons()
{
    if (m_quickBandRow == nullptr) return;
    auto *row = qobject_cast<QHBoxLayout *>(m_quickBandRow->layout());
    if (row == nullptr) return;
    for (QToolButton *b : std::as_const(m_quickBandButtons)) {
        row->removeWidget(b);
        b->deleteLater();
    }
    m_quickBandButtons.clear();
    while (row->count() > 0) delete row->takeAt(0);
    const QStringList bands = M3BandPolicy::preferredInBandOrder(m_settings.preferredBands);
    for (const QString &band : bands) {
        auto *b = new QToolButton(m_quickBandRow);
        b->setObjectName(QStringLiteral("quickBand_") + band);
        b->setCheckable(true);
        b->setAutoRaise(false);
        b->setToolButtonStyle(Qt::ToolButtonTextOnly);
        connect(b, &QToolButton::clicked, this, [this, band]() { quickBandSelected(band); });
        row->addWidget(b);
        m_quickBandButtons.insert(band, b);
    }
    row->addStretch(1);
    m_quickBandRow->setVisible(!bands.isEmpty());
    updateQuickBandButtons();
}

void MainWindow::updateQuickBandButtons()
{
    if (m_manualFrequencyWidget != nullptr) {
        m_manualFrequencyWidget->setVisible(!m_settings.hamlibCatEnabled);
        if (m_lblManualFrequencyState != nullptr) {
            const bool confirmed = m_manualDialHz > 0.0;
            m_lblManualFrequencyState->setText(confirmed ? uiText("manual_frequency_confirmed", "set")
                                                         : uiText("manual_frequency_unconfirmed", "not set – no TX"));
            M3Ui::setSemanticRole(m_lblManualFrequencyState, confirmed ? QStringLiteral("positive") : QStringLiteral("negative"));
        }
    }
    if (m_quickBandButtons.isEmpty() || ui == nullptr || ui->cmbMode == nullptr) return;
    const QString mode = ui->cmbMode->currentText();
    const double dial = m_settings.hamlibCatEnabled ? m_lastRigFrequencyHz : m_manualDialHz;
    const QString current = M3StandardFrequencies::bandOf(dial);
    const bool busy = m_txRunning || m_txPreparationPending;
    for (auto it = m_quickBandButtons.cbegin(); it != m_quickBandButtons.cend(); ++it) {
        const QString &band = it.key();
        QToolButton *b = it.value();
        const double hz = quickBandDialHz(mode, band);
        const bool tx = M3BandPolicy::txAllowedOn(m_settings.bandsConfigured, m_settings.txBands, band);
        const bool hasFrequency = hz > 0.0;
        QFont f = b->font();
        f.setStrikeOut(!hasFrequency);
        f.setItalic(hasFrequency && !tx);
        b->setFont(f);
        b->setText(hasFrequency && !tx ? band + QStringLiteral(" RX") : band);
        // Three states that read without colour: TX band (plain), receive
        // only (italic, "RX", dimmed), no frequency for this mode (struck
        // through, dashed frame, disabled).
        QString style;
        if (!hasFrequency) style = QStringLiteral("QToolButton { border: 1px dashed palette(mid); padding: 2px 6px; }");
        else if (!tx) style = QStringLiteral("QToolButton { color: palette(mid); padding: 2px 6px; }");
        else style = QStringLiteral("QToolButton { padding: 2px 6px; }");
        // The band the rig is on: a strong frame and bold text.
        style += QStringLiteral(" QToolButton:checked { border: 2px solid palette(highlight); font-weight: bold; padding: 1px 5px; }");
        if (b->styleSheet() != style) b->setStyleSheet(style);
        b->setEnabled(hasFrequency && !busy);
        {
            const QSignalBlocker block(b);
            b->setChecked(band == current);
        }
        QString tip;
        if (!hasFrequency)
            tip = uiText("quick_band_no_frequency_tip", "No %1 frequency on %2 in the frequency list (Settings → Frequencies).").arg(mode, band);
        else
            tip = uiText("quick_band_tip", "QSY to %1 (%2).").arg(formatRigFrequency(hz), mode) +
                  QStringLiteral("\n") + (tx ? uiText("quick_band_tx_tip", "TX allowed on this band.")
                                             : uiText("quick_band_rx_tip", "Receive only: TX asks first, no automatic TX."));
        b->setToolTip(tip);
    }
}

namespace {
// Reorders @p combo: preferred bands first, then the rest, with a separator
// between; text, data and the current entry stay.  @p byData: the band name
// is the item data (mode band lists), else the text (FT, JS8).
void orderBandCombo(QComboBox *combo, const QStringList &preferred, bool byData)
{
    if (combo == nullptr) return;
    struct Item { QString text; QVariant data; QString band; };
    QVector<Item> items;
    for (int i = 0; i < combo->count(); ++i) {
        const QString text = combo->itemText(i);
        const QVariant data = combo->itemData(i);
        if (text.isEmpty() && !data.isValid()) continue;   // separator
        items.push_back({text, data, (byData ? data.toString() : text).toLower()});
    }
    QVector<Item> first, rest;
    for (const Item &it : items) (preferred.contains(it.band) ? first : rest).push_back(it);
    const QVector<Item> ordered = first + rest;
    const int separatorAt = (!first.isEmpty() && !rest.isEmpty()) ? first.size() : -1;
    // Unchanged order: leave the combo alone (no flicker, no signals).
    bool same = combo->count() == ordered.size() + (separatorAt >= 0 ? 1 : 0);
    for (int i = 0, j = 0; same && i < combo->count(); ++i) {
        if (i == separatorAt) { same = combo->itemText(i).isEmpty() && !combo->itemData(i).isValid(); continue; }
        same = combo->itemText(i) == ordered.at(j).text && combo->itemData(i) == ordered.at(j).data;
        ++j;
    }
    if (same) return;
    const QString currentText = combo->currentText();
    const QVariant currentData = combo->currentData();
    const QSignalBlocker block(combo);
    combo->clear();
    for (const Item &it : ordered) combo->addItem(it.text, it.data);
    if (separatorAt >= 0) combo->insertSeparator(separatorAt);
    int idx = byData ? combo->findData(currentData) : combo->findText(currentText);
    if (idx < 0) idx = separatorAt == 0 ? 1 : 0;
    combo->setCurrentIndex(idx);
}
}

void MainWindow::applyPreferredBandOrder()
{
    const QStringList &preferred = m_settings.preferredBands;
    orderBandCombo(m_cmbFt8Band, preferred, false);
    orderBandCombo(m_cmbJs8Band, preferred, false);
    for (auto it = m_modeBandWidgets.begin(); it != m_modeBandWidgets.end(); ++it)
        orderBandCombo(it->band, preferred, true);
}

// QSY plan: a WSPR entry the user marked "Send WSPR beacon" switches the
// beacon on once the rig is on the entry's band (after the CAT readback).
void MainWindow::schedulerWsprTxTryArm()
{
    if (m_schedulerWsprTxBand.isEmpty() || !m_settings.schedulerEnabled) return;
    if (ui == nullptr || ui->cmbMode == nullptr || !WsprMode::isMode(ui->cmbMode->currentText())) return;
    if (m_chkWsprTx == nullptr || m_chkWsprTx->isChecked()) return;
    if (currentTxBand() != m_schedulerWsprTxBand) return;   // rig not there yet
    if (!autoTxPermitted(true)) {
        if (!m_schedulerWsprTxRefusalLogged) {
            m_schedulerWsprTxRefusalLogged = true;
            const QString why = !m_ft8AutoModeEnabled ? uiText("wspr_tx_needs_auto", "Beacon TX is automatic operation: enable Auto mode in Settings first.")
                                                      : txBandRefusalReason();
            appendLog(uiText("scheduler_wspr_tx_refused_log", "QSY plan: WSPR beacon on %1 not started: %2").arg(m_schedulerWsprTxBand, why));
        }
        return;
    }
    appendLog(uiText("scheduler_wspr_tx_log", "QSY plan: WSPR beacon on %1 allowed (entry %2 UTC, marked for TX).")
                  .arg(m_schedulerWsprTxBand, m_schedulerWsprTxEntryTime));
    m_chkWsprTx->setChecked(true);
}

// The entry of today's plan that is active now (latest start <= now).  When it
// is a WSPR entry marked for TX, its beacon permission is restored, e.g. after
// a restart in the middle of the entry.
void MainWindow::schedulerRestoreActiveWsprTx(const QString &why)
{
    if (!m_settings.schedulerEnabled || !m_schedulerWsprTxBand.isEmpty()) return;
    const QDateTime now = QDateTime::currentDateTimeUtc();
    const QTime minute(now.time().hour(), now.time().minute());
    const ScheduledQsyEntry *active = nullptr;
    const QList<ScheduledQsyEntry> entries = schedulerEntries();
    for (const ScheduledQsyEntry &e : entries) {
        if (!e.enabled || !e.timeUtc.isValid() || !e.runsOn(now.date()) || e.timeUtc > minute) continue;
        if (active == nullptr || e.timeUtc >= active->timeUtc) active = &e;
    }
    if (active == nullptr || !active->wsprTx || !WsprMode::isMode(active->mode)) return;
    m_schedulerWsprTxBand = M3StandardFrequencies::bandOf(active->frequencyHz);
    m_schedulerWsprTxEntryTime = active->timeUtc.toString(QStringLiteral("HH:mm"));
    m_schedulerWsprTxRefusalLogged = false;
    appendLog(uiText("scheduler_wspr_tx_restored_log", "QSY plan (%1): the active entry %2 UTC allows the WSPR beacon on %3.")
                  .arg(why, m_schedulerWsprTxEntryTime, m_schedulerWsprTxBand));
    schedulerWsprTxTryArm();
}

QString MainWindow::wsprBeaconRefusalReason() const
{
    if (!m_ft8AutoModeEnabled)
        return uiText("wspr_tx_needs_auto", "Beacon TX is automatic operation: enable Auto mode in Settings first.");
    if (m_settings.schedulerEnabled && m_schedulerWsprTxBand.isEmpty())
        return uiText("wspr_tx_plan_only", "QSY plan on: the beacon only transmits in plan entries marked \"Send WSPR beacon\".");
    if (m_settings.schedulerEnabled && currentTxBand() != m_schedulerWsprTxBand)
        return uiText("wspr_tx_plan_band", "QSY plan: the beacon of this entry is for %1; the rig is not there.").arg(m_schedulerWsprTxBand);
    return txBandRefusalReason();
}

void MainWindow::schedulerWsprTxEnd(const QString &reason)
{
    if (m_schedulerWsprTxBand.isEmpty()) return;
    m_schedulerWsprTxBand.clear();
    m_schedulerWsprTxEntryTime.clear();
    m_schedulerWsprTxRefusalLogged = false;
    // The running 2-minute period may finish on the same band.
    if (m_chkWsprTx != nullptr && m_chkWsprTx->isChecked()) wsprStopBeacon(reason, true);
}
