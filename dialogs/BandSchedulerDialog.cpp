#include "BandSchedulerDialog.h"

#include <QAbstractItemView>
#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSaveFile>
#include <QSignalBlocker>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimeEdit>
#include <QVBoxLayout>

#include "../utils/StandardFrequencies.h"

#include <algorithm>

namespace {
constexpr int SourceTextRole = Qt::UserRole + 923;

QString normalizedMode(const QString &mode)
{
    const QString m = mode.trimmed();
    if (m.compare(QStringLiteral("FT4"), Qt::CaseInsensitive) == 0) return QStringLiteral("FT4");
    if (m.compare(QStringLiteral("FT8"), Qt::CaseInsensitive) == 0) return QStringLiteral("FT8");
    if (m.compare(QStringLiteral("JS8"), Qt::CaseInsensitive) == 0) return QStringLiteral("JS8");
    if (m.compare(QStringLiteral("WSPR"), Qt::CaseInsensitive) == 0) return QStringLiteral("WSPR");
    return m;
}
}

QString ScheduledQsyEntry::triggerKey(const QDate &utcDate) const
{
    return QStringLiteral("%1|%2|%3|%4|%5")
        .arg(utcDate.toString(Qt::ISODate),
             timeUtc.toString(QStringLiteral("HH:mm")),
             mode.trimmed().toUpper(),
             band.trimmed().toUpper(),
             QString::number(qRound64(frequencyHz)));
}

BandSchedulerDialog::BandSchedulerDialog(const QList<ScheduledQsyEntry> &entries,
                                         Translator translator,
                                         QWidget *parent)
    : QDialog(parent),
      m_tr(std::move(translator)),
      m_entries(entries)
{
    buildUi();
    refreshTable();
    if (!m_entries.isEmpty()) {
        m_table->selectRow(0);
        loadRowToEditor(0);
    } else {
        ScheduledQsyEntry entry;
        entry.mode = QStringLiteral("FT8");
        entry.band = QStringLiteral("20m");
        entry.useFtStandard = true;
        entry.frequencyHz = standardFtFrequencyHz(entry.mode, entry.band);
        setEditorEntry(entry);
    }
}

QString BandSchedulerDialog::L(const QString &key, const QString &fallback) const
{
    return m_tr ? m_tr(key, fallback) : fallback;
}

QStringList BandSchedulerDialog::defaultModes()
{
    return { QStringLiteral("MeteoFax / HF WEFAX RX"), QStringLiteral("SSTV RX"),
             QStringLiteral("RTTY Text"), QStringLiteral("PSK Text"), QStringLiteral("MFSK Text"),
             QStringLiteral("CW Morse"), QStringLiteral("Feld Hell"), QStringLiteral("FT4"), QStringLiteral("FT8"),
             QStringLiteral("JS8"), QStringLiteral("MSK144"), QStringLiteral("Q65"), QStringLiteral("WSPR") };
}

QStringList BandSchedulerDialog::defaultBands()
{
    return { QStringLiteral("160m"), QStringLiteral("80m"), QStringLiteral("60m"),
             QStringLiteral("40m"), QStringLiteral("30m"), QStringLiteral("20m"),
             QStringLiteral("17m"), QStringLiteral("15m"), QStringLiteral("12m"),
             QStringLiteral("10m"), QStringLiteral("6m"), QStringLiteral("2m"),
             QStringLiteral("70cm"), QStringLiteral("23cm") };
}

