// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef FREQUENCYLISTEDITOR_H
#define FREQUENCYLISTEDITOR_H

#include "../utils/StandardFrequencies.h"

#include <QWidget>
#include <functional>

class QCheckBox;
class QComboBox;
class QLabel;
class QTableWidget;

/**
 * @brief Settings page "Frequencies": the editable working-frequency list.
 *
 * One row per entry (mode, band, dial frequency in MHz, IARU region,
 * preferred flag, description), as the Frequencies tab of WSJT-X/JTDX.
 * "Restore defaults" reloads the default list; "Add missing defaults"
 * adds the default entries the list lacks and keeps the operator's own.
 */
class FrequencyListEditor final : public QWidget
{
    Q_OBJECT

public:
    using Translator = std::function<QString(const QString &)>;

    FrequencyListEditor(const QVector<M3StandardFrequencies::Entry> &entries, const QString &region,
                        bool qsyOnModeChange, Translator translator, QWidget *parent = nullptr);

    /// Valid rows in table order; rows with an unreadable frequency are left out.
    QVector<M3StandardFrequencies::Entry> entries() const;
    QString region() const;
    bool qsyOnModeChange() const;
    int invalidRows() const;

private:
    QString L(const QString &source) const;
    void load(const QVector<M3StandardFrequencies::Entry> &entries);
    void appendRow(const M3StandardFrequencies::Entry &entry);
    void updateRow(int row);
    void applyFilter();
    void unpreferOthers(int row);
    QString rowMode(int row) const;
    QString rowRegion(int row) const;
    double rowHz(int row, bool *ok = nullptr) const;
    void updateSummary();

    Translator m_translator;
    QComboBox *m_region = nullptr;
    QComboBox *m_filter = nullptr;
    QCheckBox *m_qsyOnModeChange = nullptr;
    QTableWidget *m_table = nullptr;
    QLabel *m_summary = nullptr;
    bool m_loading = false;
};

#endif // FREQUENCYLISTEDITOR_H
