// SPDX-License-Identifier: GPL-3.0-or-later
// Working frequency in the main window: one Frequency area above the side
// tabs with the CAT readout and, per mode, band (or "outside amateur bands"),
// the stored frequencies of that band, a free frequency entry and a status
// line.  FT and JS8 bring their own band selector.  Also the QSY on a mode
// change.
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "modems/ft8/Ft8Mode.h"
#include "modems/js8/Js8Mode.h"
#include "settings/AppSettings.h"
#include "utils/CockpitTheme.h"
#include "utils/StandardFrequencies.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QSignalBlocker>
#include <QStackedWidget>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include <cmath>

namespace {
// A stored frequency counts as "the rig is on it" within this distance.
constexpr double kOnEntryToleranceHz = 500.0;
}

QString MainWindow::frequencyGroupLabel(const QString &group) const
{
    return group == M3StandardFrequencies::otherGroup()
        ? uiText("frequency_group_other", "Outside amateur bands")
        : group;
}

QWidget *MainWindow::frequencyTabPage(const QString &pageKey)
{
    if (QWidget *page = m_frequencyPages.value(pageKey, nullptr)) return page;
    if (m_stkFrequency == nullptr) {
        // One Frequency area above the side tabs, for every mode: the CAT
        // readout (placed by updateRigReadout) and the band/stored/entry page
        // of the current mode.  It stays visible whichever tab is open.
        QWidget *side = ui->sideTabWidget->parentWidget();
        auto *sideLayout = side != nullptr ? qobject_cast<QVBoxLayout *>(side->layout()) : nullptr;
        m_grpFrequencyHeader = new QGroupBox(uiText("frequency", "Frequency"), side);
        m_grpFrequencyHeader->setObjectName(QStringLiteral("frequencyHeader"));
        m_grpFrequencyHeader->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        auto *layout = new QVBoxLayout(m_grpFrequencyHeader);
        layout->setContentsMargins(8, 6, 8, 6);
        layout->setSpacing(4);
        m_frequencyReadoutSlot = new QWidget(m_grpFrequencyHeader);
        auto *slotLayout = new QVBoxLayout(m_frequencyReadoutSlot);
        slotLayout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(m_frequencyReadoutSlot);
        m_stkFrequency = new QStackedWidget(m_grpFrequencyHeader);
        m_stkFrequency->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        layout->addWidget(m_stkFrequency);
        if (sideLayout != nullptr) sideLayout->insertWidget(0, m_grpFrequencyHeader);
        else ui->modeTabLayout->insertWidget(0, m_grpFrequencyHeader);
        // The stack takes the height of the current page, not of the largest.
        connect(m_stkFrequency, &QStackedWidget::currentChanged, this, [this](int) {
            for (int i = 0; i < m_stkFrequency->count(); ++i) {
                QWidget *w = m_stkFrequency->widget(i);
                w->setSizePolicy(QSizePolicy::Preferred, w == m_stkFrequency->currentWidget() ? QSizePolicy::Preferred : QSizePolicy::Ignored);
            }
            m_stkFrequency->adjustSize();
        });
    }
    auto *page = new QWidget(m_stkFrequency);
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);
    page->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    m_stkFrequency->addWidget(page);
    m_frequencyPages.insert(pageKey, page);
    return page;
}

void MainWindow::installModeFrequencyGroup(const QString &listMode)
{
    QWidget *page = frequencyTabPage(listMode);
    static_cast<QVBoxLayout *>(page->layout())->insertWidget(0, createModeBandGroup(page, listMode));
}

