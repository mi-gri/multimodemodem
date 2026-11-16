#ifndef BANDSCHEDULERDIALOG_H
#define BANDSCHEDULERDIALOG_H

#include <QDate>
#include <QDialog>
#include <QList>
#include <QString>
#include <QTime>

#include <functional>

class QCheckBox;
class QComboBox;
class QDateEdit;
class QDoubleSpinBox;
class QLabel;
class QPushButton;
class QTableWidget;
class QTimeEdit;

/**
 * @brief One UTC QSY scheduler entry (daily, or on one date).
 *
 * The scheduler intentionally stores only a safe QSY request: mode, band and
 * rig frequency. Applying it switches the m³ mode, band and rig frequency;
 * it never starts TX. It never stores TX/QSO automation state. Auto CQ/Auto
 * QSO can remain enabled; MainWindow decides when a pending QSY is safe to
 * apply after TX/QSO completion.
 */
struct ScheduledQsyEntry
{
    bool enabled = true;
    QTime timeUtc = QTime(0, 0);
    // Valid: this entry runs only on that UTC date (a one-time plan such as a
    // test run). Invalid: it runs every day.
    QDate dateUtc;
    QString mode;
    QString band;
    double frequencyHz = 0.0;
    bool useFtStandard = false;
    // WSPR only: the user explicitly allows this entry to send WSPR beacons
    // (the only automatic TX a running plan may start). Default: receive.
    bool wsprTx = false;

    QString triggerKey(const QDate &utcDate) const;
    bool runsOn(const QDate &utcDate) const { return !dateUtc.isValid() || dateUtc == utcDate; }
};

/**
 * @brief Editor for the daily band/frequency scheduler plan.
 */
class BandSchedulerDialog final : public QDialog
{
public:
    using Translator = std::function<QString(const QString &, const QString &)>;

    explicit BandSchedulerDialog(const QList<ScheduledQsyEntry> &entries,
                                 Translator translator,
                                 QWidget *parent = nullptr);

    QList<ScheduledQsyEntry> entries() const;

    static QString serializeEntries(const QList<ScheduledQsyEntry> &entries);
    static QList<ScheduledQsyEntry> deserializeEntries(const QString &json);
    static double standardFtFrequencyHz(const QString &modeName, const QString &band);
    static QStringList defaultModes();
    static QStringList defaultBands();
    /// Puts the preferred bands (Settings > Bands) on top of the band list.
    void setPreferredBands(const QStringList &preferred);
    /// Entries of @p entries that send (wsprTx), for confirmations.
    static QList<ScheduledQsyEntry> transmittingEntries(const QList<ScheduledQsyEntry> &entries);

private:
    QString L(const QString &key, const QString &fallback) const;
    void buildUi();
    void refreshTable();
    void loadRowToEditor(int row);
    void updateEditorStandardFrequency();
    ScheduledQsyEntry editorEntry() const;
    void setEditorEntry(const ScheduledQsyEntry &entry);
    QString frequencyText(double hz) const;
    void importPlan();
    void exportPlan();

    Translator m_tr;
    QList<ScheduledQsyEntry> m_entries;

    QTableWidget *m_table = nullptr;
    QCheckBox *m_chkEnabled = nullptr;
    QTimeEdit *m_timeEdit = nullptr;
    QCheckBox *m_chkOnlyDate = nullptr;
    QDateEdit *m_dateEdit = nullptr;
    QComboBox *m_modeCombo = nullptr;
    QComboBox *m_bandCombo = nullptr;
    QCheckBox *m_chkFtStandard = nullptr;
    QCheckBox *m_chkWsprTx = nullptr;
    QDoubleSpinBox *m_freqMHz = nullptr;
    QLabel *m_hint = nullptr;
    QPushButton *m_addButton = nullptr;
    QPushButton *m_updateButton = nullptr;
    QPushButton *m_removeButton = nullptr;
    QPushButton *m_removeAllButton = nullptr;
    QPushButton *m_importButton = nullptr;
    QPushButton *m_exportButton = nullptr;
};

#endif // BANDSCHEDULERDIALOG_H
