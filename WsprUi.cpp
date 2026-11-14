// SPDX-License-Identifier: GPL-3.0-or-later
// WSPR pages of the main window: spot table, receiver settings and the live
// decoder thread.  Decoding lives in modems/wspr.
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "audio/AudioEngine.h"
#include "audio/BoundedAudioDispatcher.h"
#include "modems/wspr/WsprMode.h"
#include "runtime/WeakSignalTxPreparer.h"
#include "third_party/wsjtx_gpl/wspr/WsprCodec.h"
#include "runtime/SessionDiagnostics.h"
#include "settings/AppSettings.h"
#include "utils/RuntimeI18n.h"
#include "widgets/QsoMapWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QGridLayout>
#include <QGroupBox>
#include <QRandomGenerator>
#include <QSettings>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLocale>
#include <QPushButton>
#include <QTableWidget>
#include <QThread>
#include <QVBoxLayout>

#include <cmath>

namespace {
enum WsprColumn { ColUtc, ColSnr, ColDt, ColFrequency, ColDrift, ColCall, ColGrid, ColPower, ColDistance, ColCount };
constexpr int kMaxWsprRows = 2000;
}

void MainWindow::setupWsprDisplayPage()
{
    m_wsprDisplayPage = new QWidget(m_mainDisplayStack);
    auto *layout = new QVBoxLayout(m_wsprDisplayPage);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->setSpacing(5);

    auto *caption = new QLabel(uiText("wspr_spots", "WSPR spots"), m_wsprDisplayPage);
    caption->setStyleSheet(QStringLiteral("font-weight: 600;"));
    layout->addWidget(caption);

    m_tableWspr = new QTableWidget(0, ColCount, m_wsprDisplayPage);
    m_tableWspr->setHorizontalHeaderLabels({QStringLiteral("UTC"), QStringLiteral("dB"), QStringLiteral("DT"),
                                            uiText("wspr_frequency", "MHz"), uiText("wspr_drift", "Drift"),
                                            uiText("wspr_call", "Call"), uiText("wspr_grid", "Grid"),
                                            QStringLiteral("dBm"), QStringLiteral("km")});
    m_tableWspr->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWspr->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableWspr->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWspr->verticalHeader()->setVisible(false);
    m_tableWspr->horizontalHeader()->setStretchLastSection(true);
    m_tableWspr->setSortingEnabled(false);
    const int widths[ColCount] = {52, 40, 42, 90, 48, 110, 80, 48, 60};
    for (int c = 0; c < ColCount; ++c) m_tableWspr->setColumnWidth(c, widths[c]);
    applyDecodeTableVisualSettings(m_tableWspr);
    layout->addWidget(m_tableWspr, 1);

    auto *row = new QHBoxLayout;
    auto *clear = new QPushButton(uiText("clear_messages", "Clear messages"), m_wsprDisplayPage);
    connect(clear, &QPushButton::clicked, this, &MainWindow::clearWsprTable);
    row->addWidget(clear);
    row->addStretch(1);
    layout->addLayout(row);
    m_mainDisplayStack->addWidget(m_wsprDisplayPage);
}

