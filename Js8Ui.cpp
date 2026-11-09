// SPDX-License-Identifier: GPL-3.0-or-later
// JS8 pages of the main window: band activity, heard stations, conversation,
// message entry and the RX/TX settings.  Protocol work lives in modems/js8.
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "audio/AudioEngine.h"
#include "audio/BoundedAudioDispatcher.h"
#include "runtime/SessionDiagnostics.h"
#include "settings/AppSettings.h"
#include "utils/CockpitTheme.h"
#include "utils/RuntimeI18n.h"
#include "utils/StandardFrequencies.h"
#include "widgets/WaterfallWidget.h"

#include <QApplication>
#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileInfo>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QSettings>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QSplitter>
#include <QTableWidget>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>

#include <algorithm>
#include <cmath>

namespace {
constexpr int kJs8MaxActivityRows = 80;
constexpr int kJs8MaxHeard = 200;
constexpr int kJs8MaxTextChars = 400;
constexpr int kJs8MaxQueue = 6;
constexpr qint64 kJs8AllCallReplyMs = 15 * 60 * 1000;   // one @ALLCALL/HB answer per station
constexpr qint64 kJs8HeardAgingMs = 60 * 60 * 1000;     // HEARING?/QUERY CALL look back one hour

QSettings js8Settings() { return QSettings(AppSettings::settingsFilePath(), QSettings::IniFormat); }

QString js8InboxPath()
{
    return QFileInfo(AppSettings::settingsFilePath()).absolutePath() + QStringLiteral("/js8_inbox.json");
}

void fillIntervalCombo(QComboBox *combo, const QList<int> &minutes, const QString &offText)
{
    for (int m : minutes)
        combo->addItem(m == 0 ? offText : QStringLiteral("%1 min").arg(m), m);
}

QTableWidget *makeTable(QWidget *parent, const QStringList &headers)
{
    auto *t = new QTableWidget(0, headers.size(), parent);
    t->setHorizontalHeaderLabels(headers);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->verticalHeader()->setVisible(false);
    t->horizontalHeader()->setStretchLastSection(true);
    return t;
}
} // namespace

Js8Mode::Submode MainWindow::currentJs8Submode() const
{
    return Js8Mode::submodeFromInt(m_cmbJs8Submode != nullptr ? m_cmbJs8Submode->currentData().toInt() : 0);
}

void MainWindow::setupJs8DisplayPage()
{
    m_js8DisplayPage = new QWidget(m_mainDisplayStack);
    auto *layout = new QVBoxLayout(m_js8DisplayPage);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->setSpacing(5);

    auto *vertical = new QSplitter(Qt::Vertical, m_js8DisplayPage);
    auto *horizontal = new QSplitter(Qt::Horizontal, vertical);

    auto *bandBox = new QWidget(horizontal);
    auto *bandLayout = new QVBoxLayout(bandBox);
    bandLayout->setContentsMargins(0, 0, 0, 0);
    auto *bandCaption = new QLabel(uiText("js8_band_activity", "Band activity"), bandBox);
    bandCaption->setStyleSheet(QStringLiteral("font-weight: 600;"));
    m_tableJs8Band = makeTable(bandBox, {QStringLiteral("Offset"), QStringLiteral("UTC"), QStringLiteral("dB"),
                                         uiText("js8_speed_column", "Sp."), uiText("js8_text_column", "Text")});
    m_tableJs8Band->setColumnWidth(0, 56);
    m_tableJs8Band->setColumnWidth(1, 70);
    m_tableJs8Band->setColumnWidth(2, 40);
    m_tableJs8Band->setColumnWidth(3, 30);
    m_tableJs8Band->horizontalHeaderItem(3)->setToolTip(uiText("js8_speed_column_tooltip", "Speed: A Normal, B Fast, C Turbo, E Slow"));
    m_tableJs8Band->setWordWrap(false);
    applyDecodeTableVisualSettings(m_tableJs8Band);
    bandLayout->addWidget(bandCaption);
    bandLayout->addWidget(m_tableJs8Band, 1);

    auto *callBox = new QWidget(horizontal);
    auto *callLayout = new QVBoxLayout(callBox);
    callLayout->setContentsMargins(0, 0, 0, 0);
    auto *callCaption = new QLabel(uiText("js8_call_activity", "Heard stations"), callBox);
    callCaption->setStyleSheet(QStringLiteral("font-weight: 600;"));
    m_tableJs8Calls = makeTable(callBox, {uiText("js8_call_column", "Call"), QStringLiteral("dB"),
                                          uiText("js8_grid_column", "Grid"), QStringLiteral("UTC"), QStringLiteral("Offset")});
    m_tableJs8Calls->setColumnWidth(0, 86);
    m_tableJs8Calls->setColumnWidth(1, 40);
    m_tableJs8Calls->setColumnWidth(2, 64);
    m_tableJs8Calls->setColumnWidth(3, 70);
    applyDecodeTableVisualSettings(m_tableJs8Calls);
    callLayout->addWidget(callCaption);
    callLayout->addWidget(m_tableJs8Calls, 1);
    horizontal->addWidget(bandBox);
    horizontal->addWidget(callBox);
    horizontal->setStretchFactor(0, 3);
    horizontal->setStretchFactor(1, 2);

    auto *rxBox = new QWidget(vertical);
    auto *rxLayout = new QVBoxLayout(rxBox);
    rxLayout->setContentsMargins(0, 0, 0, 0);
    auto *rxCaption = new QLabel(uiText("js8_conversation", "Messages (RX offset, to me, @ALLCALL)"), rxBox);
    rxCaption->setStyleSheet(QStringLiteral("font-weight: 600;"));
    m_txtJs8Rx = new QPlainTextEdit(rxBox);
    m_txtJs8Rx->setReadOnly(true);
    m_txtJs8Rx->setMaximumBlockCount(2000);
    rxLayout->addWidget(rxCaption);
    rxLayout->addWidget(m_txtJs8Rx, 1);

    vertical->addWidget(horizontal);
    vertical->addWidget(rxBox);
    vertical->setStretchFactor(0, 3);
    vertical->setStretchFactor(1, 2);

    auto *txRow = new QHBoxLayout();
    txRow->setSpacing(4);
    auto *toLabel = new QLabel(uiText("js8_to", "To:"), m_js8DisplayPage);
    m_editJs8To = new QLineEdit(m_js8DisplayPage);
    m_editJs8To->setPlaceholderText(uiText("js8_to_placeholder", "call or @GROUP"));
    m_editJs8To->setMaximumWidth(130);
    m_editJs8To->setToolTip(uiText("js8_to_tooltip", "Selected station. Your text is sent directed to it unless the text already starts with a call, @group, CQ or HEARTBEAT."));
    m_btnJs8ClearTo = new QPushButton(QStringLiteral("\u00d7"), m_js8DisplayPage);
    m_btnJs8ClearTo->setMaximumWidth(26);
    m_btnJs8ClearTo->setToolTip(uiText("js8_clear_to", "Deselect the station"));
    m_editJs8Tx = new QLineEdit(m_js8DisplayPage);
    m_editJs8Tx->setPlaceholderText(uiText("js8_tx_placeholder", "Message, e.g. HW CPY? or free text"));
    m_btnJs8Send = new QPushButton(uiText("js8_send", "Send"), m_js8DisplayPage);
    m_btnJs8Send->setToolTip(uiText("js8_send_tooltip", "Transmit the message from the next period on (one frame per period)."));
    m_btnJs8Cq = new QPushButton(QStringLiteral("CQ"), m_js8DisplayPage);
    m_btnJs8Hb = new QPushButton(QStringLiteral("HB"), m_js8DisplayPage);
    m_btnJs8Hb->setToolTip(uiText("js8_hb_tooltip", "Send a heartbeat with your grid."));
    m_btnJs8Snr = new QPushButton(QStringLiteral("SNR?"), m_js8DisplayPage);
    m_btnJs8Snr->setToolTip(uiText("js8_snr_tooltip", "Ask the selected station for a signal report."));
    m_btnJs8Inbox = new QPushButton(uiText("js8_inbox", "Inbox"), m_js8DisplayPage);
    m_btnJs8Inbox->setToolTip(uiText("js8_inbox_tooltip", "Messages received with MSG and messages kept for other stations (MSG TO:)."));
    txRow->addWidget(toLabel);
    txRow->addWidget(m_editJs8To);
    txRow->addWidget(m_btnJs8ClearTo);
    txRow->addWidget(m_editJs8Tx, 1);
    txRow->addWidget(m_btnJs8Send);
    txRow->addWidget(m_btnJs8Cq);
    txRow->addWidget(m_btnJs8Hb);
    txRow->addWidget(m_btnJs8Snr);
    txRow->addWidget(m_btnJs8Inbox);

    m_lblJs8TxStatus = new QLabel(m_js8DisplayPage);
    m_lblJs8TxStatus->setWordWrap(true);

    layout->addWidget(vertical, 1);
    layout->addLayout(txRow);
    layout->addWidget(m_lblJs8TxStatus);

    // Header and button texts are already localised or are protocol words.
    for (QObject *fixed : std::initializer_list<QObject *>{m_tableJs8Band, m_tableJs8Calls, m_btnJs8ClearTo, m_btnJs8Cq,
                                                           m_btnJs8Hb, m_btnJs8Snr})
        fixed->setProperty("i18nSkipSelf", true);

    connect(m_btnJs8Inbox, &QPushButton::clicked, this, &MainWindow::showJs8Inbox);
    connect(m_btnJs8Send, &QPushButton::clicked, this, &MainWindow::js8SendFromEditor);
    connect(m_editJs8Tx, &QLineEdit::returnPressed, this, &MainWindow::js8SendFromEditor);
    connect(m_btnJs8ClearTo, &QPushButton::clicked, this, [this]() { m_editJs8To->clear(); js8OperatorActivity(); });
    connect(m_editJs8Tx, &QLineEdit::textEdited, this, [this]() { js8OperatorActivity(); });
    connect(m_btnJs8Cq, &QPushButton::clicked, this, [this]() {
        m_editJs8To->clear();
        js8Send(Js8Message::cqText(stationLocator()), QString());
    });
    connect(m_btnJs8Hb, &QPushButton::clicked, this, [this]() {
        js8Send(stationCallsign() + QStringLiteral(": ") + Js8Message::heartbeatText(stationLocator()), QString());
    });
    connect(m_btnJs8Snr, &QPushButton::clicked, this, [this]() {
        const QString to = m_editJs8To->text().trimmed().toUpper();
        if (to.isEmpty()) {
            m_lblJs8TxStatus->setText(uiText("js8_select_station", "Select a station first (click it in the heard list)."));
            return;
        }
        js8Send(QStringLiteral("SNR?"), to);
    });
    auto selectCall = [this](const QString &call) {
        if (call.isEmpty()) return;
        js8OperatorActivity();
        m_editJs8To->setText(call);
        if (m_js8QsoForm != nullptr) {
            if (m_js8QsoForm->callsign != nullptr) m_js8QsoForm->callsign->setText(call);
            const auto heard = m_js8Heard.constFind(call);
            if (heard != m_js8Heard.constEnd()) {
                if (m_js8QsoForm->grid != nullptr && !heard->grid.isEmpty()) m_js8QsoForm->grid->setText(heard->grid);
                if (m_js8QsoForm->rstSent != nullptr) m_js8QsoForm->rstSent->setText(QString::asprintf("%+03d", heard->snrDb));
            }
        }
    };
    connect(m_tableJs8Calls, &QTableWidget::cellClicked, this, [this, selectCall](int row, int) {
        auto *item = m_tableJs8Calls->item(row, 0);
        if (item != nullptr) selectCall(item->text());
    });
    connect(m_tableJs8Calls, &QTableWidget::cellDoubleClicked, this, [this](int row, int) {
        auto *item = m_tableJs8Calls->item(row, 4);
        if (item != nullptr) js8SetOffset(item->text().toInt(), true, true);
    });
    connect(m_tableJs8Band, &QTableWidget::cellClicked, this, [this, selectCall](int row, int) {
        auto *from = m_tableJs8Band->item(row, 4);
        const QString call = from != nullptr ? from->data(Qt::UserRole).toString() : QString();
        if (!call.isEmpty() && call != stationCallsign()) selectCall(call);
    });
    connect(m_tableJs8Band, &QTableWidget::cellDoubleClicked, this, [this](int row, int) {
        auto *item = m_tableJs8Band->item(row, 0);
        if (item == nullptr) return;
        // A double-click on another speed's signal also selects that speed.
        auto *speed = m_tableJs8Band->item(row, 3);
        if (speed != nullptr && m_cmbJs8Submode != nullptr) {
            const int idx = m_cmbJs8Submode->findData(speed->data(Qt::UserRole).toInt());
            if (idx >= 0 && idx != m_cmbJs8Submode->currentIndex()) m_cmbJs8Submode->setCurrentIndex(idx);
        }
        js8SetOffset(item->text().toInt(), true, true);
    });

    m_js8DisplayPage = wrapTextDisplayPageWithMap(m_js8DisplayPage, QStringLiteral("JS8"), QStringLiteral("JS8"));
    m_mainDisplayStack->addWidget(m_js8DisplayPage);
}