void MainWindow::updateSideTabsForMode(const QString &modeName)
{
    if (ui == nullptr || ui->sideTabWidget == nullptr) return;
    QTabWidget *tabs = ui->sideTabWidget;
    const int modeIndex = tabs->indexOf(ui->tabModeSettings);
    const auto showTab = [tabs, modeIndex](QWidget *tab, bool show) {
        const int i = tab != nullptr ? tabs->indexOf(tab) : -1;
        if (i < 0) return;
        tabs->tabBar()->setTabVisible(i, show);
        if (!show && tabs->currentIndex() == i) tabs->setCurrentIndex(modeIndex >= 0 ? modeIndex : 0);
    };

    const QString listMode = M3StandardFrequencies::listModeForName(modeName);
    const QString key = Ft8Mode::isFamilyMode(modeName) ? QStringLiteral("FT")
                      : Js8Mode::isMode(modeName)       ? QStringLiteral("JS8")
                                                        : listMode;
    QWidget *page = m_frequencyPages.value(key, nullptr);
    if (page != nullptr && m_stkFrequency != nullptr) m_stkFrequency->setCurrentWidget(page);
    if (m_stkFrequency != nullptr) m_stkFrequency->setVisible(page != nullptr);
    updateFt8TxPeriodButton();   // slot times differ between FT8 and FT4
    // Weather fax and the radio telescope (no list mode) have no QSO partner.
    showTab(m_tabQrz, !listMode.isEmpty() && listMode != QStringLiteral("WEFAX"));
}

QWidget *MainWindow::createModeBandGroup(QWidget *parent, const QString &listMode)
{
    // Untitled: the Frequency area above the tabs is the group.
    auto *group = new QWidget(parent);
    auto *grid = new QGridLayout(group);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setVerticalSpacing(4);
    int row = 0;
    ModeBandWidgets w;
    // Weather fax stations are no amateur radio: no band selector, the
    // stored list shows all its stations.
    if (!listMode.isEmpty() && !M3StandardFrequencies::isServiceMode(listMode)) {
        w.band = new QComboBox(group);
        w.band->setToolTip(uiText("mode_band_tooltip", "Select a band to tune the CAT-connected rig to the preferred frequency of this mode (Settings → Frequencies)."));
        for (const QString &g : M3StandardFrequencies::groups(listMode)) w.band->addItem(frequencyGroupLabel(g), g);
        const QString key = listMode + QStringLiteral("/band");
        const QString saved = QSettings(AppSettings::settingsFilePath(), QSettings::IniFormat).value(key).toString();
        const int idx = w.band->findData(saved);
        if (idx >= 0) w.band->setCurrentIndex(idx);
        grid->addWidget(new QLabel(uiText("band", "Band"), group), row, 0);
        grid->addWidget(w.band, row++, 1);
        connect(w.band, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, listMode, key]() {
            const ModeBandWidgets mw = m_modeBandWidgets.value(listMode);
            if (mw.band == nullptr) return;
            QSettings(AppSettings::settingsFilePath(), QSettings::IniFormat).setValue(key, mw.band->currentData().toString());
            refreshFrequencyPickers();
            qsyModeBand(listMode);
            updateModeBandUis();
        });
    }
    QComboBox *band = w.band;
    row = addFrequencyPickerRows(grid, row, group, [listMode]() { return listMode; },
                                 [band]() { return band != nullptr ? band->currentData().toString() : QString(); });
    if (!listMode.isEmpty()) {
        w.status = new QLabel(group);
        w.status->setWordWrap(true);
        grid->addWidget(w.status, row++, 0, 1, 2);
        m_modeBandWidgets.insert(listMode, w);
    }
    grid->setColumnStretch(1, 1);
    updateModeBandUis();
    return group;
}