void MainWindow::setupWsprPage()
{
    if (ui == nullptr || ui->stkModeSettings == nullptr) return;
    m_pageWsprSettings = new QWidget(ui->stkModeSettings);
    auto *outer = new QVBoxLayout(m_pageWsprSettings);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(8);
    installModeFrequencyGroup(WsprMode::modeName());

    auto *group = new QGroupBox(uiText("wspr_rx", "WSPR receive"), m_pageWsprSettings);
    auto *grid = new QVBoxLayout(group);
    grid->setContentsMargins(8, 8, 8, 8);
    auto *info = new QLabel(uiText("wspr_rx_info", "WSPR-2: 2-minute periods from each even UTC minute. Each period is decoded after 114 s; RX must run from the start of a period. Receive only."),
                            group);
    info->setWordWrap(true);
    grid->addWidget(info);
    m_lblWsprStatus = new QLabel(group);
    m_lblWsprStatus->setWordWrap(true);
    m_lblWsprStatus->setText(uiText("wspr_waiting", "WSPR RX: waiting for the next even minute"));
    grid->addWidget(m_lblWsprStatus);
    auto *psk = new QLabel(uiText("wspr_psk_info", "Decoded stations are reported to PSK Reporter when it is enabled in Settings (callsign with a known country only)."),
                           group);
    psk->setWordWrap(true);
    grid->addWidget(psk);
    outer->addWidget(group);

    // Beacon TX: automatic operation, so only with Auto mode (Settings).
    QSettings stored(AppSettings::settingsFilePath(), QSettings::IniFormat);
    auto *txGroup = new QGroupBox(uiText("wspr_tx", "WSPR transmit"), m_pageWsprSettings);
    auto *txGrid = new QGridLayout(txGroup);
    txGrid->setContentsMargins(8, 8, 8, 8);
    int row = 0;
    m_chkWsprTx = new QCheckBox(uiText("wspr_tx_beacon", "Transmit as beacon"), txGroup);
    m_chkWsprTx->setToolTip(uiText("wspr_tx_beacon_tip", "Transmits your WSPR message in randomly chosen 2-minute periods (TX share). Automatic operation: only with Auto mode enabled in Settings; do not leave the station unattended. The TX button stops the beacon."));
    txGrid->addWidget(m_chkWsprTx, row++, 0, 1, 2);
    m_spinWsprTxPercent = new QSpinBox(txGroup);
    m_spinWsprTxPercent->setRange(0, 100);
    m_spinWsprTxPercent->setSingleStep(5);
    m_spinWsprTxPercent->setSuffix(QStringLiteral(" %"));
    m_spinWsprTxPercent->setValue(qBound(0, stored.value(QStringLiteral("WSPR/txPercent"), 20).toInt(), 100));
    m_spinWsprTxPercent->setToolTip(uiText("wspr_tx_percent_tip", "Share of 2-minute periods with TX (WSJT-X: Tx Pct). Up to 50 % never two periods in a row, so you also receive."));
    txGrid->addWidget(new QLabel(uiText("wspr_tx_percent", "TX share"), txGroup), row, 0);
    txGrid->addWidget(m_spinWsprTxPercent, row++, 1);
    m_spinWsprTxHz = new QSpinBox(txGroup);
    m_spinWsprTxHz->setRange(1400, 1600);
    m_spinWsprTxHz->setSuffix(QStringLiteral(" Hz"));
    m_spinWsprTxHz->setValue(qBound(1400, stored.value(QStringLiteral("WSPR/txHz"), 1500).toInt(), 1600));
    m_spinWsprTxHz->setToolTip(uiText("wspr_tx_hz_tip", "Audio frequency of your signal inside the 200 Hz WSPR band (1400-1600 Hz)."));
    txGrid->addWidget(new QLabel(uiText("wspr_tx_hz", "TX audio frequency"), txGroup), row, 0);
    txGrid->addWidget(m_spinWsprTxHz, row++, 1);
    m_cmbWsprPower = new QComboBox(txGroup);
    for (int dbm = 0; dbm <= 60; dbm += 10) {
        for (int step : {0, 3, 7}) {
            const int value = dbm + step;
            if (value > 60) break;
            const double watts = std::pow(10.0, (value - 30) / 10.0);
            const QString w = watts >= 1.0 ? QStringLiteral("%1 W").arg(watts, 0, 'g', 3)
                                           : QStringLiteral("%1 mW").arg(watts * 1000.0, 0, 'g', 3);
            m_cmbWsprPower->addItem(QStringLiteral("%1 dBm (%2)").arg(value).arg(w), value);
        }
    }
    const int storedPower = M3Wspr::validPowerDbm(stored.value(QStringLiteral("WSPR/powerDbm"), 37).toInt());
    m_cmbWsprPower->setCurrentIndex(qMax(0, m_cmbWsprPower->findData(storedPower)));
    m_cmbWsprPower->setToolTip(uiText("wspr_power_tip", "Transmit power reported in the WSPR message; set it to the power at the antenna."));
    txGrid->addWidget(new QLabel(uiText("wspr_power", "Power"), txGroup), row, 0);
    txGrid->addWidget(m_cmbWsprPower, row++, 1);
    m_lblWsprTxStatus = new QLabel(txGroup);
    m_lblWsprTxStatus->setWordWrap(true);
    txGrid->addWidget(m_lblWsprTxStatus, row++, 0, 1, 2);
    txGrid->setColumnStretch(1, 1);
    outer->addWidget(txGroup);
    outer->addStretch(1);
    ui->stkModeSettings->addWidget(m_pageWsprSettings);

    const auto persist = [this]() {
        QSettings settings(AppSettings::settingsFilePath(), QSettings::IniFormat);
        settings.setValue(QStringLiteral("WSPR/txPercent"), m_spinWsprTxPercent->value());
        settings.setValue(QStringLiteral("WSPR/txHz"), m_spinWsprTxHz->value());
        settings.setValue(QStringLiteral("WSPR/powerDbm"), m_cmbWsprPower->currentData().toInt());
        updateWsprTxAvailability();
    };
    connect(m_spinWsprTxPercent, QOverload<int>::of(&QSpinBox::valueChanged), this, persist);
    connect(m_spinWsprTxHz, QOverload<int>::of(&QSpinBox::valueChanged), this, persist);
    connect(m_cmbWsprPower, QOverload<int>::of(&QComboBox::currentIndexChanged), this, persist);
    connect(m_chkWsprTx, &QCheckBox::toggled, this, [this](bool on) {
        if (!on) {
            wsprStopBeacon(QStringLiteral("beacon switched off"));
            return;
        }
        QString error;
        if (!m_ft8AutoModeEnabled || wsprTxMessages(&error).isEmpty()) {
            const QSignalBlocker block(m_chkWsprTx);
            m_chkWsprTx->setChecked(false);
            updateWsprTxAvailability();
            return;
        }
        appendLog(QStringLiteral("WSPR beacon on: %1 %, %2 Hz, %3")
                      .arg(m_spinWsprTxPercent->value()).arg(m_spinWsprTxHz->value())
                      .arg(wsprTxMessages().join(QStringLiteral(" / "))));
        wsprPlanNextPeriod();
    });
    m_wsprTxTimer.setSingleShot(true);
    connect(&m_wsprTxTimer, &QTimer::timeout, this, &MainWindow::wsprPlanNextPeriod);
    updateWsprTxAvailability();

    if (m_wsprDecoder != nullptr || m_audioEngine == nullptr) return;
    m_wsprDecoder = new WsprDecoder();
    connect(m_wsprDecoder, &WsprDecoder::statusChanged, m_wsprDecoder, [](const QString &text) {
        SessionDiagnostics::instance().record("rx.status", {{"mode", "WSPR"}, {"message", text}});
    }, Qt::DirectConnection);
    m_wsprThread = new QThread(this);
    m_wsprDecoder->moveToThread(m_wsprThread);
    auto *queue = new BoundedAudioDispatcher(256);
    queue->moveToThread(m_wsprThread);
    connect(m_wsprThread, &QThread::finished, queue, &QObject::deleteLater);
    connect(m_audioEngine, &AudioEngine::audioBlockReady, queue,
            [queue, decoder = m_wsprDecoder](const AudioBlock &block) {
                if (decoder->liveInputEnabled()) queue->enqueue(block);
            }, Qt::DirectConnection);
    connect(queue, &BoundedAudioDispatcher::blocksAvailable, m_wsprDecoder,
            [queue, decoder = m_wsprDecoder]() {
                int dropped = 0;
                const auto blocks = queue->takePending(8, &dropped);
                if (!decoder->liveInputEnabled()) return;
                if (dropped) {
                    decoder->reset();
                    emit decoder->statusChanged(QStringLiteral("WSPR audio queue overflow: %1 block(s) lost; period reset.").arg(dropped));
                }
                for (const auto &block : blocks) decoder->processAudioBlock(block);
            }, Qt::QueuedConnection);
    connect(m_wsprThread, &QThread::finished, m_wsprDecoder, &QObject::deleteLater);
    connect(m_wsprDecoder, &WsprDecoder::decoded, this, &MainWindow::handleWsprDecoded, Qt::QueuedConnection);
    connect(m_wsprDecoder, &WsprDecoder::periodStateChanged, this,
            [this](WsprDecoder::PeriodState state, const QDateTime &periodUtc) {
        if (m_lblWsprStatus == nullptr) return;
        const QString hhmm = periodUtc.toString(QStringLiteral("HH:mm"));
        switch (state) {
        case WsprDecoder::PeriodState::Waiting:
            m_lblWsprStatus->setText(uiText("wspr_waiting", "WSPR RX: waiting for the next even minute"));
            break;
        case WsprDecoder::PeriodState::Collecting:
            m_lblWsprStatus->setText(uiText("wspr_collecting", "WSPR RX: receiving period %1 UTC").arg(hhmm));
            break;
        case WsprDecoder::PeriodState::LateStart:
            m_lblWsprStatus->setText(uiText("wspr_late_start", "WSPR RX: period %1 UTC started before RX; waiting for the next even minute").arg(hhmm));
            break;
        case WsprDecoder::PeriodState::Decoding:
            m_lblWsprStatus->setText(uiText("wspr_decoding", "WSPR RX: decoding period %1 UTC").arg(hhmm));
            break;
        case WsprDecoder::PeriodState::BacklogFull:
            m_lblWsprStatus->setText(uiText("wspr_backlog", "WSPR RX: period %1 UTC not decoded (decoder busy)").arg(hhmm));
            break;
        }
    }, Qt::QueuedConnection);
    connect(m_wsprDecoder, &WsprDecoder::statusChanged, this, [this](const QString &text) {
        if (text.contains(QStringLiteral("overflow")) || text.contains(QStringLiteral("failed")) ||
            text.contains(QStringLiteral("backlog")))
            appendLog(text);
    }, Qt::QueuedConnection);
    m_wsprThread->start();
}

