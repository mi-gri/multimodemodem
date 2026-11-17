// SPDX-License-Identifier: GPL-3.0-or-later
// RTTY signal browser (like the fldigi signal browser): one row per signal
// the RTTY skimmer decodes in parallel, under the RTTY receive text.  It only
// displays what RttyMultiDecoder already computes; double-click tunes the
// main RTTY decoder to that signal.
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dxcc/CtyCountryFile.h"
#include "logbook/AdifLogbook.h"
#include "modems/ft8/FtDecodeFilter.h"
#include "modems/rtty/RttyDecoder.h"
#include "settings/AppSettings.h"
#include "utils/CockpitTheme.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QHeaderView>
#include <QLabel>
#include <QPlainTextEdit>
#include <QSettings>
#include <QSpinBox>
#include <QSplitter>
#include <QTableWidget>
#include <QVBoxLayout>

#include <algorithm>

namespace {
enum BrowserColumn { ColHz, ColCall, ColCountry, ColText, ColCount };
constexpr qint64 kStaleRemoveMs = 60000;   // a vanished signal stays greyed this long
constexpr int kTrackRole = Qt::UserRole + 811;
}

void MainWindow::setupRttyBrowser()
{
    if (m_rttyDisplayPage == nullptr || m_txtRttyRx == nullptr || m_tableRttyBrowser != nullptr) return;
    auto *layout = qobject_cast<QVBoxLayout *>(m_rttyDisplayPage->layout());
    if (layout == nullptr) return;
    const int index = layout->indexOf(m_txtRttyRx);
    if (index < 0) return;

    auto *splitter = new QSplitter(Qt::Vertical, m_rttyDisplayPage);
    splitter->setChildrenCollapsible(false);
    layout->insertWidget(index, splitter, 1);
    layout->removeWidget(m_txtRttyRx);
    splitter->addWidget(m_txtRttyRx);

    auto *browser = new QWidget(splitter);
    auto *browserLayout = new QVBoxLayout(browser);
    browserLayout->setContentsMargins(0, 0, 0, 0);
    browserLayout->setSpacing(2);
    m_lblRttyBrowser = new QLabel(browser);
    m_lblRttyBrowser->setWordWrap(true);
    browserLayout->addWidget(m_lblRttyBrowser);

    m_tableRttyBrowser = new QTableWidget(0, ColCount, browser);
    m_tableRttyBrowser->setObjectName(QStringLiteral("rttySignalBrowser"));
    m_tableRttyBrowser->setHorizontalHeaderLabels({uiText("rtty_browser_hz", "Hz"), uiText("rtty_browser_call", "CQ"),
                                                   uiText("rtty_browser_country", "Country"), uiText("rtty_browser_text", "Text")});
    m_tableRttyBrowser->verticalHeader()->setVisible(false);
    m_tableRttyBrowser->verticalHeader()->setDefaultSectionSize(m_tableRttyBrowser->fontMetrics().height() + 6);
    m_tableRttyBrowser->horizontalHeader()->setSectionResizeMode(ColHz, QHeaderView::ResizeToContents);
    m_tableRttyBrowser->horizontalHeader()->setSectionResizeMode(ColCall, QHeaderView::ResizeToContents);
    m_tableRttyBrowser->horizontalHeader()->setSectionResizeMode(ColCountry, QHeaderView::ResizeToContents);
    m_tableRttyBrowser->horizontalHeader()->setSectionResizeMode(ColText, QHeaderView::Stretch);
    m_tableRttyBrowser->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableRttyBrowser->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableRttyBrowser->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableRttyBrowser->setWordWrap(false);
    // The newest characters are at the right end: elide on the left.
    m_tableRttyBrowser->setTextElideMode(Qt::ElideLeft);
    QFont mono = m_txtRttyRx->font();
    m_tableRttyBrowser->setFont(mono);
    m_tableRttyBrowser->setToolTip(uiText("rtty_browser_tooltip",
        "All RTTY signals the skimmer decodes in parallel, one row each. Double-click a row to tune the main decoder to it."));
    browserLayout->addWidget(m_tableRttyBrowser, 1);
    splitter->addWidget(browser);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 2);

    QSettings settings(AppSettings::settingsFilePath(), QSettings::IniFormat);
    const QByteArray state = settings.value(QStringLiteral("RTTY/browserSplitter")).toByteArray();
    if (!state.isEmpty()) splitter->restoreState(state);
    connect(splitter, &QSplitter::splitterMoved, this, [splitter] {
        QSettings s(AppSettings::settingsFilePath(), QSettings::IniFormat);
        s.setValue(QStringLiteral("RTTY/browserSplitter"), splitter->saveState());
    });

    connect(m_tableRttyBrowser, &QTableWidget::cellDoubleClicked, this, [this](int row, int) {
        const QTableWidgetItem *item = m_tableRttyBrowser->item(row, ColHz);
        if (item == nullptr) return;
        const auto it = m_rttyBrowserRows.constFind(item->data(kTrackRole).toULongLong());
        if (it == m_rttyBrowserRows.constEnd()) return;
        tuneRttyToSignal(it->markHz, it->spaceHz);
    });
    if (m_chkRttyMultiDecode != nullptr)
        connect(m_chkRttyMultiDecode, &QCheckBox::toggled, this, [this] { renderRttyBrowser(); });
    m_rttyBrowserTimer.setInterval(2000);   // ages and removes vanished signals
    connect(&m_rttyBrowserTimer, &QTimer::timeout, this, [this] { renderRttyBrowser(); });
    m_rttyBrowserTimer.start();
    renderRttyBrowser();
}