void MainWindow::setupJs8Page()
{
    if (ui == nullptr || ui->stkModeSettings == nullptr) return;
    QSettings settings = js8Settings();

    m_pageJs8Settings = new QWidget(ui->stkModeSettings);
    auto *outer = new QVBoxLayout(m_pageJs8Settings);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(8);

    auto *group = new QGroupBox(uiText("js8_rx_tx_settings", "JS8 RX/TX settings"), m_pageJs8Settings);
    auto *grid = new QGridLayout(group);
    grid->setContentsMargins(8, 8, 8, 8);
    int row = 0;

    // Dial frequency: CAT readout (moved here by updateRigReadout), band and standard frequency.
    m_js8FrequencySlot = new QWidget(group);
    auto *slotLayout = new QVBoxLayout(m_js8FrequencySlot);
    slotLayout->setContentsMargins(0, 0, 0, 0);
    slotLayout->setSpacing(2);
    m_lblJs8BandStatus = new QLabel(m_js8FrequencySlot);
    m_lblJs8BandStatus->setWordWrap(true);
    slotLayout->addWidget(m_lblJs8BandStatus);
    grid->addWidget(new QLabel(uiText("frequency", "Frequency"), group), row, 0, Qt::AlignLeft | Qt::AlignTop);
    grid->addWidget(m_js8FrequencySlot, row++, 1, 1, 3);

    m_cmbJs8Band = new QComboBox(group);
    m_cmbJs8Band->addItems(M3StandardFrequencies::bands(QStringLiteral("JS8")));
    m_cmbJs8Band->setCurrentIndex(qMax(0, m_cmbJs8Band->findText(settings.value(QStringLiteral("JS8/band"), QStringLiteral("20m")).toString())));
    m_cmbJs8Band->setToolTip(uiText("js8_band_tooltip", "Select a band to QSY the CAT-connected rig to the JS8 standard frequency (JS8Call list)."));
    grid->addWidget(new QLabel(uiText("band", "Band"), group), row, 0);
    grid->addWidget(m_cmbJs8Band, row++, 1, 1, 3);

    m_cmbJs8Submode = new QComboBox(group);
    for (Js8Mode::Submode s : Js8Mode::allSubmodes()) {
        const Js8Mode::Parameters p = Js8Mode::parameters(s);
        m_cmbJs8Submode->addItem(QStringLiteral("%1 (%2 s, %3 Hz)").arg(Js8Mode::submodeName(s)).arg(p.periodSeconds)
                                     .arg(qRound(p.bandwidthHz())), int(s));
    }
    m_cmbJs8Submode->setCurrentIndex(qMax(0, m_cmbJs8Submode->findData(settings.value(QStringLiteral("JS8/submode"), 0).toInt())));
    grid->addWidget(new QLabel(uiText("js8_speed", "Speed"), group), row, 0);
    grid->addWidget(m_cmbJs8Submode, row++, 1, 1, 3);

    m_chkJs8MultiDecode = new QCheckBox(uiText("js8_multi_decode", "Decode all speeds"), group);
    m_chkJs8MultiDecode->setToolTip(uiText("js8_multi_decode_tooltip", "Also decode the other JS8 speeds (Normal, Fast, Turbo, Slow) at the same time. Needs more CPU. Transmission uses the selected speed."));
    m_chkJs8MultiDecode->setChecked(settings.value(QStringLiteral("JS8/multiDecode"), false).toBool());
    grid->addWidget(m_chkJs8MultiDecode, row++, 1, 1, 3);

    m_cmbJs8Depth = new QComboBox(group);
    m_cmbJs8Depth->addItem(QStringLiteral("Fast"), 1);
    m_cmbJs8Depth->addItem(QStringLiteral("Normal"), 2);
    m_cmbJs8Depth->addItem(QStringLiteral("Deep"), 3);
    m_cmbJs8Depth->setCurrentIndex(qMax(0, m_cmbJs8Depth->findData(settings.value(QStringLiteral("JS8/decodeDepth"), 3).toInt())));
    m_cmbJs8Depth->setToolTip(uiText("js8_depth_tooltip", "Fast: one pass. Normal: subtracts decoded signals and decodes again. Deep: also ordered-statistics decoding (more CPU)."));
    grid->addWidget(new QLabel(uiText("decode_depth", "Decode depth"), group), row, 0);
    grid->addWidget(m_cmbJs8Depth, row++, 1, 1, 3);

    m_spinJs8RxFreq = new QSpinBox(group);
    m_spinJs8RxFreq->setRange(Js8Mode::minimumFrequencyHz(), 2900);
    m_spinJs8RxFreq->setSuffix(QStringLiteral(" Hz"));
    m_spinJs8RxFreq->setValue(settings.value(QStringLiteral("JS8/rxFrequency"), Js8Mode::defaultRxFrequencyHz()).toInt());
    m_spinJs8TxFreq = new QSpinBox(group);
    m_spinJs8TxFreq->setRange(Js8Mode::minimumFrequencyHz(), 2900);
    m_spinJs8TxFreq->setSuffix(QStringLiteral(" Hz"));
    m_spinJs8TxFreq->setValue(settings.value(QStringLiteral("JS8/txFrequency"), Js8Mode::defaultTxFrequencyHz()).toInt());
    grid->addWidget(new QLabel(QStringLiteral("RX"), group), row, 0);
    grid->addWidget(m_spinJs8RxFreq, row, 1);
    grid->addWidget(new QLabel(QStringLiteral("TX"), group), row, 2);
    grid->addWidget(m_spinJs8TxFreq, row++, 3);

    m_chkJs8TxFollowsRx = new QCheckBox(uiText("js8_tx_follows_rx", "TX offset follows RX offset"), group);
    m_chkJs8TxFollowsRx->setChecked(settings.value(QStringLiteral("JS8/txFollowsRx"), true).toBool());
    grid->addWidget(m_chkJs8TxFollowsRx, row++, 0, 1, 4);

    m_btnJs8Rx = new QPushButton(QStringLiteral("RX"), group);
    m_btnJs8Stop = new QPushButton(QStringLiteral("STOP"), group);
    grid->addWidget(m_btnJs8Rx, row, 0, 1, 2);
    grid->addWidget(m_btnJs8Stop, row++, 2, 1, 2);
    auto *clear = new QPushButton(uiText("clear_messages", "Clear messages"), group);
    grid->addWidget(clear, row++, 0, 1, 4);

    m_lblJs8Status = new QLabel(group);
    m_lblJs8Status->setWordWrap(true);
    grid->addWidget(m_lblJs8Status, row++, 0, 1, 4);
    if (!Js8Message::dictionaryAvailable())
        m_lblJs8Status->setText(uiText("js8_no_dictionary", "JS8 word tables missing: free text cannot be compressed or read."));
    grid->setColumnStretch(1, 1);
    grid->setColumnStretch(3, 1);

    // Automatic operation: replies, heartbeat/CQ repeats, relay.
    auto *autoGroup = new QGroupBox(uiText("js8_auto_group", "JS8 automatic operation"), m_pageJs8Settings);
    auto *autoGrid = new QGridLayout(autoGroup);
    autoGrid->setContentsMargins(8, 8, 8, 8);
    int arow = 0;
    m_chkJs8Auto = new QCheckBox(uiText("js8_auto_short", "AUTO: answer queries"), autoGroup);
    m_chkJs8Auto->setToolTip(uiText("js8_auto_tooltip", "Answers SNR?, GRID?, INFO?, STATUS?, HEARING?, AGN?, QUERY MSGS and acknowledges MSG without asking, as JS8Call's AUTO. Off: a reply is only prepared in the message field."));
    m_chkJs8Auto->setChecked(settings.value(QStringLiteral("JS8/auto"), false).toBool());
    autoGrid->addWidget(m_chkJs8Auto, arow++, 0, 1, 4);
    m_chkJs8HbAck = new QCheckBox(uiText("js8_hb_ack_short", "Acknowledge heartbeats"), autoGroup);
    m_chkJs8HbAck->setChecked(settings.value(QStringLiteral("JS8/heartbeatAck"), false).toBool());
    m_chkJs8HbAck->setToolTip(uiText("js8_hb_ack_tooltip", "With AUTO, answer heartbeats with HEARTBEAT SNR (at most once per station in 15 minutes)."));
    autoGrid->addWidget(m_chkJs8HbAck, arow++, 0, 1, 4);
    m_chkJs8Relay = new QCheckBox(uiText("js8_relay_short", "Relay and store messages"), autoGroup);
    m_chkJs8Relay->setChecked(settings.value(QStringLiteral("JS8/relay"), true).toBool());
    m_chkJs8Relay->setToolTip(uiText("js8_relay_tooltip", "Forward relay messages (CALL>CALL text) and keep MSG TO: messages for other stations until they fetch them."));
    autoGrid->addWidget(m_chkJs8Relay, arow++, 0, 1, 4);

    m_cmbJs8HbInterval = new QComboBox(autoGroup);
    fillIntervalCombo(m_cmbJs8HbInterval, {0, 10, 15, 30, 60}, uiText("js8_off", "Off"));
    m_cmbJs8HbInterval->setCurrentIndex(qMax(0, m_cmbJs8HbInterval->findData(settings.value(QStringLiteral("JS8/heartbeatMinutes"), 0).toInt())));
    m_cmbJs8HbInterval->setToolTip(uiText("js8_hb_interval_tooltip", "Repeats a heartbeat on a free offset between 500 and 1000 Hz. Paused while a station is selected."));
    m_cmbJs8CqInterval = new QComboBox(autoGroup);
    fillIntervalCombo(m_cmbJs8CqInterval, {0, 5, 10, 15, 30}, uiText("js8_off", "Off"));
    m_cmbJs8CqInterval->setCurrentIndex(qMax(0, m_cmbJs8CqInterval->findData(settings.value(QStringLiteral("JS8/cqMinutes"), 0).toInt())));
    m_cmbJs8CqInterval->setToolTip(uiText("js8_cq_interval_tooltip", "Repeats CQ on the TX offset. Paused while a station is selected."));
    autoGrid->addWidget(new QLabel(uiText("js8_hb_every", "Heartbeat every"), autoGroup), arow, 0);
    autoGrid->addWidget(m_cmbJs8HbInterval, arow++, 1, 1, 3);
    autoGrid->addWidget(new QLabel(uiText("js8_cq_every", "CQ every"), autoGroup), arow, 0);
    autoGrid->addWidget(m_cmbJs8CqInterval, arow++, 1, 1, 3);

    m_spinJs8Watchdog = new QSpinBox(autoGroup);
    m_spinJs8Watchdog->setRange(0, 600);
    m_spinJs8Watchdog->setSuffix(QStringLiteral(" min"));
    m_spinJs8Watchdog->setSpecialValueText(uiText("js8_off", "Off"));
    m_spinJs8Watchdog->setValue(settings.value(QStringLiteral("JS8/watchdogMinutes"), 60).toInt());
    m_spinJs8Watchdog->setToolTip(uiText("js8_watchdog_tooltip", "Stops automatic transmissions (replies, heartbeat, CQ) after this time without operator action."));
    autoGrid->addWidget(new QLabel(uiText("js8_watchdog", "Idle watchdog"), autoGroup), arow, 0);
    autoGrid->addWidget(m_spinJs8Watchdog, arow++, 1, 1, 3);

    m_editJs8Info = new QLineEdit(autoGroup);
    m_editJs8Info->setText(settings.value(QStringLiteral("JS8/info")).toString());
    m_editJs8Info->setPlaceholderText(uiText("js8_info_placeholder", "e.g. IC-7300 50W DIPOLE"));
    m_editJs8StatusText = new QLineEdit(autoGroup);
    m_editJs8StatusText->setText(settings.value(QStringLiteral("JS8/status")).toString());
    m_editJs8StatusText->setPlaceholderText(uiText("js8_status_placeholder", "e.g. IDLE, QRV"));
    autoGrid->addWidget(new QLabel(QStringLiteral("INFO"), autoGroup), arow, 0);
    autoGrid->addWidget(m_editJs8Info, arow++, 1, 1, 3);
    autoGrid->addWidget(new QLabel(QStringLiteral("STATUS"), autoGroup), arow, 0);
    autoGrid->addWidget(m_editJs8StatusText, arow++, 1, 1, 3);
    autoGrid->setColumnStretch(1, 1);
    autoGrid->setColumnStretch(3, 1);

    QWidget *qsoPanel = createQsoFormPanel(m_pageJs8Settings, QStringLiteral("JS8"), &m_js8QsoForm);

    outer->addWidget(group);
    outer->addWidget(autoGroup);
    outer->addWidget(qsoPanel);
    outer->addStretch(1);
    ui->stkModeSettings->addWidget(m_pageJs8Settings);

    m_js8Inbox.setPath(js8InboxPath());
    if (!m_js8Inbox.load()) appendLog(QStringLiteral("JS8 inbox could not be read: %1").arg(m_js8Inbox.path()));
    js8UpdateInboxButton();

    connect(m_cmbJs8Band, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        js8Settings().setValue(QStringLiteral("JS8/band"), m_cmbJs8Band->currentText());
        js8OperatorActivity();
        m_js8Activity.clear();
        m_js8Commands.clear();
        js8RefreshActivity();
        js8QsyToSelectedBand();
        js8UpdateBandUi();
    });
    connect(m_cmbJs8Submode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        js8Settings().setValue(QStringLiteral("JS8/submode"), int(currentJs8Submode()));
        if (m_nativeWeakSignalTxPending) cancelNativeWeakSignalPeriodTx(QStringLiteral("JS8 speed changed"));
        if (m_chkJs8MultiDecode == nullptr || !m_chkJs8MultiDecode->isChecked()) {
            m_js8Activity.clear();
            js8RefreshActivity();
        }
        applyJs8Settings();
        if (m_waterfallWidget != nullptr && ui != nullptr && Js8Mode::isMode(ui->cmbMode->currentText()))
            m_waterfallWidget->setPeriodMs(js8WaterfallPeriodMs());
    });
    connect(m_chkJs8MultiDecode, &QCheckBox::toggled, this, [this](bool on) {
        js8Settings().setValue(QStringLiteral("JS8/multiDecode"), on);
        applyJs8Settings();
    });
    connect(m_cmbJs8Depth, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        js8Settings().setValue(QStringLiteral("JS8/decodeDepth"), m_cmbJs8Depth->currentData().toInt());
        applyJs8Settings();
    });
    connect(m_spinJs8RxFreq, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int hz) {
        js8Settings().setValue(QStringLiteral("JS8/rxFrequency"), hz);
        if (m_chkJs8TxFollowsRx->isChecked() && m_spinJs8TxFreq->value() != hz) m_spinJs8TxFreq->setValue(hz);
        applyJs8Settings();
    });
    connect(m_spinJs8TxFreq, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int hz) {
        js8Settings().setValue(QStringLiteral("JS8/txFrequency"), hz);
        applyJs8Settings();
    });
    connect(m_chkJs8TxFollowsRx, &QCheckBox::toggled, this, [this](bool on) {
        js8Settings().setValue(QStringLiteral("JS8/txFollowsRx"), on);
        if (on) m_spinJs8TxFreq->setValue(m_spinJs8RxFreq->value());
    });
    connect(m_chkJs8Auto, &QCheckBox::toggled, this, [this](bool on) {
        js8Settings().setValue(QStringLiteral("JS8/auto"), on);
        js8OperatorActivity();
        if (!on) m_js8TxQueue.clear();
    });
    connect(m_chkJs8HbAck, &QCheckBox::toggled, this, [](bool on) { js8Settings().setValue(QStringLiteral("JS8/heartbeatAck"), on); });
    connect(m_chkJs8Relay, &QCheckBox::toggled, this, [](bool on) { js8Settings().setValue(QStringLiteral("JS8/relay"), on); });
    connect(m_cmbJs8HbInterval, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        js8Settings().setValue(QStringLiteral("JS8/heartbeatMinutes"), m_cmbJs8HbInterval->currentData().toInt());
        js8OperatorActivity();
        js8ScheduleRepeats();
    });
    connect(m_cmbJs8CqInterval, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        js8Settings().setValue(QStringLiteral("JS8/cqMinutes"), m_cmbJs8CqInterval->currentData().toInt());
        js8OperatorActivity();
        js8ScheduleRepeats();
    });
    connect(m_spinJs8Watchdog, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int minutes) {
        js8Settings().setValue(QStringLiteral("JS8/watchdogMinutes"), minutes);
        js8OperatorActivity();
    });
    connect(m_editJs8Info, &QLineEdit::editingFinished, this, [this]() {
        js8Settings().setValue(QStringLiteral("JS8/info"), Js8Message::normalizeText(m_editJs8Info->text()));
    });
    connect(m_editJs8StatusText, &QLineEdit::editingFinished, this, [this]() {
        js8Settings().setValue(QStringLiteral("JS8/status"), Js8Message::normalizeText(m_editJs8StatusText->text()));
    });
    connect(m_btnJs8Rx, &QPushButton::clicked, this, &MainWindow::startJs8RxShell);
    connect(m_btnJs8Stop, &QPushButton::clicked, this, &MainWindow::stopJs8Shell);
    connect(clear, &QPushButton::clicked, this, &MainWindow::clearJs8Activity);

    m_js8Timer = new QTimer(this);
    m_js8Timer->setInterval(1000);
    connect(m_js8Timer, &QTimer::timeout, this, &MainWindow::js8Tick);
    m_js8Timer->start();
    m_js8LastOperatorMs = QDateTime::currentMSecsSinceEpoch();
    js8ScheduleRepeats();
    js8UpdateBandUi();

    if (m_js8Decoder == nullptr) {
        m_js8Decoder = new Js8Decoder();
        connect(m_js8Decoder, &Js8Decoder::statusChanged, m_js8Decoder, [](const QString &text) {
            SessionDiagnostics::instance().record("rx.status", {{"mode", "JS8"}, {"message", text}});
        }, Qt::DirectConnection);
        m_js8Thread = new QThread(this);
        m_js8Decoder->moveToThread(m_js8Thread);
        auto *queue = new BoundedAudioDispatcher(64);
        queue->moveToThread(m_js8Thread);
        connect(m_js8Thread, &QThread::finished, queue, &QObject::deleteLater);
        connect(m_audioEngine, &AudioEngine::audioBlockReady, queue,
                [queue, decoder = m_js8Decoder](const AudioBlock &block) {
                    if (decoder->liveInputEnabled()) queue->enqueue(block);
                }, Qt::DirectConnection);
        connect(queue, &BoundedAudioDispatcher::blocksAvailable, m_js8Decoder,
                [queue, decoder = m_js8Decoder]() {
                    int dropped = 0;
                    const auto blocks = queue->takePending(4, &dropped);
                    if (!decoder->liveInputEnabled()) return;
                    if (dropped) {
                        decoder->reset();
                        emit decoder->statusChanged(QStringLiteral("JS8 audio queue overflow: %1 block(s) lost; period reset.").arg(dropped));
                    }
                    for (const auto &block : blocks) decoder->processAudioBlock(block);
                }, Qt::QueuedConnection);
        connect(m_js8Thread, &QThread::finished, m_js8Decoder, &QObject::deleteLater);
        connect(m_js8Decoder, &Js8Decoder::decoded, this, &MainWindow::handleJs8Decode, Qt::QueuedConnection);
        connect(m_js8Decoder, &Js8Decoder::statusChanged, this, [this](const QString &s) {
            if (m_lblJs8Status != nullptr) m_lblJs8Status->setText(s);
            if (s.contains(QStringLiteral("overflow")) || s.contains(QStringLiteral("backlog")) || s.contains(QStringLiteral("failed")))
                appendLog(s);
        }, Qt::QueuedConnection);
        connect(m_js8Decoder, &Js8Decoder::periodDecoded, this, [this](int count, int submode) {
            if (m_lblJs8Status != nullptr && submode == int(currentJs8Submode()))
                m_lblJs8Status->setText(M3I18n::text(QStringLiteral("JS8 %1: %2 frame(s) in the last period"))
                                            .arg(Js8Mode::submodeName(currentJs8Submode()))
                                            .arg(count));
        }, Qt::QueuedConnection);
        m_js8Thread->start();
    }
}