void MainWindow::clearWsprTable()
{
    if (m_tableWspr != nullptr) m_tableWspr->setRowCount(0);
}

void MainWindow::handleWsprDecoded(const QVector<WsprSpot> &spots, const QDateTime &periodUtc, int milliseconds)
{
    appendLog(QStringLiteral("WSPR %1 UTC: %2 decode(s) in %3 s")
                  .arg(periodUtc.toString(QStringLiteral("HH:mm")))
                  .arg(spots.size())
                  .arg(milliseconds / 1000.0, 0, 'f', 1));
    if (m_lblWsprStatus != nullptr)
        m_lblWsprStatus->setText(uiText("wspr_decoded", "WSPR RX: %1 decode(s) in period %2 UTC (%3 s)")
                                     .arg(spots.size())
                                     .arg(periodUtc.toString(QStringLiteral("HH:mm")))
                                     .arg(QLocale().toString(milliseconds / 1000.0, 'f', 1)));
    if (m_tableWspr == nullptr) return;
    QPointF home;
    const bool haveHome = QsoMapWidget::maidenheadToLonLat(stationLocator().trimmed().toUpper(), &home);
    const double dialHz = m_lastRxDialHz;
    for (const WsprSpot &spot : spots) {
        const QString call = spot.type == 3 ? (spot.callsign.isEmpty() ? QStringLiteral("<...>")
                                                                      : QStringLiteral("<%1>").arg(spot.callsign))
                                            : spot.callsign;
        QString distance;
        QPointF dx;
        if (haveHome && !spot.grid.isEmpty() && QsoMapWidget::maidenheadToLonLat(spot.grid, &dx))
            distance = QString::number(QsoMapWidget::distanceKm(home, dx), 'f', 0);
        const QString frequency = dialHz > 0.0 ? QString::number((dialHz + spot.audioHz) / 1e6, 'f', 6)
                                               : QStringLiteral("%1 Hz").arg(QString::number(spot.audioHz, 'f', 0));
        const QStringList cells = {periodUtc.toString(QStringLiteral("HHmm")),
                                   QString::number(spot.snrDb),
                                   QString::number(spot.dtSeconds, 'f', 1),
                                   frequency,
                                   QString::number(spot.drift),
                                   call,
                                   spot.grid,
                                   QString::number(spot.powerDbm),
                                   distance};
        const int row = m_tableWspr->rowCount();
        m_tableWspr->insertRow(row);
        for (int c = 0; c < ColCount; ++c) {
            auto *item = new QTableWidgetItem(cells.value(c));
            if (c != ColCall && c != ColGrid) item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            m_tableWspr->setItem(row, c, item);
        }
        SessionDiagnostics::instance().record("wspr.spot", {{"period_utc", periodUtc.toString(Qt::ISODate)},
                                                            {"snr_db", spot.snrDb},
                                                            {"dt", spot.dtSeconds},
                                                            {"audio_hz", spot.audioHz},
                                                            {"drift", spot.drift},
                                                            {"message", spot.message}});
        // PSK Reporter: known callsign only; type 2 has no locator.
        if (!spot.callsign.isEmpty())
            reportPskSpot(spot.callsign, spot.grid.left(4), spot.audioHz, WsprMode::modeName(), spot.snrDb, true, false);
    }
    while (m_tableWspr->rowCount() > kMaxWsprRows) m_tableWspr->removeRow(0);
    if (!spots.isEmpty()) m_tableWspr->scrollToBottom();
}