void BandSchedulerDialog::buildUi()
{
    setWindowTitle(L(QStringLiteral("scheduler_dialog_title"), QStringLiteral("Band / frequency scheduler")));
    resize(780, 500);
    setMinimumSize(0, 0);

    QVBoxLayout *outer = new QVBoxLayout(this);
    outer->setContentsMargins(10, 10, 10, 10);
    outer->setSpacing(8);

    QLabel *description = new QLabel(L(QStringLiteral("scheduler_plan_description"),
        QStringLiteral("UTC QSY plan. At its time each entry switches the m³ mode, the band and the rig frequency, every day or only on the chosen date. The plan never starts TX. It waits until TX ends and, in FT4/FT8, until the current QSO is complete.")), this);
    description->setWordWrap(true);
    outer->addWidget(description);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(7);
    const QStringList headers = {
        L(QStringLiteral("enabled"), QStringLiteral("Enabled")),
        L(QStringLiteral("scheduler_date"), QStringLiteral("Date")),
        L(QStringLiteral("utc_time"), QStringLiteral("UTC time")),
        L(QStringLiteral("mode"), QStringLiteral("Mode")),
        L(QStringLiteral("band"), QStringLiteral("Band")),
        L(QStringLiteral("frequency"), QStringLiteral("Frequency")),
        L(QStringLiteral("source"), QStringLiteral("Source"))
    };
    for (int i = 0; i < headers.size(); ++i) {
        auto *item = new QTableWidgetItem(headers.at(i));
        item->setData(SourceTextRole, headers.at(i));
        m_table->setHorizontalHeaderItem(i, item);
    }
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    outer->addWidget(m_table, 1);

    QGroupBox *editorGroup = new QGroupBox(L(QStringLiteral("scheduler_entry_editor"), QStringLiteral("Entry editor")), this);
    QGridLayout *grid = new QGridLayout(editorGroup);
    grid->setHorizontalSpacing(8);
    grid->setVerticalSpacing(6);

    m_chkEnabled = new QCheckBox(L(QStringLiteral("enabled"), QStringLiteral("Enabled")), editorGroup);
    m_chkEnabled->setChecked(true);
    m_timeEdit = new QTimeEdit(editorGroup);
    m_timeEdit->setDisplayFormat(QStringLiteral("HH:mm"));
    m_timeEdit->setTime(QDateTime::currentDateTimeUtc().time().addSecs(3600));
    m_chkOnlyDate = new QCheckBox(L(QStringLiteral("scheduler_only_on_date"), QStringLiteral("Only on UTC date")), editorGroup);
    m_dateEdit = new QDateEdit(editorGroup);
    m_dateEdit->setDisplayFormat(QStringLiteral("yyyy-MM-dd"));
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDate(QDateTime::currentDateTimeUtc().date());
    m_dateEdit->setEnabled(false);

    m_modeCombo = new QComboBox(editorGroup);
    m_modeCombo->addItems(defaultModes());
    m_bandCombo = new QComboBox(editorGroup);
    m_bandCombo->addItems(defaultBands());

    m_chkFtStandard = new QCheckBox(L(QStringLiteral("scheduler_use_frequency_list"), QStringLiteral("Use the frequency list entry for the band")), editorGroup);
    m_freqMHz = new QDoubleSpinBox(editorGroup);
    m_freqMHz->setDecimals(6);
    m_freqMHz->setMinimum(0.100000);
    m_freqMHz->setMaximum(1300.000000);
    m_freqMHz->setSingleStep(0.001000);
    m_freqMHz->setSuffix(QStringLiteral(" MHz"));

    grid->addWidget(m_chkEnabled, 0, 0);
    grid->addWidget(new QLabel(L(QStringLiteral("utc_time"), QStringLiteral("UTC time")), editorGroup), 0, 1);
    grid->addWidget(m_timeEdit, 0, 2);
    grid->addWidget(m_chkOnlyDate, 1, 1);
    grid->addWidget(m_dateEdit, 1, 2);
    grid->addWidget(new QLabel(L(QStringLiteral("mode"), QStringLiteral("Mode")), editorGroup), 2, 0);
    grid->addWidget(m_modeCombo, 2, 1, 1, 2);
    grid->addWidget(new QLabel(L(QStringLiteral("band"), QStringLiteral("Band")), editorGroup), 3, 0);
    grid->addWidget(m_bandCombo, 3, 1, 1, 2);
    grid->addWidget(m_chkFtStandard, 4, 0, 1, 3);
    grid->addWidget(new QLabel(L(QStringLiteral("frequency_mhz"), QStringLiteral("Frequency MHz")), editorGroup), 5, 0);
    grid->addWidget(m_freqMHz, 5, 1, 1, 2);

    QHBoxLayout *buttonRow = new QHBoxLayout();
    m_addButton = new QPushButton(L(QStringLiteral("add"), QStringLiteral("Add")), editorGroup);
    m_updateButton = new QPushButton(L(QStringLiteral("update_selected"), QStringLiteral("Update selected")), editorGroup);
    m_removeButton = new QPushButton(L(QStringLiteral("remove_selected"), QStringLiteral("Remove selected")), editorGroup);
    buttonRow->addWidget(m_addButton);
    buttonRow->addWidget(m_updateButton);
    buttonRow->addWidget(m_removeButton);
    m_removeAllButton = new QPushButton(L(QStringLiteral("scheduler_remove_all"), QStringLiteral("Remove all")), editorGroup);
    buttonRow->addWidget(m_removeAllButton);
    buttonRow->addStretch(1);
    m_importButton = new QPushButton(L(QStringLiteral("scheduler_import"), QStringLiteral("Import...")), editorGroup);
    m_exportButton = new QPushButton(L(QStringLiteral("scheduler_export"), QStringLiteral("Export...")), editorGroup);
    buttonRow->addWidget(m_importButton);
    buttonRow->addWidget(m_exportButton);
    grid->addLayout(buttonRow, 6, 0, 1, 3);

    m_hint = new QLabel(editorGroup);
    m_hint->setWordWrap(true);
    grid->addWidget(m_hint, 7, 0, 1, 3);

    outer->addWidget(editorGroup);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setText(L(QStringLiteral("ok"), QStringLiteral("OK")));
    buttons->button(QDialogButtonBox::Cancel)->setText(L(QStringLiteral("cancel"), QStringLiteral("Cancel")));
    outer->addWidget(buttons);

    connect(m_table, &QTableWidget::cellClicked, this, [this](int row, int) { loadRowToEditor(row); });
    connect(m_table, &QTableWidget::cellDoubleClicked, this, [this](int row, int) { loadRowToEditor(row); });
    connect(m_modeCombo, &QComboBox::currentTextChanged, this, [this]() { updateEditorStandardFrequency(); });
    connect(m_bandCombo, &QComboBox::currentTextChanged, this, [this]() { updateEditorStandardFrequency(); });
    connect(m_chkFtStandard, &QCheckBox::toggled, this, [this]() { updateEditorStandardFrequency(); });
    connect(m_addButton, &QPushButton::clicked, this, [this]() {
        m_entries.append(editorEntry());
        refreshTable();
        m_table->selectRow(m_entries.size() - 1);
    });
    connect(m_updateButton, &QPushButton::clicked, this, [this]() {
        const int row = m_table->currentRow();
        if (row >= 0 && row < m_entries.size()) {
            m_entries[row] = editorEntry();
            refreshTable();
            m_table->selectRow(row);
        }
    });
    connect(m_removeButton, &QPushButton::clicked, this, [this]() {
        const int row = m_table->currentRow();
        if (row >= 0 && row < m_entries.size()) {
            m_entries.removeAt(row);
            refreshTable();
            if (!m_entries.isEmpty()) m_table->selectRow(qMin(row, m_entries.size() - 1));
        }
    });
    connect(m_removeAllButton, &QPushButton::clicked, this, [this]() {
        if (m_entries.isEmpty()) return;
        if (QMessageBox::question(this, windowTitle(),
                L(QStringLiteral("scheduler_remove_all_confirm"), QStringLiteral("Remove all %1 entries from the plan?")).arg(m_entries.size()))
            != QMessageBox::Yes) {
            return;
        }
        m_entries.clear();
        refreshTable();
    });
    connect(m_chkOnlyDate, &QCheckBox::toggled, m_dateEdit, &QWidget::setEnabled);
    connect(m_importButton, &QPushButton::clicked, this, [this]() { importPlan(); });
    connect(m_exportButton, &QPushButton::clicked, this, [this]() { exportPlan(); });
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    updateEditorStandardFrequency();
}

