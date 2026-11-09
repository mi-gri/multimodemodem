// SPDX-License-Identifier: GPL-3.0-or-later
#include "FrequencyListEditor.h"

#include <QAbstractItemView>
#include <QBrush>
#include <QColor>
#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalBlocker>
#include <QTableWidget>
#include <QVBoxLayout>

#include <algorithm>
#include <cmath>
#include <functional>

namespace {
enum Column { ColMode, ColBand, ColFrequency, ColRegion, ColPreferred, ColDescription, ColCount };

QString mhzText(double hz) { return QString::number(hz / 1e6, 'f', 6); }
} // namespace

FrequencyListEditor::FrequencyListEditor(const QVector<M3StandardFrequencies::Entry> &entries, const QString &region,
                                         Translator translator, QWidget *parent)
    : QWidget(parent), m_translator(std::move(translator))
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    auto *intro = new QLabel(L(QStringLiteral("Working frequencies per mode and band, as in WSJT-X/JTDX. The band selectors and the band scheduler use the preferred frequency of a band; without one, the first entry. Frequencies are dial frequencies (USB) in MHz.")), this);
    intro->setWordWrap(true);
    layout->addWidget(intro);

    auto *top = new QHBoxLayout();
    top->addWidget(new QLabel(L(QStringLiteral("IARU region")), this));
    m_region = new QComboBox(this);
    m_region->addItem(L(QStringLiteral("All regions")), QStringLiteral("ALL"));
    m_region->addItem(L(QStringLiteral("Region 1 (Europe, Africa, Middle East)")), QStringLiteral("R1"));
    m_region->addItem(L(QStringLiteral("Region 2 (Americas)")), QStringLiteral("R2"));
    m_region->addItem(L(QStringLiteral("Region 3 (Asia, Pacific)")), QStringLiteral("R3"));
    m_region->setCurrentIndex(qMax(0, m_region->findData(region)));
    m_region->setToolTip(L(QStringLiteral("Entries for other regions are ignored.")));
    top->addWidget(m_region, 1);
    top->addSpacing(12);
    top->addWidget(new QLabel(L(QStringLiteral("Show")), this));
    m_filter = new QComboBox(this);
    m_filter->addItem(L(QStringLiteral("All modes")), QString());
    for (const QString &mode : M3StandardFrequencies::modes()) m_filter->addItem(mode, mode);
    top->addWidget(m_filter);
    layout->addLayout(top);

    m_table = new QTableWidget(0, ColCount, this);
    m_table->setHorizontalHeaderLabels({L(QStringLiteral("Mode")), L(QStringLiteral("Band")),
                                        L(QStringLiteral("Frequency (MHz)")), L(QStringLiteral("Region")),
                                        L(QStringLiteral("Preferred")), L(QStringLiteral("Description"))});
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setColumnWidth(ColMode, 90);
    m_table->setColumnWidth(ColBand, 60);
    m_table->setColumnWidth(ColFrequency, 120);
    m_table->setColumnWidth(ColRegion, 70);
    m_table->setColumnWidth(ColPreferred, 95);
    layout->addWidget(m_table, 1);

    auto *buttons = new QHBoxLayout();
    auto *add = new QPushButton(L(QStringLiteral("Add")), this);
    auto *remove = new QPushButton(L(QStringLiteral("Delete")), this);
    auto *restore = new QPushButton(L(QStringLiteral("Restore defaults")), this);
    restore->setToolTip(L(QStringLiteral("Replaces the list with the defaults of WSJT-X 3.0 (FT8, FT4, MSK144, Q65) and JS8Call 2.2.0 (JS8).")));
    buttons->addWidget(add);
    buttons->addWidget(remove);
    buttons->addStretch(1);
    buttons->addWidget(restore);
    layout->addLayout(buttons);
    m_summary = new QLabel(this);
    m_summary->setWordWrap(true);
    layout->addWidget(m_summary);

    load(entries);

    connect(m_filter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() { applyFilter(); });
    connect(m_table, &QTableWidget::itemChanged, this, [this](QTableWidgetItem *item) {
        if (m_loading || item == nullptr) return;
        if (item->column() == ColFrequency) updateRow(item->row());
        if (item->column() == ColPreferred && item->checkState() == Qt::Checked) unpreferOthers(item->row());
        updateSummary();
    });
    connect(add, &QPushButton::clicked, this, [this]() {
        M3StandardFrequencies::Entry e;
        e.mode = m_filter->currentData().toString().isEmpty() ? QStringLiteral("FT8") : m_filter->currentData().toString();
        e.hz = 14074000.0;
        e.region = QStringLiteral("ALL");
        appendRow(e);
        const int row = m_table->rowCount() - 1;
        m_table->scrollToItem(m_table->item(row, ColFrequency));
        m_table->setCurrentCell(row, ColFrequency);
        m_table->editItem(m_table->item(row, ColFrequency));
        updateSummary();
    });
    connect(remove, &QPushButton::clicked, this, [this]() {
        QList<int> rows;
        for (const QModelIndex &index : m_table->selectionModel()->selectedRows()) rows << index.row();
        std::sort(rows.begin(), rows.end(), std::greater<int>());
        for (int row : rows) m_table->removeRow(row);
        updateSummary();
    });
    connect(restore, &QPushButton::clicked, this, [this]() {
        if (QMessageBox::question(this, L(QStringLiteral("Restore defaults")),
                                  L(QStringLiteral("Replace the frequency list with the defaults? Your own entries are lost.")))
            != QMessageBox::Yes)
            return;
        load(M3StandardFrequencies::defaults());
    });
}