// The WSPR message(s) of this station: type 1 for a plain call; a compound
// call is sent as type 2, alternating with type 3 (hash + 6-character
// locator) when the locator has 6 characters, as WSJT-X does.
QStringList MainWindow::wsprTxMessages(QString *error) const
{
    const QString call = stationCallsign().trimmed().toUpper();
    const QString grid = stationLocator().trimmed().toUpper();
    const int power = m_cmbWsprPower != nullptr ? m_cmbWsprPower->currentData().toInt() : 37;
    QStringList messages;
    if (call.contains(QLatin1Char('/'))) {
        messages << QStringLiteral("%1 %2").arg(call).arg(power);
        if (grid.size() >= 6) messages << QStringLiteral("<%1> %2 %3").arg(call, grid.left(6)).arg(power);
    } else {
        messages << QStringLiteral("%1 %2 %3").arg(call, grid.left(4)).arg(power);
    }
    for (const QString &message : messages) {
        unsigned char data[M3Wspr::kDataBytes];
        std::string why;
        if (call.isEmpty() || grid.size() < 4 || !M3Wspr::pack(message.toStdString(), data, &why)) {
            if (error != nullptr)
                *error = call.isEmpty() || grid.size() < 4
                    ? uiText("wspr_tx_need_station", "Enter your callsign and locator in Settings first.")
                    : uiText("wspr_tx_cannot_pack", "\"%1\" cannot be sent as WSPR: %2").arg(message, QString::fromStdString(why));
            return {};
        }
    }
    return messages;
}