void BandSchedulerDialog::refreshTable()
{
    m_table->setRowCount(m_entries.size());
    for (int r = 0; r < m_entries.size(); ++r) {
        const ScheduledQsyEntry &e = m_entries.at(r);
        const QStringList cells = {
            e.enabled ? L(QStringLiteral("yes"), QStringLiteral("Yes")) : L(QStringLiteral("no"), QStringLiteral("No")),
            e.dateUtc.isValid() ? e.dateUtc.toString(Qt::ISODate) : L(QStringLiteral("scheduler_daily"), QStringLiteral("daily")),
            e.timeUtc.toString(QStringLiteral("HH:mm")),
            normalizedMode(e.mode),
            e.band,
            frequencyText(e.frequencyHz),
            e.useFtStandard ? L(QStringLiteral("scheduler_standard_ft"), QStringLiteral("standard FT"))
                            : L(QStringLiteral("manual"), QStringLiteral("manual"))
        };
        for (int c = 0; c < cells.size(); ++c) {
            auto *item = new QTableWidgetItem(cells.at(c));
            item->setData(SourceTextRole, cells.at(c));
            m_table->setItem(r, c, item);
        }
    }
    m_table->resizeColumnsToContents();
}

void BandSchedulerDialog::loadRowToEditor(int row)
{
    if (row < 0 || row >= m_entries.size()) return;
    setEditorEntry(m_entries.at(row));
}