void MainWindow::applyJs8Settings()
{
    if (m_js8Decoder == nullptr) return;
    const Js8Mode::Submode submode = currentJs8Submode();
    const int depth = m_cmbJs8Depth != nullptr ? m_cmbJs8Depth->currentData().toInt() : 3;
    const int rx = m_spinJs8RxFreq != nullptr ? m_spinJs8RxFreq->value() : 1500;
    const int tx = m_spinJs8TxFreq != nullptr ? m_spinJs8TxFreq->value() : 1500;
    const bool multi = m_chkJs8MultiDecode != nullptr && m_chkJs8MultiDecode->isChecked();
    const int maxTx = Js8Mode::maximumFrequencyHz(submode);
    if (m_spinJs8TxFreq != nullptr) m_spinJs8TxFreq->setMaximum(maxTx);
    QMetaObject::invokeMethod(m_js8Decoder, [decoder = m_js8Decoder, submode, depth, rx, tx, multi]() {
        decoder->setMultiDecode(multi);
        decoder->setSubmode(submode);
        decoder->setDecodeDepth(depth);
        decoder->setRxFrequencyHz(rx);
        decoder->setTxFrequencyHz(tx);
    }, Qt::QueuedConnection);
    updateWaterfallMarkers();
}

void MainWindow::startJs8RxShell()
{
    if (ui != nullptr && ui->cmbMode != nullptr && !Js8Mode::isMode(ui->cmbMode->currentText()))
        ui->cmbMode->setCurrentText(Js8Mode::familyName());
    applyJs8Settings();
    startRx();
}