void MainWindow::tuneRttyToSignal(double markHz, double spaceHz)
{
    if (m_txRunning || m_spinRttyMarkHz == nullptr || m_spinRttyShiftHz == nullptr || m_cmbRttyPreset == nullptr) return;
    const int mark = qRound(qMin(markHz, spaceHz));
    {
        const QSignalBlocker blockPreset(m_cmbRttyPreset);
        const QSignalBlocker blockMark(m_spinRttyMarkHz);
        const int customIndex = m_cmbRttyPreset->findData(QStringLiteral("CUSTOM"));
        if (customIndex >= 0) m_cmbRttyPreset->setCurrentIndex(customIndex);
        m_spinRttyMarkHz->setValue(qBound(m_spinRttyMarkHz->minimum(), mark, m_spinRttyMarkHz->maximum()));
    }
    applyRttySettings();
    appendLog(uiText("rtty_browser_tuned", "RTTY: main decoder tuned to the signal at %1 Hz.")
                  .arg(qRound((markHz + spaceHz) * 0.5)));
}

void MainWindow::updateRttyBrowser(const QVector<SkimmerSpot> &spots)
{
    if (m_tableRttyBrowser == nullptr) return;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    for (const SkimmerSpot &spot : spots) {
        RttyBrowserRow &row = m_rttyBrowserRows[spot.id];
        row.frequencyHz = spot.frequencyHz;
        row.markHz = spot.lowerToneHz;
        row.spaceHz = spot.upperToneHz;
        row.text = spot.text.simplified();
        if (!spot.cqCall.isEmpty()) row.cqCall = spot.cqCall;
        row.lastSeenMs = now;
    }
    renderRttyBrowser();
}

