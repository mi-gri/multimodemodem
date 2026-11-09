// SPDX-License-Identifier: GPL-3.0-or-later
// JS8 pages of the main window: band activity, heard stations, conversation,
// message entry and the RX/TX settings.  Protocol work lives in modems/js8.
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "audio/AudioEngine.h"
#include "audio/BoundedAudioDispatcher.h"
#include "runtime/SessionDiagnostics.h"
#include "settings/AppSettings.h"
#include "utils/RuntimeI18n.h"
#include "widgets/WaterfallWidget.h"

#include <QCheckBox>
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
#include <QVBoxLayout>

#include <algorithm>
#include <cmath>

namespace {
constexpr int kJs8MaxActivityRows = 80;
constexpr int kJs8MaxHeard = 200;
constexpr int kJs8MaxTextChars = 400;

QSettings js8Settings() { return QSettings(AppSettings::settingsFilePath(), QSettings::IniFormat); }

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
                                         uiText("js8_text_column", "Text")});
    m_tableJs8Band->setColumnWidth(0, 56);
    m_tableJs8Band->setColumnWidth(1, 70);
    m_tableJs8Band->setColumnWidth(2, 40);
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
    txRow->addWidget(toLabel);
    txRow->addWidget(m_editJs8To);
    txRow->addWidget(m_btnJs8ClearTo);
    txRow->addWidget(m_editJs8Tx, 1);
    txRow->addWidget(m_btnJs8Send);
    txRow->addWidget(m_btnJs8Cq);
    txRow->addWidget(m_btnJs8Hb);
    txRow->addWidget(m_btnJs8Snr);

    m_lblJs8TxStatus = new QLabel(m_js8DisplayPage);
    m_lblJs8TxStatus->setWordWrap(true);

    layout->addWidget(vertical, 1);
    layout->addLayout(txRow);
    layout->addWidget(m_lblJs8TxStatus);

    // Header and button texts are already localised or are protocol words.
    for (QObject *fixed : std::initializer_list<QObject *>{m_tableJs8Band, m_tableJs8Calls, m_btnJs8ClearTo, m_btnJs8Cq,
                                                           m_btnJs8Hb, m_btnJs8Snr})
        fixed->setProperty("i18nSkipSelf", true);

    connect(m_btnJs8Send, &QPushButton::clicked, this, &MainWindow::js8SendFromEditor);
    connect(m_editJs8Tx, &QLineEdit::returnPressed, this, &MainWindow::js8SendFromEditor);
    connect(m_btnJs8ClearTo, &QPushButton::clicked, this, [this]() { m_editJs8To->clear(); });
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
        auto *from = m_tableJs8Band->item(row, 3);
        const QString call = from != nullptr ? from->data(Qt::UserRole).toString() : QString();
        if (!call.isEmpty() && call != stationCallsign()) selectCall(call);
    });
    connect(m_tableJs8Band, &QTableWidget::cellDoubleClicked, this, [this](int row, int) {
        auto *item = m_tableJs8Band->item(row, 0);
        if (item != nullptr) js8SetOffset(item->text().toInt(), true, true);
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

    m_cmbJs8Submode = new QComboBox(group);
    for (Js8Mode::Submode s : Js8Mode::allSubmodes()) {
        const Js8Mode::Parameters p = Js8Mode::parameters(s);
        m_cmbJs8Submode->addItem(QStringLiteral("%1 (%2 s, %3 Hz)").arg(Js8Mode::submodeName(s)).arg(p.periodSeconds)
                                     .arg(qRound(p.bandwidthHz())), int(s));
    }
    m_cmbJs8Submode->setCurrentIndex(qMax(0, m_cmbJs8Submode->findData(settings.value(QStringLiteral("JS8/submode"), 0).toInt())));
    grid->addWidget(new QLabel(uiText("js8_speed", "Speed"), group), row, 0);
    grid->addWidget(m_cmbJs8Submode, row++, 1, 1, 3);

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

    QWidget *qsoPanel = createQsoFormPanel(m_pageJs8Settings, QStringLiteral("JS8"), &m_js8QsoForm);

    outer->addWidget(group);
    outer->addWidget(qsoPanel);
    outer->addStretch(1);
    ui->stkModeSettings->addWidget(m_pageJs8Settings);

    connect(m_cmbJs8Submode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        js8Settings().setValue(QStringLiteral("JS8/submode"), int(currentJs8Submode()));
        if (m_nativeWeakSignalTxPending) cancelNativeWeakSignalPeriodTx(QStringLiteral("JS8 speed changed"));
        m_js8Activity.clear();
        js8RefreshActivity();
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
    connect(m_btnJs8Rx, &QPushButton::clicked, this, &MainWindow::startJs8RxShell);
    connect(m_btnJs8Stop, &QPushButton::clicked, this, &MainWindow::stopJs8Shell);
    connect(clear, &QPushButton::clicked, this, &MainWindow::clearJs8Activity);

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
        connect(m_js8Decoder, &Js8Decoder::periodDecoded, this, [this](int count, int) {
            if (m_lblJs8Status != nullptr)
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
    const int maxTx = Js8Mode::maximumFrequencyHz(submode);
    if (m_spinJs8TxFreq != nullptr) m_spinJs8TxFreq->setMaximum(maxTx);
    QMetaObject::invokeMethod(m_js8Decoder, [decoder = m_js8Decoder, submode, depth, rx, tx]() {
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
    if (decode.submode != currentJs8Submode()) return;
    const double tolerance = Js8Mode::parameters(decode.submode).bandwidthHz() / 2.0;
    const QString myCall = stationCallsign().trimmed().toUpper();

    // Band activity: one running text per offset.
    int index = -1;
    for (int i = 0; i < m_js8Activity.size(); ++i)
        if (std::fabs(m_js8Activity.at(i).offsetHz - decode.frequencyHz) <= tolerance) { index = i; break; }
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
    a.text += decode.text;
    a.current += decode.text;
    if (a.text.size() > kJs8MaxTextChars) a.text = a.text.right(kJs8MaxTextChars);
    if (!decode.from.isEmpty()) a.from = decode.from;
    a.complete = decode.last;
    while (m_js8Activity.size() > kJs8MaxActivityRows) m_js8Activity.removeFirst();

    // Heard stations.
    QString from = decode.from;
    if (from.isEmpty() && decode.directed) from = a.from;
    if (!from.isEmpty() && !from.startsWith(QLatin1Char('@')) && !from.contains(QStringLiteral("....")) && from != myCall) {
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
    const bool atRx = std::fabs(decode.frequencyHz - rxHz) <= tolerance;
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
        if (toMe) appendLog(QStringLiteral("JS8 to %1: %2").arg(myCall, a.current.trimmed()));
    }

    js8RefreshActivity();
    js8RefreshCalls();
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
        auto *text = new QTableWidgetItem(a.text.trimmed() + (a.complete ? QStringLiteral(" ✓") : QString()));
        text->setData(Qt::UserRole, a.from);
        text->setToolTip(a.text.trimmed());
        if (!myCall.isEmpty() && a.text.contains(myCall)) {
            for (auto *item : {offset, utc, snr, text}) item->setForeground(QColor(255, 120, 60));
        }
        m_tableJs8Band->setItem(r, 0, offset);
        m_tableJs8Band->setItem(r, 1, utc);
        m_tableJs8Band->setItem(r, 2, snr);
        m_tableJs8Band->setItem(r, 3, text);
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
    if (ui != nullptr && ui->cmbMode != nullptr && !Js8Mode::isMode(ui->cmbMode->currentText()))
        ui->cmbMode->setCurrentText(Js8Mode::familyName());
    const QString myCall = stationCallsign().trimmed().toUpper();
    if (myCall.isEmpty()) {
        m_lblJs8TxStatus->setText(uiText("js8_need_call", "Enter your callsign in the settings first."));
        return;
    }
    if (m_txRunning || m_nativeWeakSignalTxPending) {
        m_lblJs8TxStatus->setText(uiText("js8_tx_busy", "A transmission is already running or armed; press STOP to cancel it."));
        return;
    }
    const Js8Mode::Submode submode = currentJs8Submode();
    const auto frames = Js8Message::buildFrames(myCall, stationLocator(), to, text, submode, true);
    if (frames.isEmpty()) {
        m_lblJs8TxStatus->setText(uiText("js8_cannot_encode", "This text cannot be sent in JS8."));
        return;
    }
    m_js8PendingFrames = frames;
    m_js8PendingText = Js8Message::preview(frames, submode).trimmed();
    const Js8Mode::Parameters p = Js8Mode::parameters(submode);
    m_lblJs8TxStatus->setText(M3I18n::text(QStringLiteral("JS8 TX: %1 frame(s), %2 s: %3"))
                                  .arg(frames.size())
                                  .arg(frames.size() * p.periodSeconds)
                                  .arg(m_js8PendingText));
    js8AppendRx(QStringLiteral("%1  TX  %2 Hz  %3")
                    .arg(QDateTime::currentDateTimeUtc().time().toString(QStringLiteral("HH:mm:ss")))
                    .arg(m_spinJs8TxFreq != nullptr ? m_spinJs8TxFreq->value() : 1500)
                    .arg(m_js8PendingText),
                QColor(255, 80, 80));
    startImageTx();
}