QString FrequencyListEditor::L(const QString &source) const { return m_translator ? m_translator(source) : source; }

void FrequencyListEditor::load(const QVector<M3StandardFrequencies::Entry> &entries)
{
    m_loading = true;
    m_table->setRowCount(0);
    QVector<M3StandardFrequencies::Entry> sorted = entries;
    std::stable_sort(sorted.begin(), sorted.end(), [](const M3StandardFrequencies::Entry &a, const M3StandardFrequencies::Entry &b) {
        return a.hz < b.hz;
    });
    for (const auto &e : sorted) appendRow(e);
    m_loading = false;
    applyFilter();
    updateSummary();
}

void FrequencyListEditor::appendRow(const M3StandardFrequencies::Entry &entry)
{
    const bool wasLoading = m_loading;
    m_loading = true;
    const int row = m_table->rowCount();
    m_table->insertRow(row);

    auto *mode = new QComboBox(m_table);
    mode->addItems(M3StandardFrequencies::modes());
    if (mode->findText(entry.mode) < 0) mode->addItem(entry.mode);
    mode->setCurrentText(entry.mode);
    m_table->setCellWidget(row, ColMode, mode);
    connect(mode, &QComboBox::currentTextChanged, this, [this, mode]() {
        for (int r = 0; r < m_table->rowCount(); ++r)
            if (m_table->cellWidget(r, ColMode) == mode) { unpreferOthers(r); break; }
        applyFilter();
        updateSummary();
    });

    auto *band = new QTableWidgetItem();
    band->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    m_table->setItem(row, ColBand, band);
    m_table->setItem(row, ColFrequency, new QTableWidgetItem(mhzText(entry.hz)));

    auto *region = new QComboBox(m_table);
    region->addItems(M3StandardFrequencies::regions());
    region->setCurrentText(entry.region.isEmpty() ? QStringLiteral("ALL") : entry.region);
    m_table->setCellWidget(row, ColRegion, region);
    connect(region, &QComboBox::currentTextChanged, this, [this]() { updateSummary(); });

    auto *preferred = new QTableWidgetItem();
    preferred->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    preferred->setCheckState(entry.preferred ? Qt::Checked : Qt::Unchecked);
    m_table->setItem(row, ColPreferred, preferred);
    m_table->setItem(row, ColDescription, new QTableWidgetItem(entry.description));
    updateRow(row);
    m_loading = wasLoading;
}

QString FrequencyListEditor::rowMode(int row) const
{
    auto *combo = qobject_cast<QComboBox *>(m_table->cellWidget(row, ColMode));
    return combo != nullptr ? combo->currentText().trimmed().toUpper() : QString();
}