void MainWindow::stopJs8Shell()
{
    js8OperatorActivity();
    if (!m_js8TxQueue.isEmpty()) {
        m_js8TxQueue.clear();
        js8ScheduleRepeats();
    }
    if (m_nativeWeakSignalTxPending) {
        cancelNativeWeakSignalPeriodTx(QStringLiteral("operator STOP"));
        m_js8PendingFrames.clear();
        if (m_lblJs8TxStatus != nullptr) m_lblJs8TxStatus->setText(uiText("js8_tx_cancelled", "Transmission cancelled."));
        return;
    }
    if (m_txRunning || m_txPreparationPending) { stopImageTx(); return; }
    if (m_rxRunning || m_rxStartPending) stopRx();
}

void MainWindow::js8SetOffset(int hz, bool rx, bool tx)
{
    if (rx && m_spinJs8RxFreq != nullptr) m_spinJs8RxFreq->setValue(qBound(m_spinJs8RxFreq->minimum(), hz, m_spinJs8RxFreq->maximum()));
    if (tx && m_spinJs8TxFreq != nullptr) m_spinJs8TxFreq->setValue(qBound(m_spinJs8TxFreq->minimum(), hz, m_spinJs8TxFreq->maximum()));
}

void MainWindow::updateJs8WaterfallMarkers()
{
    if (m_waterfallWidget == nullptr) return;
    const double bw = Js8Mode::parameters(currentJs8Submode()).bandwidthHz();
    QVector<FrequencyMarker> markers;
    FrequencyMarker rx;
    rx.frequencyHz = m_spinJs8RxFreq != nullptr ? m_spinJs8RxFreq->value() : 1500.0;
    rx.bandwidthHz = bw;
    rx.label = QStringLiteral("RX");
    rx.color = QColor(80, 255, 120);
    rx.width = 2;
    rx.dashed = false;
    markers.append(rx);
    FrequencyMarker tx;
    tx.frequencyHz = m_spinJs8TxFreq != nullptr ? m_spinJs8TxFreq->value() : 1500.0;
    tx.bandwidthHz = bw;
    tx.label = QStringLiteral("TX");
    tx.color = QColor(255, 80, 80);
    tx.width = 2;
    tx.dashed = true;
    markers.append(tx);
    m_waterfallWidget->setMarkers(markers);
    m_waterfallWidget->setTextOverlays({});
}

