// SPDX-License-Identifier: GPL-3.0-or-later
// WSPR pages of the main window: spot table, receiver settings and the live
// decoder thread.  Decoding lives in modems/wspr.
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "audio/AudioEngine.h"
#include "audio/BoundedAudioDispatcher.h"
#include "modems/wspr/WsprMode.h"
#include "runtime/SessionDiagnostics.h"
#include "utils/RuntimeI18n.h"
#include "widgets/QsoMapWidget.h"

#include <QGroupBox>
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
    outer->addStretch(1);
    ui->stkModeSettings->addWidget(m_pageWsprSettings);

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
