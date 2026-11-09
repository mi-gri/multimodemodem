// SPDX-License-Identifier: GPL-3.0-or-later
// Working-frequency list in the main window: band selectors of the
// weak-signal pages without their own band logic (MSK144, Q65) and the QSY
// on a mode change for all modes.  FT and JS8 keep their own band selectors.
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
#include <QSettings>
#include <QSignalBlocker>

#include <cmath>

QWidget *MainWindow::createModeBandGroup(QWidget *parent, const QString &listMode)
{
    auto *group = new QGroupBox(uiText("frequency", "Frequency"), parent);
    auto *grid = new QGridLayout(group);
    grid->setContentsMargins(8, 8, 8, 8);
    ModeBandWidgets w;
    w.band = new QComboBox(group);
    w.band->setToolTip(uiText("mode_band_tooltip", "Select a band to tune the CAT-connected rig to the preferred frequency of this mode (Settings → Frequencies)."));
    w.status = new QLabel(group);
    w.status->setWordWrap(true);
    grid->addWidget(new QLabel(uiText("band", "Band"), group), 0, 0);
    grid->addWidget(w.band, 0, 1);
    grid->addWidget(w.status, 1, 0, 1, 2);
    grid->setColumnStretch(1, 1);
    m_modeBandWidgets.insert(listMode, w);

    const QString key = listMode + QStringLiteral("/band");
    w.band->addItems(M3StandardFrequencies::bands(listMode));
    const QString saved = QSettings(AppSettings::settingsFilePath(), QSettings::IniFormat).value(key).toString();
    const int idx = w.band->findText(saved);
    if (idx >= 0) w.band->setCurrentIndex(idx);
    connect(w.band, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, listMode, key]() {
        const ModeBandWidgets mw = m_modeBandWidgets.value(listMode);
        if (mw.band == nullptr) return;
        QSettings(AppSettings::settingsFilePath(), QSettings::IniFormat).setValue(key, mw.band->currentText());
        qsyModeBand(listMode);
        updateModeBandUis();
    });
    updateModeBandUis();
    return group;
}

void MainWindow::refreshModeBandLists()
{
    for (auto it = m_modeBandWidgets.begin(); it != m_modeBandWidgets.end(); ++it) {
        QComboBox *combo = it->band;
        if (combo == nullptr) continue;
        const QString current = combo->currentText();
        const QSignalBlocker block(combo);
        combo->clear();
        combo->addItems(M3StandardFrequencies::bands(it.key()));
        const int idx = combo->findText(current);
        combo->setCurrentIndex(idx >= 0 ? idx : 0);
    }
    updateModeBandUis();
}

void MainWindow::updateModeBandUis()
{
    const bool haveCat = m_lastRigFrequencyHz > 0.0;
    for (auto it = m_modeBandWidgets.cbegin(); it != m_modeBandWidgets.cend(); ++it) {
        if (it->band == nullptr || it->status == nullptr) continue;
        const double standard = M3StandardFrequencies::dialHz(it.key(), it->band->currentText());
        const bool onSlot = haveCat && standard > 0.0 && std::fabs(m_lastRigFrequencyHz - standard) <= 1000.0;
        QString text = uiText("mode_standard_frequency", "Frequency from the list (%1)").arg(it.key()) + QStringLiteral(": ") +
                       (standard > 0.0 ? formatRigFrequency(standard) : QStringLiteral("--"));
        if (it->band->count() == 0) text = uiText("mode_no_frequencies", "No frequencies for this mode in Settings → Frequencies.");
        else if (haveCat && !onSlot) text += QStringLiteral(" — ") + uiText("mode_off_standard", "rig is elsewhere");
        it->status->setText(text);
        M3Ui::setSemanticRole(it->status, onSlot || !haveCat ? QStringLiteral("positive") : QStringLiteral("negative"));
    }
}

void MainWindow::followCatForModeBands(double frequencyHz)
{
    const QString band = M3StandardFrequencies::bandOf(frequencyHz);
    for (auto it = m_modeBandWidgets.begin(); it != m_modeBandWidgets.end(); ++it) {
        if (it->band == nullptr || band.isEmpty()) continue;
        const int idx = it->band->findText(band);
        if (idx >= 0 && idx != it->band->currentIndex()) {
            const QSignalBlocker block(it->band);
            it->band->setCurrentIndex(idx);
        }
    }
    updateModeBandUis();
}

void MainWindow::qsyModeBand(const QString &listMode)
{
    const ModeBandWidgets w = m_modeBandWidgets.value(listMode);
    if (w.band == nullptr || w.band->currentText().isEmpty()) return;
    const double target = M3StandardFrequencies::dialHz(listMode, w.band->currentText());
    if (target <= 0.0 || !m_settings.hamlibCatEnabled) return;
    if (m_lastRigFrequencyHz > 0.0 && std::fabs(m_lastRigFrequencyHz - target) < 1.0) return;
    tuneRigTo(target, listMode + QLatin1Char(' ') + w.band->currentText());
}

void MainWindow::qsyForModeChange(const QString &modeName)
{
    // FT and JS8 tune in their own branches of handleModeChanged().
    if (!m_settings.frequencyQsyOnModeChange || !m_settings.hamlibCatEnabled) return;
    if (Ft8Mode::isFamilyMode(modeName) || Js8Mode::isMode(modeName)) return;
    const QString listMode = M3StandardFrequencies::listModeForName(modeName);
    if (listMode.isEmpty()) return;

    // Weather fax stations are outside the amateur bands: stay on any listed
    // station, otherwise go to the preferred one.
    if (listMode == QStringLiteral("WEFAX")) {
        if (m_lastRigFrequencyHz > 0.0 && M3StandardFrequencies::nearEntry(listMode, m_lastRigFrequencyHz, 5000.0)) return;
        const double target = M3StandardFrequencies::preferredAnyBandHz(listMode);
        if (target > 0.0) tuneRigTo(target, modeName);
        return;
    }

    // Modes with a band selector: take the rig's band, then tune.
    if (m_modeBandWidgets.contains(listMode)) {
        followCatForModeBands(m_lastRigFrequencyHz);
        qsyModeBand(listMode);
        return;
    }

    // Other modes: the preferred frequency of the band the rig is on.
    if (m_lastRigFrequencyHz <= 0.0) return;
    const double target = M3StandardFrequencies::dialHz(listMode, M3StandardFrequencies::bandOf(m_lastRigFrequencyHz));
    if (target > 0.0 && std::fabs(m_lastRigFrequencyHz - target) >= 1.0) tuneRigTo(target, modeName);
}