void MainWindow::js8AppendRx(const QString &line, const QColor &colour)
{
    if (m_txtJs8Rx == nullptr) return;
    QTextCursor cursor(m_txtJs8Rx->document());
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat format;
    if (colour.isValid()) format.setForeground(colour);
    if (!m_txtJs8Rx->document()->isEmpty()) cursor.insertBlock();
    cursor.insertText(line, format);
    m_txtJs8Rx->verticalScrollBar()->setValue(m_txtJs8Rx->verticalScrollBar()->maximum());
}

void MainWindow::handleJs8Decode(const Js8Decode &decode)
{
    SessionDiagnostics::instance().record("rx.message.delivery", {{"mode", "JS8"}, {"message", decode.text},
                                                                  {"snr", decode.snrDb}, {"frequency", decode.frequencyHz}});
    const bool multi = m_chkJs8MultiDecode != nullptr && m_chkJs8MultiDecode->isChecked();
    if (!multi && decode.submode != currentJs8Submode()) return;
    const double tolerance = Js8Mode::parameters(decode.submode).bandwidthHz() / 2.0;
    const QString myCall = stationCallsign().trimmed().toUpper();
    const auto knownCall = [](const QString &call) {
        return !call.isEmpty() && !call.startsWith(QLatin1Char('@')) && !call.contains(QStringLiteral("...."));
    };

    // Band activity: one running text per offset and speed.
    int index = -1;
    for (int i = 0; i < m_js8Activity.size(); ++i)
        if (m_js8Activity.at(i).submode == decode.submode && std::fabs(m_js8Activity.at(i).offsetHz - decode.frequencyHz) <= tolerance) {
            index = i;
            break;
        }
    if (index < 0) {
        m_js8Activity.append(Js8Activity());
        index = m_js8Activity.size() - 1;
    }
    Js8Activity &a = m_js8Activity[index];
    if (a.complete || decode.first) {
        if (!a.text.isEmpty() && !a.text.endsWith(QLatin1Char(' '))) a.text += QLatin1Char(' ');
        a.from.clear();
        a.current.clear();
    }
    a.offsetHz = decode.frequencyHz;
    a.utc = decode.utc;
    a.snrDb = decode.snrDb;
    a.submode = decode.submode;
    a.text += decode.text;
    a.current += decode.text;
    if (a.text.size() > kJs8MaxTextChars) a.text = a.text.right(kJs8MaxTextChars);
    if (knownCall(decode.from)) a.from = decode.from;
    a.complete = decode.last;
    while (m_js8Activity.size() > kJs8MaxActivityRows) m_js8Activity.removeFirst();

    // Heard stations.
    QString from = decode.from;
    if (!knownCall(from) && (decode.directed || decode.compound)) from = a.from;
    if (knownCall(from) && from != myCall) {
        Js8Heard &h = m_js8Heard[from];
        h.call = from;
        if (!decode.grid.isEmpty()) h.grid = decode.grid.left(6);
        h.snrDb = decode.snrDb;
        h.utc = decode.utc;
        h.offsetHz = decode.frequencyHz;
        if (m_js8Heard.size() > kJs8MaxHeard) {
            auto oldest = m_js8Heard.begin();
            for (auto it = m_js8Heard.begin(); it != m_js8Heard.end(); ++it)
                if (it->utc < oldest->utc) oldest = it;
            m_js8Heard.erase(oldest);
        }
    }

    // Conversation window: my RX offset, messages to me, @ALLCALL and CQ.
    const int rxHz = m_spinJs8RxFreq != nullptr ? m_spinJs8RxFreq->value() : 1500;
    const bool atRx = decode.submode == currentJs8Submode() && std::fabs(decode.frequencyHz - rxHz) <= tolerance;
    // The whole message counts: a directed header names the target, the
    // data frames that follow do not.
    const bool toMe = !myCall.isEmpty() && (decode.to == myCall || a.current.contains(myCall));
    const bool allCall = a.current.contains(QStringLiteral("@ALLCALL"));
    if (decode.last && (atRx || toMe || allCall)) {
        const QString line = QStringLiteral("%1  %2 dB  %3 Hz  %4")
                                 .arg(decode.utc.toUTC().time().toString(QStringLiteral("HH:mm:ss")))
                                 .arg(decode.snrDb)
                                 .arg(qRound(decode.frequencyHz))
                                 .arg(a.current.trimmed());
        js8AppendRx(line, toMe ? QColor(255, 120, 60) : QColor());
        if (toMe) {
            appendLog(QStringLiteral("JS8 to %1: %2").arg(myCall, a.current.trimmed()));
            QApplication::alert(this);
        }
    }

    for (const Js8Command &command : m_js8Commands.add(decode)) js8ProcessCommand(command);

    js8RefreshActivity();
    js8RefreshCalls();
}