int MainWindow::addFrequencyPickerRows(QGridLayout *grid, int row, QWidget *parent, std::function<QString()> listMode,
                                       std::function<QString()> band)
{
    FrequencyPicker picker;
    picker.listMode = std::move(listMode);
    picker.band = std::move(band);
    if (!picker.listMode().isEmpty()) {
        picker.stored = new QComboBox(parent);
        picker.stored->setToolTip(uiText("stored_frequencies_tooltip", "Frequencies of this mode and band from Settings → Frequencies. Selecting one tunes the CAT-connected rig."));
        picker.stored->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
        picker.stored->setMinimumContentsLength(14);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        picker.stored->setPlaceholderText(uiText("stored_frequencies_none_selected", "(rig not on a stored frequency)"));
#endif
        grid->addWidget(new QLabel(uiText("stored_frequencies", "Stored"), parent), row, 0);
        grid->addWidget(picker.stored, row++, 1);
        QComboBox *stored = picker.stored;
        const auto mode = picker.listMode;
        connect(stored, QOverload<int>::of(&QComboBox::activated), this, [this, stored, mode](int index) {
            const double hz = stored->itemData(index).toDouble();
            if (hz > 0.0) tuneRigTo(hz, mode() + QLatin1Char(' ') + stored->itemText(index));
        });
    }
    picker.entry = new QLineEdit(parent);
    picker.entry->setPlaceholderText(uiText("frequency_entry_placeholder", "e.g. 14.080 MHz or 4583 kHz, Enter tunes"));
    picker.entry->setToolTip(uiText("frequency_entry_tooltip", "Dial frequency to tune the CAT-connected rig to, also outside the amateur bands (receive). Without a unit, values below 1000 are MHz, larger ones kHz."));
    picker.entry->setClearButtonEnabled(true);
    grid->addWidget(new QLabel(uiText("frequency_entry", "Enter"), parent), row, 0);
    grid->addWidget(picker.entry, row++, 1);
    QLineEdit *entry = picker.entry;
    connect(entry, &QLineEdit::returnPressed, this, [this, entry]() {
        const double hz = M3StandardFrequencies::parseFrequency(entry->text());
        if (hz <= 0.0) {
            appendLog(QStringLiteral("Invalid frequency: %1").arg(entry->text()));
            M3Ui::setSemanticRole(entry, QStringLiteral("negative"));
            return;
        }
        M3Ui::setSemanticRole(entry, QString());
        if (tuneRigTo(hz, QStringLiteral("entered"))) {
            m_rigTuneTargetHz = hz;
            m_rigTuneHoldUntilMs = QDateTime::currentMSecsSinceEpoch() + 1500;
            updateRigReadout();
            entry->clear();
        }
    });
    m_frequencyPickers.append(picker);
    refreshFrequencyPickers();
    return row;
}

void MainWindow::refreshFrequencyPickers()
{
    for (FrequencyPicker &picker : m_frequencyPickers) {
        if (picker.stored == nullptr) continue;
        const QString mode = picker.listMode();
        const QString band = picker.band();
        const QString signature = mode + QLatin1Char('|') + band + QLatin1Char('|') + QString::number(m_frequencyListVersion);
        if (signature != picker.signature) {
            picker.signature = signature;
            const QSignalBlocker block(picker.stored);
            picker.stored->clear();
            for (const M3StandardFrequencies::Entry &e : M3StandardFrequencies::entriesFor(mode, band)) {
                QString text = formatRigFrequency(e.hz);
                if (!e.description.trimmed().isEmpty()) text += QStringLiteral("  ") + e.description.trimmed();
                picker.stored->addItem(text, e.hz);
            }
        }
        int match = -1;
        double best = kOnEntryToleranceHz;
        for (int i = 0; m_lastRigFrequencyHz > 0.0 && i < picker.stored->count(); ++i) {
            const double distance = std::fabs(picker.stored->itemData(i).toDouble() - m_lastRigFrequencyHz);
            if (distance <= best) { best = distance; match = i; }
        }
        if (picker.stored->currentIndex() != match) {
            const QSignalBlocker block(picker.stored);
            picker.stored->setCurrentIndex(match);
        }
        picker.stored->setEnabled(picker.stored->count() > 0);
    }
}

void MainWindow::refreshModeBandLists()
{
    ++m_frequencyListVersion;
    for (auto it = m_modeBandWidgets.begin(); it != m_modeBandWidgets.end(); ++it) {
        QComboBox *combo = it->band;
        if (combo == nullptr) continue;
        const QString current = combo->currentData().toString();
        const QSignalBlocker block(combo);
        combo->clear();
        for (const QString &g : M3StandardFrequencies::groups(it.key())) combo->addItem(frequencyGroupLabel(g), g);
        const int idx = combo->findData(current);
        combo->setCurrentIndex(idx >= 0 ? idx : 0);
    }
    refreshFrequencyPickers();
    updateModeBandUis();
}