ScheduledQsyEntry BandSchedulerDialog::editorEntry() const
{
    ScheduledQsyEntry e;
    e.enabled = m_chkEnabled->isChecked();
    e.timeUtc = m_timeEdit->time();
    if (m_chkOnlyDate->isChecked()) e.dateUtc = m_dateEdit->date();
    e.mode = normalizedMode(m_modeCombo->currentText());
    e.band = m_bandCombo->currentText().trimmed();
    e.useFtStandard = m_chkFtStandard->isChecked();
    e.frequencyHz = m_freqMHz->value() * 1000000.0;
    if (e.useFtStandard) {
        const double standard = standardFtFrequencyHz(e.mode, e.band);
        if (standard > 0.0) e.frequencyHz = standard;
    }
    return e;
}

void BandSchedulerDialog::setEditorEntry(const ScheduledQsyEntry &entry)
{
    m_chkEnabled->setChecked(entry.enabled);
    if (entry.timeUtc.isValid()) m_timeEdit->setTime(entry.timeUtc);
    m_chkOnlyDate->setChecked(entry.dateUtc.isValid());
    if (entry.dateUtc.isValid()) m_dateEdit->setDate(entry.dateUtc);
    m_dateEdit->setEnabled(entry.dateUtc.isValid());
    const int modeIndex = m_modeCombo->findText(normalizedMode(entry.mode), Qt::MatchFixedString);
    m_modeCombo->setCurrentIndex(modeIndex >= 0 ? modeIndex : m_modeCombo->findText(QStringLiteral("FT8")));
    const int bandIndex = m_bandCombo->findText(entry.band, Qt::MatchFixedString);
    m_bandCombo->setCurrentIndex(bandIndex >= 0 ? bandIndex : m_bandCombo->findText(QStringLiteral("20m")));
    m_chkFtStandard->setChecked(entry.useFtStandard);
    if (entry.frequencyHz > 0.0) {
        m_freqMHz->setValue(entry.frequencyHz / 1000000.0);
    }
    updateEditorStandardFrequency();
}