QStringList MainWindow::js8RecentCalls() const
{
    QVector<Js8Heard> rows;
    const QDateTime now = QDateTime::currentDateTimeUtc();
    for (const Js8Heard &h : m_js8Heard)
        if (h.utc.isValid() && h.utc.msecsTo(now) <= kJs8HeardAgingMs) rows << h;
    std::sort(rows.begin(), rows.end(), [](const Js8Heard &l, const Js8Heard &r) { return l.utc > r.utc; });
    QStringList calls;
    for (const Js8Heard &h : rows) calls << h.call;
    return calls;
}

void MainWindow::js8ProcessCommand(const Js8Command &command)
{
    SessionDiagnostics::instance().record("rx.js8.command", {{"from", command.from}, {"to", command.to},
                                                             {"command", command.command}, {"text", command.text}});
    // Grids from heartbeats, CQs and " GRID" messages go to the heard list.
    if (!command.from.isEmpty() && m_js8Heard.contains(command.from)) {
        QString grid = command.grid;
        if (grid.isEmpty() && command.command == QStringLiteral(" GRID")) grid = command.text.section(QLatin1Char(' '), 0, 0);
        if (grid.size() >= 4 && grid.size() <= 8) m_js8Heard[command.from].grid = grid.left(6);
    }

    Js8ReplyContext ctx;
    ctx.myCall = stationCallsign().trimmed().toUpper();
    ctx.myGrid = stationLocator().trimmed().toUpper();
    ctx.info = m_editJs8Info != nullptr ? Js8Message::normalizeText(m_editJs8Info->text()) : QString();
    ctx.status = m_editJs8StatusText != nullptr ? Js8Message::normalizeText(m_editJs8StatusText->text()) : QString();
    ctx.autoReply = m_chkJs8Auto != nullptr && m_chkJs8Auto->isChecked() && !m_js8WatchdogTripped;
    ctx.heartbeatAck = m_chkJs8HbAck != nullptr && m_chkJs8HbAck->isChecked();
    ctx.relay = m_chkJs8Relay == nullptr || m_chkJs8Relay->isChecked();
    ctx.heardRecentFirst = js8RecentCalls();
    ctx.heardReport = [this](const QString &call) -> QString {
        const QDateTime now = QDateTime::currentDateTimeUtc();
        for (const Js8Heard &h : m_js8Heard) {
            if ((h.call == call || Js8Message::baseCallsign(h.call) == call) && h.utc.msecsTo(now) <= kJs8HeardAgingMs)
                return QStringLiteral("%1 (%2)").arg(Js8Message::formatSnr(h.snrDb), Js8AutoReply::since(h.utc, now)).trimmed();
        }
        return QString();
    };
    ctx.lastTx = m_js8LastTxText;
    ctx.inbox = &m_js8Inbox;
    ctx.dialHz = m_lastRigFrequencyHz;
    const Js8ReplyAction action = Js8AutoReply::process(command, ctx);

    if (!action.note.isEmpty()) appendLog(action.note);
    if (action.inboxId > 0) {
        js8AppendRx(QStringLiteral("%1  %2").arg(QDateTime::currentDateTimeUtc().time().toString(QStringLiteral("HH:mm:ss")),
                                                 M3I18n::text(QStringLiteral("JS8 inbox: new message from %1")).arg(command.from)),
                    QColor(255, 120, 60));
        js8UpdateInboxButton();
        QApplication::alert(this);
    }
    if (action.storedId > 0) js8UpdateInboxButton();
    if (action.reply.isEmpty()) return;

    const bool automatic = m_chkJs8Auto != nullptr && m_chkJs8Auto->isChecked();
    if (automatic && !m_js8WatchdogTripped) {
        const bool broadcast = action.allCall || action.lowPriority;
        const qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
        if (broadcast) {
            const qint64 last = m_js8AllCallReplyMs.value(command.from, 0);
            if (last > 0 && nowMs - last < kJs8AllCallReplyMs) return;
            m_js8AllCallReplyMs.insert(command.from, nowMs);
        }
        // Heartbeat acknowledgements go to a free offset of the HB sub-band;
        // answers to directed queries to the TX offset.
        js8Enqueue(action.reply, action.lowPriority,
                   QStringLiteral("reply to %1%2").arg(command.from, command.command),
                   action.lowPriority ? js8FreeHeartbeatOffset() : 0);
    } else if (action.toMe && !action.allCall && m_editJs8Tx != nullptr && m_editJs8Tx->text().trimmed().isEmpty()) {
        m_editJs8Tx->setText(action.reply);
        if (m_lblJs8TxStatus != nullptr)
            m_lblJs8TxStatus->setText(uiText("js8_reply_prepared", "Reply prepared in the message field; press Send to transmit it (AUTO is off)."));
    }
}

void MainWindow::js8RefreshActivity()
{
    if (m_tableJs8Band == nullptr) return;
    QVector<Js8Activity> rows = m_js8Activity;
    std::sort(rows.begin(), rows.end(), [](const Js8Activity &l, const Js8Activity &r) { return l.offsetHz < r.offsetHz; });
    const QString myCall = stationCallsign().trimmed().toUpper();
    m_tableJs8Band->setRowCount(rows.size());
    for (int r = 0; r < rows.size(); ++r) {
        const Js8Activity &a = rows.at(r);
        auto *offset = new QTableWidgetItem(QString::number(qRound(a.offsetHz)));
        auto *utc = new QTableWidgetItem(a.utc.toUTC().time().toString(QStringLiteral("HH:mm:ss")));
        auto *snr = new QTableWidgetItem(QString::number(a.snrDb));
        auto *speed = new QTableWidgetItem(QString(QLatin1Char(Js8Mode::parameters(a.submode).letter)));
        speed->setData(Qt::UserRole, int(a.submode));
        speed->setToolTip(Js8Mode::submodeName(a.submode));
        auto *text = new QTableWidgetItem(a.text.trimmed() + (a.complete ? QStringLiteral(" ✓") : QString()));
        text->setData(Qt::UserRole, a.from);
        text->setToolTip(a.text.trimmed());
        const bool otherSpeed = a.submode != currentJs8Submode();
        if (!myCall.isEmpty() && a.text.contains(myCall)) {
            for (auto *item : {offset, utc, snr, speed, text}) item->setForeground(QColor(255, 120, 60));
        } else if (otherSpeed) {
            for (auto *item : {offset, utc, snr, speed, text}) item->setForeground(QColor(140, 140, 140));
        }
        m_tableJs8Band->setItem(r, 0, offset);
        m_tableJs8Band->setItem(r, 1, utc);
        m_tableJs8Band->setItem(r, 2, snr);
        m_tableJs8Band->setItem(r, 3, speed);
        m_tableJs8Band->setItem(r, 4, text);
    }
}

void MainWindow::js8RefreshCalls()
{
    if (m_tableJs8Calls == nullptr) return;
    QVector<Js8Heard> rows;
    for (const Js8Heard &h : m_js8Heard) rows << h;
    std::sort(rows.begin(), rows.end(), [](const Js8Heard &l, const Js8Heard &r) { return l.utc > r.utc; });
    m_tableJs8Calls->setRowCount(rows.size());
    for (int r = 0; r < rows.size(); ++r) {
        const Js8Heard &h = rows.at(r);
        m_tableJs8Calls->setItem(r, 0, new QTableWidgetItem(h.call));
        m_tableJs8Calls->setItem(r, 1, new QTableWidgetItem(QString::number(h.snrDb)));
        m_tableJs8Calls->setItem(r, 2, new QTableWidgetItem(h.grid));
        m_tableJs8Calls->setItem(r, 3, new QTableWidgetItem(h.utc.toUTC().time().toString(QStringLiteral("HH:mm:ss"))));
        m_tableJs8Calls->setItem(r, 4, new QTableWidgetItem(QString::number(qRound(h.offsetHz))));
    }
}

void MainWindow::clearJs8Activity()
{
    m_js8Activity.clear();
    m_js8Heard.clear();
    m_js8Commands.clear();
    if (m_txtJs8Rx != nullptr) m_txtJs8Rx->clear();
    js8RefreshActivity();
    js8RefreshCalls();
}

void MainWindow::js8SendFromEditor()
{
    if (m_editJs8Tx == nullptr) return;
    const QString text = m_editJs8Tx->text();
    if (text.trimmed().isEmpty()) return;
    js8Send(text, m_editJs8To != nullptr ? m_editJs8To->text().trimmed().toUpper() : QString());
    if (!m_js8PendingFrames.isEmpty()) m_editJs8Tx->clear();
}

void MainWindow::js8Send(const QString &text, const QString &to)
{
    js8OperatorActivity();
    js8Transmit(text, to, false);
}