void MainWindow::updateWsprTxAvailability()
{
    if (m_chkWsprTx == nullptr) return;
    QString error;
    const QStringList messages = wsprTxMessages(&error);
    const bool possible = m_ft8AutoModeEnabled && !messages.isEmpty();
    if (!possible && m_chkWsprTx->isChecked()) m_chkWsprTx->setChecked(false);   // stops the beacon
    m_chkWsprTx->setEnabled(possible);
    if (m_lblWsprTxStatus == nullptr) return;
    if (!m_ft8AutoModeEnabled) {
        m_lblWsprTxStatus->setText(uiText("wspr_tx_needs_auto", "Beacon TX is automatic operation: enable Auto mode in Settings first."));
    } else if (messages.isEmpty()) {
        m_lblWsprTxStatus->setText(error);
    } else if (!m_chkWsprTx->isChecked()) {
        m_lblWsprTxStatus->setText(uiText("wspr_tx_message", "Message: %1").arg(messages.join(QStringLiteral(" / "))));
    }
}

void MainWindow::wsprStopBeacon(const QString &reason)
{
    const bool wasOn = m_chkWsprTx != nullptr && m_chkWsprTx->isChecked();
    m_wsprTxTimer.stop();
    if (m_chkWsprTx != nullptr && m_chkWsprTx->isChecked()) {
        const QSignalBlocker block(m_chkWsprTx);
        m_chkWsprTx->setChecked(false);
    }
    if (m_nativeWeakSignalTxPending && WsprMode::isMode(m_nativeWeakSignalTxMode))
        cancelNativeWeakSignalPeriodTx(reason);
    const bool transmitting = m_txPreparationPending || m_txRunning ||
                              (m_txAudioEngine != nullptr && m_txAudioEngine->isRunning());
    if (transmitting && ui != nullptr && ui->cmbMode != nullptr && WsprMode::isMode(ui->cmbMode->currentText()))
        stopImageTx();
    if (wasOn || transmitting) appendLog(QStringLiteral("WSPR beacon off (%1).").arg(reason));
    updateWsprTxAvailability();
    updateTxControlState();
}