void BandSchedulerDialog::updateEditorStandardFrequency()
{
    const QString mode = normalizedMode(m_modeCombo->currentText());
    const bool isFt = !M3StandardFrequencies::listModeForName(mode).isEmpty();
    m_chkFtStandard->setEnabled(isFt);
    if (!isFt) {
        m_chkFtStandard->setChecked(false);
    }
    const double standard = standardFtFrequencyHz(mode, m_bandCombo->currentText());
    if (m_chkFtStandard->isChecked() && standard > 0.0) {
        const QSignalBlocker block(m_freqMHz);
        m_freqMHz->setValue(standard / 1000000.0);
    }
    m_freqMHz->setEnabled(!m_chkFtStandard->isChecked() || standard <= 0.0);
    if (m_hint != nullptr) {
        if (isFt && standard > 0.0) {
            m_hint->setText(L(QStringLiteral("scheduler_mode_standard_hint"), QStringLiteral("Standard frequency for this entry: %1. Manual frequency remains available when the standard checkbox is off."))
                            .arg(frequencyText(standard)));
        } else if (isFt) {
            m_hint->setText(L(QStringLiteral("scheduler_no_mode_standard_hint"), QStringLiteral("No built-in standard frequency is known for this band/mode; enter the frequency manually.")));
        } else {
            m_hint->setText(L(QStringLiteral("scheduler_manual_hint"), QStringLiteral("Non-FT modes use the manual frequency field. The scheduler only requests CAT QSY; it never starts TX.")));
        }
    }
}

QString BandSchedulerDialog::frequencyText(double hz) const
{
    if (hz <= 0.0) return QStringLiteral("--");
    return QStringLiteral("%1 MHz").arg(hz / 1000000.0, 0, 'f', 6);
}

QList<ScheduledQsyEntry> BandSchedulerDialog::entries() const
{
    QList<ScheduledQsyEntry> out = m_entries;
    std::sort(out.begin(), out.end(), [](const ScheduledQsyEntry &a, const ScheduledQsyEntry &b) {
        // Daily entries first, then dated ones by date; within each by time.
        if (a.dateUtc != b.dateUtc) {
            if (!a.dateUtc.isValid()) return true;
            if (!b.dateUtc.isValid()) return false;
            return a.dateUtc < b.dateUtc;
        }
        if (a.timeUtc == b.timeUtc) return a.mode < b.mode;
        return a.timeUtc < b.timeUtc;
    });
    return out;
}

double BandSchedulerDialog::standardFtFrequencyHz(const QString &modeName, const QString &band)
{
    const QString mode = M3StandardFrequencies::listModeForName(modeName);
    if (mode.isEmpty() || !M3StandardFrequencies::hasStandard(mode)) return 0.0;
    const double hz = M3StandardFrequencies::dialHz(mode, band);
    if (hz <= 0.0 && mode == QStringLiteral("FT4")) return M3StandardFrequencies::dialHz(QStringLiteral("FT8"), band);
    return hz;
}

QString BandSchedulerDialog::serializeEntries(const QList<ScheduledQsyEntry> &entries)
{
    QJsonArray array;
    for (const ScheduledQsyEntry &e : entries) {
        QJsonObject o;
        o.insert(QStringLiteral("enabled"), e.enabled);
        o.insert(QStringLiteral("time_utc"), e.timeUtc.toString(QStringLiteral("HH:mm")));
        if (e.dateUtc.isValid()) o.insert(QStringLiteral("date_utc"), e.dateUtc.toString(Qt::ISODate));
        o.insert(QStringLiteral("mode"), normalizedMode(e.mode));
        o.insert(QStringLiteral("band"), e.band.trimmed());
        o.insert(QStringLiteral("frequency_hz"), e.frequencyHz);
        o.insert(QStringLiteral("ft_standard"), e.useFtStandard);
        array.append(o);
    }
    return QString::fromUtf8(QJsonDocument(array).toJson(QJsonDocument::Compact));
}