void MainWindow::updateModeBandUis()
{
    const bool haveCat = m_lastRigFrequencyHz > 0.0;
    for (auto it = m_modeBandWidgets.cbegin(); it != m_modeBandWidgets.cend(); ++it) {
        if (it->status == nullptr) continue;
        const bool service = it->band == nullptr;   // weather fax: no bands
        const double standard = service ? M3StandardFrequencies::preferredAnyBandHz(it.key())
                                        : M3StandardFrequencies::dialHz(it.key(), it->band->currentData().toString());
        const bool onSlot = haveCat && (M3StandardFrequencies::nearEntry(it.key(), m_lastRigFrequencyHz, kOnEntryToleranceHz) ||
                                        (standard > 0.0 && std::fabs(m_lastRigFrequencyHz - standard) <= 1000.0));
        QString text = uiText("mode_standard_frequency", "Frequency from the list (%1)").arg(it.key()) + QStringLiteral(": ") +
                       (standard > 0.0 ? formatRigFrequency(standard) : QStringLiteral("--"));
        if ((service ? M3StandardFrequencies::entriesFor(it.key(), QString()).isEmpty() : it->band->count() == 0)) text = uiText("mode_no_frequencies", "No frequencies for this mode in Settings → Frequencies.");
        else if (haveCat && !onSlot) text += QStringLiteral(" — ") + uiText("mode_off_standard", "rig is elsewhere");
        it->status->setText(text);
        M3Ui::setSemanticRole(it->status, onSlot || !haveCat ? QStringLiteral("positive") : QStringLiteral("negative"));
    }
}

void MainWindow::followCatForModeBands(double frequencyHz)
{
    if (frequencyHz > 0.0) {
        const QString group = M3StandardFrequencies::groupOf(frequencyHz);
        for (auto it = m_modeBandWidgets.begin(); it != m_modeBandWidgets.end(); ++it) {
            if (it->band == nullptr) continue;
            const int idx = it->band->findData(group);
            if (idx >= 0 && idx != it->band->currentIndex()) {
                const QSignalBlocker block(it->band);
                it->band->setCurrentIndex(idx);
            }
        }
    }
    refreshFrequencyPickers();
    updateModeBandUis();
}

void MainWindow::qsyModeBand(const QString &listMode)
{
    const ModeBandWidgets w = m_modeBandWidgets.value(listMode);
    if (w.band == nullptr || w.band->currentData().toString().isEmpty()) return;
    const QString group = w.band->currentData().toString();
    const double target = M3StandardFrequencies::dialHz(listMode, group);
    if (target <= 0.0 || !m_settings.hamlibCatEnabled) return;
    if (m_lastRigFrequencyHz > 0.0 && std::fabs(m_lastRigFrequencyHz - target) < 1.0) return;
    tuneRigTo(target, listMode + QLatin1Char(' ') + frequencyGroupLabel(group));
}

void MainWindow::qsyForModeChange(const QString &modeName)
{
    refreshFrequencyPickers();   // the FT page lists FT8 or FT4 entries
    // FT and JS8 tune in their own branches of handleModeChanged().
    if (!m_settings.frequencyQsyOnModeChange || !m_settings.hamlibCatEnabled) return;
    if (Ft8Mode::isFamilyMode(modeName) || Js8Mode::isMode(modeName)) return;
    const QString listMode = M3StandardFrequencies::listModeForName(modeName);
    if (listMode.isEmpty()) return;
    const double rigHz = m_lastRigFrequencyHz;

    // Already on a stored frequency of the new mode (a weather fax or RTTY
    // station, an activity frequency): stay.
    const double tolerance = listMode == QStringLiteral("WEFAX") ? 5000.0 : kOnEntryToleranceHz;
    if (rigHz > 0.0 && M3StandardFrequencies::nearEntry(listMode, rigHz, tolerance)) {
        followCatForModeBands(rigHz);
        return;
    }

    // Weather fax stations are outside the amateur bands: go to the preferred one.
    if (listMode == QStringLiteral("WEFAX")) {
        const double target = M3StandardFrequencies::preferredAnyBandHz(listMode);
        if (target > 0.0) tuneRigTo(target, modeName);
        return;
    }

    // Outside the amateur bands (receiving a utility station): never move the rig.
    if (rigHz > 0.0 && M3StandardFrequencies::bandOf(rigHz).isEmpty()) return;

    // Modes with a band selector: take the rig's band, then tune.
    if (m_modeBandWidgets.contains(listMode)) {
        followCatForModeBands(rigHz);
        qsyModeBand(listMode);
        return;
    }

    // Other modes: the preferred frequency of the band the rig is on.
    if (rigHz <= 0.0) return;
    const double target = M3StandardFrequencies::dialHz(listMode, M3StandardFrequencies::bandOf(rigHz));
    if (target > 0.0 && std::fabs(rigHz - target) >= 1.0) tuneRigTo(target, modeName);
}