QString FrequencyListEditor::rowRegion(int row) const
{
    auto *combo = qobject_cast<QComboBox *>(m_table->cellWidget(row, ColRegion));
    return combo != nullptr ? combo->currentText() : QStringLiteral("ALL");
}

double FrequencyListEditor::rowHz(int row, bool *ok) const
{
    auto *item = m_table->item(row, ColFrequency);
    QString text = item != nullptr ? item->text().trimmed() : QString();
    text.replace(QLatin1Char(','), QLatin1Char('.'));
    bool number = false;
    const double mhz = text.toDouble(&number);
    const double hz = std::round(mhz * 1e6);
    const bool valid = number && hz >= 100000.0 && hz <= 11.0e9;
    if (ok != nullptr) *ok = valid;
    return valid ? hz : 0.0;
}

void FrequencyListEditor::updateRow(int row)
{
    bool ok = false;
    const double hz = rowHz(row, &ok);
    const QString band = ok ? M3StandardFrequencies::bandOf(hz) : QString();
    const bool wasLoading = m_loading;
    m_loading = true;
    if (auto *item = m_table->item(row, ColBand)) item->setText(ok ? (band.isEmpty() ? QStringLiteral("–") : band) : QString());
    if (auto *item = m_table->item(row, ColFrequency)) {
        item->setForeground(ok ? QBrush() : QBrush(QColor(220, 60, 60)));
        item->setToolTip(ok ? QString() : L(QStringLiteral("Not a valid frequency in MHz; this row is not saved.")));
    }
    m_loading = wasLoading;
}

void FrequencyListEditor::unpreferOthers(int row)
{
    auto *item = m_table->item(row, ColPreferred);
    if (item == nullptr || item->checkState() != Qt::Checked) return;
    const QString mode = rowMode(row);
    const QString band = M3StandardFrequencies::bandOf(rowHz(row));
    const QString region = rowRegion(row);
    const bool wasLoading = m_loading;
    m_loading = true;
    for (int r = 0; r < m_table->rowCount(); ++r) {
        if (r == row || rowMode(r) != mode || M3StandardFrequencies::bandOf(rowHz(r)) != band || rowRegion(r) != region) continue;
        if (auto *other = m_table->item(r, ColPreferred)) other->setCheckState(Qt::Unchecked);
    }
    m_loading = wasLoading;
}

void FrequencyListEditor::applyFilter()
{
    const QString mode = m_filter->currentData().toString();
    for (int r = 0; r < m_table->rowCount(); ++r) m_table->setRowHidden(r, !mode.isEmpty() && rowMode(r) != mode);
}

QVector<M3StandardFrequencies::Entry> FrequencyListEditor::entries() const
{
    QVector<M3StandardFrequencies::Entry> out;
    for (int r = 0; r < m_table->rowCount(); ++r) {
        bool ok = false;
        M3StandardFrequencies::Entry e;
        e.hz = rowHz(r, &ok);
        if (!ok) continue;
        e.mode = rowMode(r);
        e.region = rowRegion(r);
        auto *preferred = m_table->item(r, ColPreferred);
        e.preferred = preferred != nullptr && preferred->checkState() == Qt::Checked;
        auto *description = m_table->item(r, ColDescription);
        e.description = description != nullptr ? description->text().trimmed() : QString();
        if (!e.mode.isEmpty()) out << e;
    }
    return out;
}

QString FrequencyListEditor::region() const { return m_region->currentData().toString(); }

int FrequencyListEditor::invalidRows() const
{
    int n = 0;
    for (int r = 0; r < m_table->rowCount(); ++r) {
        bool ok = false;
        rowHz(r, &ok);
        if (!ok) ++n;
    }
    return n;
}

void FrequencyListEditor::updateSummary()
{
    const int invalid = invalidRows();
    QString text = L(QStringLiteral("%1 entries")).arg(m_table->rowCount());
    if (invalid > 0) text += QStringLiteral(" — ") + L(QStringLiteral("%1 invalid (not saved)")).arg(invalid);
    m_summary->setText(text);
}