bool MainWindow::js8Transmit(const QString &text, const QString &to, bool automatic, int txHz)
{
    if (ui != nullptr && ui->cmbMode != nullptr && !Js8Mode::isMode(ui->cmbMode->currentText())) {
        if (automatic) return false;
        ui->cmbMode->setCurrentText(Js8Mode::familyName());
    }
    const QString myCall = stationCallsign().trimmed().toUpper();
    if (myCall.isEmpty()) {
        m_lblJs8TxStatus->setText(uiText("js8_need_call", "Enter your callsign in the settings first."));
        return false;
    }
    if (m_txRunning || m_nativeWeakSignalTxPending || m_txPreparationPending) {
        if (!automatic)
            m_lblJs8TxStatus->setText(uiText("js8_tx_busy", "A transmission is already running or armed; press STOP to cancel it."));
        return false;
    }
    const Js8Mode::Submode submode = currentJs8Submode();
    const auto frames = Js8Message::buildFrames(myCall, stationLocator(), to, text, submode, true);
    if (frames.isEmpty()) {
        m_lblJs8TxStatus->setText(uiText("js8_cannot_encode", "This text cannot be sent in JS8."));
        return false;
    }
    m_js8PendingFrames = frames;
    m_js8PendingText = Js8Message::preview(frames, submode).trimmed();
    m_js8PendingTxHz = txHz;
    m_js8LastTxText = Js8Message::normalizeText(text);
    const int hz = txHz > 0 ? txHz : (m_spinJs8TxFreq != nullptr ? m_spinJs8TxFreq->value() : 1500);
    const Js8Mode::Parameters p = Js8Mode::parameters(submode);
    m_lblJs8TxStatus->setText(M3I18n::text(QStringLiteral("JS8 TX: %1 frame(s), %2 s: %3"))
                                  .arg(frames.size())
                                  .arg(frames.size() * p.periodSeconds)
                                  .arg(m_js8PendingText));
    js8AppendRx(QStringLiteral("%1  %2  %3 Hz  %4")
                    .arg(QDateTime::currentDateTimeUtc().time().toString(QStringLiteral("HH:mm:ss")),
                         automatic ? QStringLiteral("AUTO") : QStringLiteral("TX"))
                    .arg(hz)
                    .arg(m_js8PendingText),
                QColor(255, 80, 80));
    startImageTx();
    const bool started = m_nativeWeakSignalTxPending || m_txRunning || m_txPreparationPending;
    if (!started && automatic) m_js8PendingFrames.clear();
    return started;
}

void MainWindow::js8Enqueue(const QString &text, bool lowPriority, const QString &reason, int txHz)
{
    const QString clean = Js8Message::normalizeText(text);
    if (clean.isEmpty()) return;
    for (const Js8QueuedTx &q : m_js8TxQueue)
        if (q.text == clean) return;
    if (m_js8TxQueue.size() >= kJs8MaxQueue) {
        appendLog(QStringLiteral("JS8 TX queue full; dropped: %1").arg(clean));
        return;
    }
    Js8QueuedTx item;
    item.text = clean;
    item.lowPriority = lowPriority;
    item.reason = reason;
    item.txHz = txHz;
    item.queuedMs = QDateTime::currentMSecsSinceEpoch();
    // Replies go before heartbeats, CQs and acknowledgements.
    int pos = m_js8TxQueue.size();
    if (!lowPriority) {
        pos = 0;
        while (pos < m_js8TxQueue.size() && !m_js8TxQueue.at(pos).lowPriority) ++pos;
    }
    m_js8TxQueue.insert(pos, item);
    appendLog(QStringLiteral("JS8 queued (%1): %2").arg(reason, clean));
}

void MainWindow::js8OperatorActivity()
{
    m_js8LastOperatorMs = QDateTime::currentMSecsSinceEpoch();
    if (m_js8WatchdogTripped) {
        m_js8WatchdogTripped = false;
        js8ScheduleRepeats();
        if (m_lblJs8TxStatus != nullptr) m_lblJs8TxStatus->clear();
    }
}

void MainWindow::js8ScheduleRepeats()
{
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    const int hb = m_cmbJs8HbInterval != nullptr ? m_cmbJs8HbInterval->currentData().toInt() : 0;
    const int cq = m_cmbJs8CqInterval != nullptr ? m_cmbJs8CqInterval->currentData().toInt() : 0;
    // The first repeat goes out soon after enabling, then every interval.
    m_js8NextHeartbeatMs = hb > 0 ? now + 5000 : 0;
    m_js8NextCqMs = cq > 0 ? now + 5000 : 0;
}

int MainWindow::js8FreeHeartbeatOffset() const
{
    // JS8Call sends heartbeats in 500..1000 Hz on an offset without recent
    // activity (findFreeFreqOffset(500, 1000, 50)).
    const Js8Mode::Submode submode = currentJs8Submode();
    const double bw = Js8Mode::parameters(submode).bandwidthHz();
    const QDateTime now = QDateTime::currentDateTimeUtc();
    for (int f = 500; f + bw <= 1000.0; f += 50) {
        bool busy = false;
        for (const Js8Activity &a : m_js8Activity) {
            if (!a.utc.isValid() || a.utc.secsTo(now) > 120) continue;
            const double abw = Js8Mode::parameters(a.submode).bandwidthHz();
            if (a.offsetHz < f + bw + 10.0 && a.offsetHz + abw > f - 10.0) { busy = true; break; }
        }
        if (!busy) return f;
    }
    return m_spinJs8TxFreq != nullptr ? m_spinJs8TxFreq->value() : 1500;
}

int MainWindow::js8WaterfallPeriodMs() const
{
    return Js8Mode::parameters(currentJs8Submode()).periodSeconds * 1000;
}

void MainWindow::js8Tick()
{
    const QDateTime nowUtc = QDateTime::currentDateTimeUtc();
    for (const Js8Command &command : m_js8Commands.expire(nowUtc)) js8ProcessCommand(command);
    if (ui == nullptr || ui->cmbMode == nullptr || !Js8Mode::isMode(ui->cmbMode->currentText())) return;

    const qint64 now = nowUtc.toMSecsSinceEpoch();
    const int hbMinutes = m_cmbJs8HbInterval != nullptr ? m_cmbJs8HbInterval->currentData().toInt() : 0;
    const int cqMinutes = m_cmbJs8CqInterval != nullptr ? m_cmbJs8CqInterval->currentData().toInt() : 0;
    const bool autoReply = m_chkJs8Auto != nullptr && m_chkJs8Auto->isChecked();

    // Idle watchdog: no unattended transmissions for ever.
    const int watchdogMin = m_spinJs8Watchdog != nullptr ? m_spinJs8Watchdog->value() : 0;
    if (!m_js8WatchdogTripped && watchdogMin > 0 && (autoReply || hbMinutes > 0 || cqMinutes > 0) &&
        now - m_js8LastOperatorMs > qint64(watchdogMin) * 60000) {
        m_js8WatchdogTripped = true;
        m_js8TxQueue.clear();
        const QString text = uiText("js8_watchdog_tripped", "Automatic JS8 transmissions stopped by the idle watchdog. Any operator action resumes them.");
        if (m_lblJs8TxStatus != nullptr) m_lblJs8TxStatus->setText(text);
        appendLog(text);
    }
    if (m_js8WatchdogTripped || !m_rxRunning) return;

    // Heartbeat and CQ repeats pause while a station is selected (QSO).
    const bool inQso = m_editJs8To != nullptr && !m_editJs8To->text().trimmed().isEmpty();
    if (hbMinutes > 0 && m_js8NextHeartbeatMs > 0 && now >= m_js8NextHeartbeatMs) {
        m_js8NextHeartbeatMs = now + qint64(hbMinutes) * 60000;
        if (!inQso)
            js8Enqueue(stationCallsign() + QStringLiteral(": ") + Js8Message::heartbeatText(stationLocator()), true,
                       QStringLiteral("heartbeat"), js8FreeHeartbeatOffset());
    }
    if (cqMinutes > 0 && m_js8NextCqMs > 0 && now >= m_js8NextCqMs) {
        m_js8NextCqMs = now + qint64(cqMinutes) * 60000;
        if (!inQso) js8Enqueue(Js8Message::cqText(stationLocator()), true, QStringLiteral("CQ"));
    }

    // Drop stale queued items (older than five minutes), then send the next.
    for (int i = m_js8TxQueue.size() - 1; i >= 0; --i)
        if (now - m_js8TxQueue.at(i).queuedMs > 5 * 60000) m_js8TxQueue.removeAt(i);
    if (m_js8TxQueue.isEmpty() || m_txRunning || m_nativeWeakSignalTxPending || m_txPreparationPending) return;
    const Js8QueuedTx next = m_js8TxQueue.first();
    // Replies need AUTO; repeats were enabled explicitly by the operator.
    if (!next.lowPriority && !autoReply) {
        m_js8TxQueue.removeFirst();
        return;
    }
    if (js8Transmit(next.text, QString(), true, next.txHz)) m_js8TxQueue.removeFirst();
}