// Called in the middle of every period while the beacon is on: decides
// whether the next period transmits (TX share, never two periods in a row
// up to 50 %) and arms it.
void MainWindow::wsprPlanNextPeriod()
{
    m_wsprTxTimer.stop();
    if (m_chkWsprTx == nullptr || !m_chkWsprTx->isChecked()) return;
    if (ui == nullptr || ui->cmbMode == nullptr || !WsprMode::isMode(ui->cmbMode->currentText()) ||
        !m_ft8AutoModeEnabled) {
        wsprStopBeacon(QStringLiteral("mode or Auto mode changed"));
        return;
    }
    constexpr qint64 periodMs = 120000;
    const qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
    const qint64 currentStart = nowMs / periodMs * periodMs;
    const qint64 nextStart = currentStart + periodMs;
    // Next decision in the middle of the coming period.
    m_wsprTxTimer.start(static_cast<int>(nextStart + periodMs / 2 - nowMs));

    const int percent = m_spinWsprTxPercent != nullptr ? m_spinWsprTxPercent->value() : 0;
    const bool afterTx = m_wsprLastTxPeriodMs == currentStart;
    const bool transmit = percent > 0 && !(afterTx && percent <= 50) &&
                          static_cast<int>(QRandomGenerator::global()->bounded(100)) < percent;
    // At least 5 s to prepare the waveform and key the rig.
    if (!transmit || nextStart - nowMs < 5000 || m_nativeWeakSignalTxPending) {
        if (m_lblWsprTxStatus != nullptr)
            m_lblWsprTxStatus->setText(uiText("wspr_tx_next_rx", "Beacon on: %1 UTC receive, next decision at %2 UTC")
                                           .arg(QDateTime::fromMSecsSinceEpoch(nextStart, Qt::UTC).toString(QStringLiteral("HH:mm")),
                                                QDateTime::fromMSecsSinceEpoch(nextStart + periodMs / 2, Qt::UTC).toString(QStringLiteral("HH:mm:ss"))));
        return;
    }
    wsprArmPeriod(nextStart);
}

void MainWindow::wsprArmPeriod(qint64 periodStartMs)
{
    QString error;
    const QStringList messages = wsprTxMessages(&error);
    if (messages.isEmpty() || m_txWaveformPreparer == nullptr) {
        wsprStopBeacon(error.isEmpty() ? QStringLiteral("no message") : error);
        return;
    }
    const QString message = messages.value(m_wsprMessageIndex % messages.size());
    ++m_wsprMessageIndex;
    const double hz = m_spinWsprTxHz != nullptr ? m_spinWsprTxHz->value() : 1500;
    const int sampleRate = (m_settings.audioSampleRate == 44100 || m_settings.audioSampleRate == 48000 ||
                            m_settings.audioSampleRate == 96000) ? m_settings.audioSampleRate : 48000;
    const QString modeName = WsprMode::modeName();
    m_nativeWeakSignalTxMode = modeName;
    m_nativeWeakSignalTxBoundaryUtcMs = periodStartMs;
    m_nativeWeakSignalTxPending = true;
    m_wsprLastTxPeriodMs = periodStartMs;
    const auto generation = ++m_nativeTxPreparationGeneration;
    m_txWaveformPreparer->prepareWspr(message, sampleRate, hz,
        [this, generation, modeName](std::unique_ptr<TxModulator> waveform, const QString &why) {
            if (m_shutdownInProgress || generation != m_nativeTxPreparationGeneration ||
                !m_nativeWeakSignalTxPending || modeName != m_nativeWeakSignalTxMode) return;
            if (!waveform) {
                appendLog(QStringLiteral("WSPR waveform preparation failed: ") + why);
                wsprStopBeacon(why);
                return;
            }
            m_nativeWeakSignalPreparedModulator = std::move(waveform);
        });
    const qint64 delayMs = qMax<qint64>(qint64{1}, periodStartMs - QDateTime::currentMSecsSinceEpoch());
    m_nativeWeakSignalTxTimer.start(static_cast<int>(delayMs));
    const QString at = QDateTime::fromMSecsSinceEpoch(periodStartMs, Qt::UTC).toString(QStringLiteral("HH:mm"));
    appendLog(QStringLiteral("WSPR TX armed: %1 UTC, %2 Hz, %3").arg(at).arg(hz, 0, 'f', 0).arg(message));
    SessionDiagnostics::instance().record("wspr.tx.armed", {{"period_utc_ms", periodStartMs}, {"audio_hz", hz}, {"message", message}});
    if (m_lblWsprTxStatus != nullptr)
        m_lblWsprTxStatus->setText(uiText("wspr_tx_armed", "Beacon on: TX at %1 UTC (%2)").arg(at, message));
    updateTxControlState();
}