void MainWindow::renderRttyBrowser()
{
    if (m_tableRttyBrowser == nullptr) return;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    for (auto it = m_rttyBrowserRows.begin(); it != m_rttyBrowserRows.end();) {
        if (now - it->lastSeenMs > kStaleRemoveMs) it = m_rttyBrowserRows.erase(it);
        else ++it;
    }

    const bool skimmerOn = m_chkRttyMultiDecode != nullptr && m_chkRttyMultiDecode->isChecked();

    QVector<RttyBrowserRow> rows;
    for (auto it = m_rttyBrowserRows.cbegin(); it != m_rttyBrowserRows.cend(); ++it) {
        RttyBrowserRow row = *it;
        row.id = it.key();
        rows.append(row);
    }
    std::sort(rows.begin(), rows.end(), [](const RttyBrowserRow &a, const RttyBrowserRow &b) { return a.frequencyHz < b.frequencyHz; });

    const bool hideBlocked = m_settings.ftDecodeFilterHideBlocked;
    const QStringList hiddenCountries = (hideBlocked && m_settings.ftDecodeFilterCountries) ? hiddenCountryPrimaryPrefixes() : QStringList();
    const QStringList hiddenContinents = hideBlocked ? FtDecodeFilter::continentList(m_settings.ftDecodeFilterContinentList) : QStringList();
    const QString band = m_lastRigFrequencyHz > 0.0 ? bandFromFrequencyHz(m_lastRigFrequencyHz).toLower() : QString();
    const QColor muted = M3Ui::themeColor(M3Ui::ThemeColorRole::MutedText);
    const QColor accent = M3Ui::themeColor(M3Ui::ThemeColorRole::Accent);
    QVariantList lookups;

    m_tableRttyBrowser->setUpdatesEnabled(false);
    m_tableRttyBrowser->setRowCount(rows.size());
    for (int r = 0; r < rows.size(); ++r) {
        const RttyBrowserRow &row = rows.at(r);
        const bool stale = now - row.lastSeenMs > 4000;
        QString country, callTip;
        QColor callBackground;
        bool hidden = false, worked = false;
        if (!row.cqCall.isEmpty()) {
            const QString call = AdifLogbook::normalizeCallsign(row.cqCall);
            const CtyCountryFile::LookupResult cty = CtyCountryFile::instance().lookupCallsign(call);
            if (cty.valid) {
                country = cty.entity.name;
                hidden = !isFtCallWatched(call) &&
                         (FtDecodeFilter::hiddenByCountry(cty.entity.primaryPrefix, hiddenCountries) ||
                          FtDecodeFilter::hiddenByContinent(cty.entity.continent, hiddenContinents));
            }
            // Logbook status from the shared (asynchronous) FT logbook cache.
            const QVariantMap query = makeFtLogbookQuery(call, band, QStringLiteral("RTTY"),
                                                         cty.valid ? cty.entity.dxcc : QString(), QString(), 0, 0);
            const QVariantMap status = cachedFtLogbookStatus(query);
            if (status.isEmpty()) {
                lookups.push_back(query);
            } else {
                worked = status.value(QStringLiteral("worked")).toBool();
                if (cty.valid && !status.value(QStringLiteral("countryWorkedAny")).toBool()) {
                    callBackground = QColor(m_settings.ftHighlightNewDxccBackground);
                    callTip = uiText("ft_new_dxcc_country_tooltip", "New DXCC country not present in logbook: %1").arg(country);
                } else if (cty.valid && !status.value(QStringLiteral("countryWorkedBand")).toBool()) {
                    callBackground = QColor(m_settings.ftHighlightNewDxccBandBackground);
                    callTip = uiText("ft_new_dxcc_band_tooltip", "New DXCC on this band: %1").arg(country);
                } else if (worked) {
                    callTip = uiText("rtty_browser_worked", "Already in the logbook.");
                }
            }
        }
        m_tableRttyBrowser->setRowHidden(r, hidden);

        const QStringList cells = {QString::number(qRound(row.frequencyHz)), row.cqCall, country, row.text};
        for (int c = 0; c < ColCount; ++c) {
            QTableWidgetItem *item = m_tableRttyBrowser->item(r, c);
            if (item == nullptr) {
                item = new QTableWidgetItem;
                m_tableRttyBrowser->setItem(r, c, item);
            }
            item->setText(cells.at(c));
            item->setData(kTrackRole, QVariant::fromValue<qulonglong>(row.id));
            item->setTextAlignment(c == ColHz ? (Qt::AlignRight | Qt::AlignVCenter) : (Qt::AlignLeft | Qt::AlignVCenter));
            QFont f = m_tableRttyBrowser->font();
            f.setBold(c == ColCall && !row.cqCall.isEmpty());
            f.setStrikeOut(c == ColCall && worked);
            item->setFont(f);
            item->setForeground(stale ? QBrush(muted) : (c == ColCall && !row.cqCall.isEmpty() && !callBackground.isValid() ? QBrush(accent) : QBrush()));
            item->setBackground(c == ColCall && callBackground.isValid() && !stale ? QBrush(callBackground) : QBrush());
            item->setToolTip(c == ColCall ? callTip
                             : c == ColText ? row.text
                             : uiText("rtty_browser_tones", "Mark %1 Hz · Space %2 Hz").arg(qRound(row.markHz)).arg(qRound(row.spaceHz)));
        }
    }
    m_tableRttyBrowser->setUpdatesEnabled(true);
    if (m_lblRttyBrowser != nullptr) {
        int shown = 0;
        for (int r = 0; r < m_tableRttyBrowser->rowCount(); ++r) if (!m_tableRttyBrowser->isRowHidden(r)) ++shown;
        m_lblRttyBrowser->setText(skimmerOn
            ? uiText("rtty_browser_title", "RTTY signals (%1) — double-click tunes the main decoder").arg(shown)
            : uiText("rtty_browser_off", "RTTY signals: switch on \"Skimmer\" in the RTTY settings to decode all signals in parallel."));
    }
    if (!lookups.isEmpty()) queueFtLogbookQueries(lookups);
}