void MainWindow::js8UpdateBandUi()
{
    if (m_cmbJs8Band == nullptr || m_lblJs8BandStatus == nullptr) return;
    const double standard = M3StandardFrequencies::dialHz(QStringLiteral("JS8"), m_cmbJs8Band->currentText());
    const bool haveCat = m_lastRigFrequencyHz > 0.0;
    // JS8 signals sit up to ~3 kHz above the dial; within 1 kHz counts as on the standard.
    const bool onSlot = haveCat && standard > 0.0 && qAbs(m_lastRigFrequencyHz - standard) <= 1000.0;
    QString text = uiText("js8_standard_frequency", "Standard JS8 frequency") + QStringLiteral(": ") +
                   (standard > 0.0 ? formatRigFrequency(standard) : QStringLiteral("--"));
    if (haveCat && !onSlot) text += QStringLiteral(" — ") + uiText("js8_off_standard", "off the JS8 standard frequency");
    m_lblJs8BandStatus->setText(text);
    M3Ui::setSemanticRole(m_lblJs8BandStatus, onSlot || !haveCat ? QStringLiteral("positive") : QStringLiteral("negative"));
}

void MainWindow::js8RefreshBandList()
{
    if (m_cmbJs8Band == nullptr) return;
    const QString current = m_cmbJs8Band->currentText();
    const QStringList bands = M3StandardFrequencies::bands(QStringLiteral("JS8"));
    {
        const QSignalBlocker block(m_cmbJs8Band);
        m_cmbJs8Band->clear();
        m_cmbJs8Band->addItems(bands);
        const int idx = m_cmbJs8Band->findText(current);
        m_cmbJs8Band->setCurrentIndex(idx >= 0 ? idx : qMax(0, m_cmbJs8Band->findText(QStringLiteral("20m"))));
    }
    js8UpdateBandUi();
}

void MainWindow::js8QsyToSelectedBand()
{
    if (m_cmbJs8Band == nullptr) return;
    const double target = M3StandardFrequencies::dialHz(QStringLiteral("JS8"), m_cmbJs8Band->currentText());
    if (target <= 0.0) return;
    if (!m_settings.hamlibCatEnabled) {
        appendLog(QStringLiteral("JS8 band %1 selected; CAT is disabled, rig not tuned.").arg(m_cmbJs8Band->currentText()));
        return;
    }
    if (m_lastRigFrequencyHz > 0.0 && qAbs(m_lastRigFrequencyHz - target) < 1.0) return;   // already there
    tuneRigTo(target, QStringLiteral("JS8 ") + m_cmbJs8Band->currentText());
    js8Settings().setValue(QStringLiteral("JS8/band"), m_cmbJs8Band->currentText());
}

void MainWindow::updateJs8BandFromCat(double frequencyHz)
{
    if (m_cmbJs8Band == nullptr || frequencyHz <= 0.0) return;
    const QString band = bandFromFrequencyHz(frequencyHz);
    const int idx = band.isEmpty() ? -1 : m_cmbJs8Band->findText(band);
    if (idx >= 0 && idx != m_cmbJs8Band->currentIndex()) {
        const QSignalBlocker block(m_cmbJs8Band);
        m_cmbJs8Band->setCurrentIndex(idx);
        js8Settings().setValue(QStringLiteral("JS8/band"), band);
    }
    js8UpdateBandUi();
}

void MainWindow::js8UpdateInboxButton()
{
    if (m_btnJs8Inbox == nullptr) return;
    const int unread = m_js8Inbox.unreadCount();
    m_btnJs8Inbox->setText(unread > 0 ? QStringLiteral("%1 (%2)").arg(uiText("js8_inbox", "Inbox")).arg(unread)
                                      : uiText("js8_inbox", "Inbox"));
    M3Ui::setSemanticRole(m_btnJs8Inbox, unread > 0 ? QStringLiteral("warning") : QString());
}

void MainWindow::showJs8Inbox()
{
    js8OperatorActivity();
    QDialog dialog(this);
    dialog.setWindowTitle(uiText("js8_inbox_title", "JS8 inbox"));
    dialog.resize(760, 420);
    auto *layout = new QVBoxLayout(&dialog);
    auto *table = makeTable(&dialog, {QStringLiteral("UTC"), uiText("js8_inbox_from", "From"), uiText("js8_inbox_to", "To"),
                                      uiText("js8_inbox_state", "State"), uiText("js8_text_column", "Text")});
    table->setColumnWidth(0, 130);
    table->setColumnWidth(1, 90);
    table->setColumnWidth(2, 90);
    table->setColumnWidth(3, 90);
    layout->addWidget(table, 1);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, &dialog);
    auto *reply = buttons->addButton(uiText("js8_inbox_reply", "Reply"), QDialogButtonBox::ActionRole);
    auto *remove = buttons->addButton(uiText("js8_inbox_delete", "Delete"), QDialogButtonBox::ActionRole);
    layout->addWidget(buttons);

    auto stateText = [this](Js8StoredMessage::Type type) {
        switch (type) {
        case Js8StoredMessage::Type::Unread: return uiText("js8_inbox_unread", "new");
        case Js8StoredMessage::Type::Read: return uiText("js8_inbox_read", "read");
        case Js8StoredMessage::Type::Store: return uiText("js8_inbox_stored", "kept for station");
        case Js8StoredMessage::Type::Delivered: return uiText("js8_inbox_delivered", "delivered");
        }
        return QString();
    };
    auto fill = [this, table, stateText]() {
        QVector<Js8StoredMessage> list = m_js8Inbox.messages();
        std::sort(list.begin(), list.end(), [](const Js8StoredMessage &l, const Js8StoredMessage &r) { return l.id > r.id; });
        table->setRowCount(list.size());
        for (int r = 0; r < list.size(); ++r) {
            const Js8StoredMessage &m = list.at(r);
            auto *utc = new QTableWidgetItem(m.utc.toUTC().toString(QStringLiteral("yyyy-MM-dd HH:mm")));
            utc->setData(Qt::UserRole, m.id);
            auto *from = new QTableWidgetItem(m.path.contains(QLatin1Char('>')) ? m.path : m.from);
            auto *to = new QTableWidgetItem(m.to);
            auto *state = new QTableWidgetItem(stateText(m.type));
            auto *text = new QTableWidgetItem(m.text);
            text->setToolTip(m.text);
            if (m.type == Js8StoredMessage::Type::Unread)
                for (auto *item : {utc, from, to, state, text}) item->setForeground(QColor(255, 120, 60));
            table->setItem(r, 0, utc);
            table->setItem(r, 1, from);
            table->setItem(r, 2, to);
            table->setItem(r, 3, state);
            table->setItem(r, 4, text);
        }
    };
    fill();
    // Opening the inbox reads the new messages.
    for (const Js8StoredMessage &m : m_js8Inbox.messages())
        if (m.type == Js8StoredMessage::Type::Unread) m_js8Inbox.setType(m.id, Js8StoredMessage::Type::Read);
    js8UpdateInboxButton();

    auto selectedId = [table]() {
        const int row = table->currentRow();
        auto *item = row >= 0 ? table->item(row, 0) : nullptr;
        return item != nullptr ? item->data(Qt::UserRole).toInt() : -1;
    };
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    connect(remove, &QPushButton::clicked, &dialog, [this, selectedId, fill]() {
        const int id = selectedId();
        if (id > 0) {
            m_js8Inbox.remove(id);
            fill();
        }
    });
    connect(reply, &QPushButton::clicked, &dialog, [this, selectedId, &dialog]() {
        const Js8StoredMessage *m = m_js8Inbox.find(selectedId());
        if (m == nullptr || m_editJs8To == nullptr) return;
        m_editJs8To->setText(m->from);
        if (m_editJs8Tx != nullptr) {
            m_editJs8Tx->setText(QStringLiteral("MSG "));
            m_editJs8Tx->setFocus();
        }
        dialog.accept();
    });
    dialog.exec();
    js8UpdateInboxButton();
}