QList<ScheduledQsyEntry> BandSchedulerDialog::deserializeEntries(const QString &json)
{
    QList<ScheduledQsyEntry> out;
    const QJsonDocument doc = QJsonDocument::fromJson(json.trimmed().toUtf8());
    if (!doc.isArray()) return out;
    for (const QJsonValue &value : doc.array()) {
        if (!value.isObject()) continue;
        const QJsonObject o = value.toObject();
        ScheduledQsyEntry e;
        e.enabled = o.value(QStringLiteral("enabled")).toBool(true);
        e.timeUtc = QTime::fromString(o.value(QStringLiteral("time_utc")).toString(), QStringLiteral("HH:mm"));
        if (!e.timeUtc.isValid()) continue;
        const QString date = o.value(QStringLiteral("date_utc")).toString().trimmed();
        if (!date.isEmpty()) {
            e.dateUtc = QDate::fromString(date, Qt::ISODate);
            if (!e.dateUtc.isValid()) continue;   // a broken date must not turn into "daily"
        }
        e.mode = normalizedMode(o.value(QStringLiteral("mode")).toString(QStringLiteral("FT8")));
        e.band = o.value(QStringLiteral("band")).toString(QStringLiteral("20m"));
        e.frequencyHz = o.value(QStringLiteral("frequency_hz")).toDouble(0.0);
        e.useFtStandard = o.value(QStringLiteral("ft_standard")).toBool(false);
        if (e.useFtStandard) {
            const double standard = standardFtFrequencyHz(e.mode, e.band);
            if (standard > 0.0) e.frequencyHz = standard;
        }
        if (e.frequencyHz > 0.0) out.append(e);
    }
    return out;
}

void BandSchedulerDialog::importPlan()
{
    const QString path = QFileDialog::getOpenFileName(this,
        L(QStringLiteral("scheduler_import_title"), QStringLiteral("Import QSY plan")), QString(),
        L(QStringLiteral("scheduler_plan_files"), QStringLiteral("QSY plan (*.json);;All files (*)")));
    if (path.isEmpty()) return;

    QFile file(path);
    QList<ScheduledQsyEntry> imported;
    // A plan is a few kB; refuse anything large instead of reading it.
    if (file.open(QIODevice::ReadOnly) && file.size() <= 1024 * 1024) {
        imported = deserializeEntries(QString::fromUtf8(file.readAll()));
    }
    if (imported.isEmpty()) {
        QMessageBox::warning(this, windowTitle(),
            L(QStringLiteral("scheduler_import_failed"), QStringLiteral("%1 contains no valid plan entries.")).arg(path));
        return;
    }

    bool replace = true;
    if (!m_entries.isEmpty()) {
        QMessageBox box(QMessageBox::Question, windowTitle(),
            L(QStringLiteral("scheduler_import_replace"),
              QStringLiteral("Import %1 entries. Replace the current %2 entries or append?")).arg(imported.size()).arg(m_entries.size()),
            QMessageBox::Cancel, this);
        QPushButton *replaceButton = box.addButton(L(QStringLiteral("scheduler_replace"), QStringLiteral("Replace")), QMessageBox::AcceptRole);
        QPushButton *appendButton = box.addButton(L(QStringLiteral("scheduler_append"), QStringLiteral("Append")), QMessageBox::AcceptRole);
        box.exec();
        if (box.clickedButton() == replaceButton) replace = true;
        else if (box.clickedButton() == appendButton) replace = false;
        else return;
    }
    if (replace) m_entries.clear();
    m_entries.append(imported);
    m_entries = entries();   // sorted
    refreshTable();
    if (!m_entries.isEmpty()) {
        m_table->selectRow(0);
        loadRowToEditor(0);
    }
}

void BandSchedulerDialog::exportPlan()
{
    const QString path = QFileDialog::getSaveFileName(this,
        L(QStringLiteral("scheduler_export_title"), QStringLiteral("Export QSY plan")), QStringLiteral("qsy-plan.json"),
        L(QStringLiteral("scheduler_plan_files"), QStringLiteral("QSY plan (*.json);;All files (*)")));
    if (path.isEmpty()) return;
    const QJsonDocument doc = QJsonDocument::fromJson(serializeEntries(entries()).toUtf8());
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly) || file.write(doc.toJson(QJsonDocument::Indented)) < 0 || !file.commit()) {
        QMessageBox::warning(this, windowTitle(),
            L(QStringLiteral("scheduler_export_failed"), QStringLiteral("Could not write %1: %2")).arg(path, file.errorString()));
    }
}
