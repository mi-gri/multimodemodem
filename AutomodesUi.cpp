// Automodes: menu and dialog that gather every setting of the automatic transmissions.
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings/AppSettings.h"
#include "modems/ft8/Ft8Mode.h"

#include <QActionGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QPlainTextEdit>
#include <QRegularExpression>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>

void MainWindow::buildAutomodesMenu()
{
    if (m_menuAutomodes == nullptr || m_automodesStageGroup != nullptr) return;   // the menu object is made by rebuildMenuBar()
    if (m_actAutoMode != nullptr) {
        if (m_menuOperation != nullptr) m_menuOperation->removeAction(m_actAutoMode);
        m_menuAutomodes->addAction(m_actAutoMode);
    }
    m_menuAutomodes->addSeparator();
    m_automodesStageGroup = new QActionGroup(this);
    m_automodesStageGroup->setExclusive(true);
    for (int stage = 0; stage <= 4; ++stage) {
        QAction *action = m_menuAutomodes->addAction(QString());
        action->setCheckable(true);
        action->setData(stage);
        m_automodesStageGroup->addAction(action);
        connect(action, &QAction::triggered, this, [this, stage]() {
            if (m_cmbAutoStage == nullptr) return;
            const int idx = m_cmbAutoStage->findData(stage);
            if (idx >= 0) m_cmbAutoStage->setCurrentIndex(idx);
            rearmAutoStage();
        });
    }
    m_menuAutomodes->addSeparator();
    QAction *open = m_menuAutomodes->addAction(QString());
    open->setObjectName(QStringLiteral("automodesOpen"));
    connect(open, &QAction::triggered, this, &MainWindow::showAutomodesDialog);
}

void MainWindow::noteRecentTxOrigin(const QString &origin, const QString &message)
{
    m_recentTxOrigins.prepend(QStringLiteral("%1Z  %2  ->  %3")
                                  .arg(QDateTime::currentDateTimeUtc().toString(QStringLiteral("HH:mm:ss")), origin, message));
    while (m_recentTxOrigins.size() > 10) m_recentTxOrigins.removeLast();
    if (m_automodesDialog != nullptr && m_automodesDialog->isVisible()) refreshAutomodesDialog();
}

void MainWindow::buildAutomodesDialog()
{
    if (m_automodesDialog != nullptr) return;
    auto *dialog = new QDialog(this);
    m_automodesDialog = dialog;
    dialog->setObjectName(QStringLiteral("automodesDialog"));
    dialog->setWindowTitle(uiText("automodes_title", "Automodes"));
    dialog->setModal(false);
    dialog->resize(640, 560);
    auto *root = new QVBoxLayout(dialog);
    auto *tabs = new QTabWidget(dialog);
    root->addWidget(tabs);

    // ---- Overview
    auto *overview = new QWidget(tabs);
    auto *ov = new QVBoxLayout(overview);
    m_automodesAutoCheck = new QCheckBox(uiText("automodes_auto_on", "Auto mode on (switching it on shows the legal and safety notice)"), overview);
    connect(m_automodesAutoCheck, &QCheckBox::clicked, this, [this](bool on) { setAutoModeEnabledFromUi(on); refreshAutomodesDialog(); });
    ov->addWidget(m_automodesAutoCheck);
    m_automodesStatus = new QLabel(overview);
    m_automodesStatus->setWordWrap(true);
    ov->addWidget(m_automodesStatus);
    auto *gates = new QLabel(uiText("automodes_gates", "These rules always apply, whatever the stage: nothing is sent after a program start until you switch Auto mode on; STOP, a mode change and a band change pause the automation until you select the stage again; no automatic transmission without a known frequency on a band with TX permission; blocked stations, excluded countries/continents, a-priori (AP) decodes and implausible callsigns are never answered; the SWR protection stops a transmission."), overview);
    gates->setWordWrap(true);
    ov->addWidget(gates);
    auto *recentBox = new QGroupBox(uiText("automodes_recent", "Last transmissions and why they happened"), overview);
    auto *rb = new QVBoxLayout(recentBox);
    m_automodesRecent = new QListWidget(recentBox);
    m_automodesRecent->setSelectionMode(QAbstractItemView::NoSelection);
    rb->addWidget(m_automodesRecent);
    ov->addWidget(recentBox, 1);
    tabs->addTab(overview, uiText("automodes_tab_overview", "Overview"));

    // ---- FT8 / FT4: the controls move here from the FT panel
    auto *ft = new QWidget(tabs);
    auto *ftl = new QVBoxLayout(ft);
    auto *stageBox = new QGroupBox(uiText("automodes_ft_stage_box", "Stage"), ft);
    auto *sf = new QFormLayout(stageBox);
    m_automodesStageCombo = new QComboBox(stageBox);
    for (int i = 0; i < (m_cmbAutoStage ? m_cmbAutoStage->count() : 0); ++i)
        m_automodesStageCombo->addItem(m_cmbAutoStage->itemText(i), m_cmbAutoStage->itemData(i));
    m_automodesStageCombo->setToolTip(m_cmbAutoStage ? m_cmbAutoStage->toolTip() : QString());
    connect(m_automodesStageCombo, QOverload<int>::of(&QComboBox::activated), this, [this](int idx) {
        if (m_cmbAutoStage == nullptr) return;
        const int panelIdx = m_cmbAutoStage->findData(m_automodesStageCombo->itemData(idx));
        if (panelIdx >= 0) m_cmbAutoStage->setCurrentIndex(panelIdx);
        rearmAutoStage();
    });
    sf->addRow(uiText("automodes_ft_stage", "Auto stage"), m_automodesStageCombo);
    const auto moveRow = [](QFormLayout *form, QLabel *label, const QString &text, QWidget *field) {
        if (field == nullptr) return;
        QLabel *l = label != nullptr ? label : new QLabel(text);
        l->setText(text);
        form->addRow(l, field);
        if (label != nullptr) label->show();
        field->show();
    };
    moveRow(sf, nullptr, uiText("ft_auto_alternate", "Switch after"), m_spinAutoAlternate);
    moveRow(sf, nullptr, uiText("ft_auto_no_target", "CQ after"), m_spinAutoNoTarget);
    ftl->addWidget(stageBox);
    auto *cqBox = new QGroupBox(uiText("automodes_ft_cq_box", "CQ series and QSO limits"), ft);
    auto *cf = new QFormLayout(cqBox);
    moveRow(cf, m_lblFt8CqTimeout, uiText("ft8_cq_series_count", "CQs per series"), m_spinFt8CqTimeoutMin);
    moveRow(cf, nullptr, uiText("ft8_cq_resume_wait", "Wait slots before CQ"), m_spinFt8CqResumeWait);
    moveRow(cf, m_lblFt8NoResponseLimit, uiText("ft8_no_response_limit", "No-response limit"), m_spinFt8NoResponseLimit);
    ftl->addWidget(cqBox);
    auto *callBox = new QGroupBox(uiText("automodes_ft_callers_box", "Callers and logging"), ft);
    auto *cl = new QVBoxLayout(callBox);
    if (m_chkFt8CallerQueue != nullptr) { cl->addWidget(m_chkFt8CallerQueue); m_chkFt8CallerQueue->show(); }
    auto *unsolicited = new QCheckBox(uiText("filter_answer_calls", "Answer calls without own CQ"), callBox);
    unsolicited->setToolTip(m_actFilterAnswerCalls != nullptr ? m_actFilterAnswerCalls->toolTip() : QString());
    if (m_actFilterAnswerCalls != nullptr) {
        unsolicited->setChecked(m_actFilterAnswerCalls->isChecked());
        connect(unsolicited, &QCheckBox::toggled, m_actFilterAnswerCalls, &QAction::setChecked);
        connect(m_actFilterAnswerCalls, &QAction::toggled, unsolicited, [unsolicited](bool on) {
            const QSignalBlocker block(unsolicited);
            unsolicited->setChecked(on);
        });
        if (m_menuFilter != nullptr) m_menuFilter->removeAction(m_actFilterAnswerCalls);   // lives here now
    }
    cl->addWidget(unsolicited);
    if (m_chkFt8AutoLog != nullptr) { cl->addWidget(m_chkFt8AutoLog); m_chkFt8AutoLog->show(); }
    ftl->addWidget(callBox);
    ftl->addStretch(1);
    tabs->addTab(ft, uiText("automodes_tab_ft", "FT8 / FT4"));

    // ---- Targets: whom may Auto call or answer?
    auto *targets = new QWidget(tabs);
    auto *tl = new QVBoxLayout(targets);
    auto *blackBox = new QGroupBox(uiText("automodes_black_box", "Blacklist"), targets);
    auto *bl = new QVBoxLayout(blackBox);
    auto *blackEdit = new QPlainTextEdit(blackBox);
    blackEdit->setPlaceholderText(uiText("automodes_black_hint", "one callsign per line; hidden from FT traffic and never answered by Auto"));
    blackEdit->setPlainText(m_settings.ftBlacklistCalls.join(QLatin1Char('\n')));
    blackEdit->setMaximumHeight(110);
    connect(blackEdit, &QPlainTextEdit::textChanged, this, [this, blackEdit]() {
        QStringList calls;
        for (const QString &line : blackEdit->toPlainText().split(QRegularExpression(QStringLiteral("[\\s,;]+")), Qt::SkipEmptyParts))
            calls << line.trimmed().toUpper();
        calls.removeDuplicates();
        m_settings.ftBlacklistCalls = calls;
        m_settings.save();
    });
    bl->addWidget(blackEdit);
    tl->addWidget(blackBox);
    auto *ruleBox = new QGroupBox(uiText("automodes_rules_box", "Which CQs does Auto answer?"), targets);
    auto *rl = new QVBoxLayout(ruleBox);
    auto *directed = new QCheckBox(uiText("automodes_directed", "Answer directed CQs only if my station fits"), ruleBox);
    directed->setToolTip(uiText("automodes_directed_tip", "Auto skips CQ NA, CQ EU, CQ JA, CQ USA and similar calls when your station is outside that area."));
    directed->setChecked(m_settings.ftAutoQsoRespectCqDirective);
    connect(directed, &QCheckBox::toggled, this, [this](bool on) { m_settings.ftAutoQsoRespectCqDirective = on; m_settings.save(); });
    rl->addWidget(directed);
    auto *dxRow = new QFormLayout();
    auto *dxRule = new QComboBox(ruleBox);
    dxRule->addItem(uiText("automodes_dx_continent", "my continent"), QStringLiteral("continent"));
    dxRule->addItem(uiText("automodes_dx_country", "my country (DXCC)"), QStringLiteral("country"));
    const int dxIdx = dxRule->findData(m_settings.ftAutoQsoCqDxRule);
    dxRule->setCurrentIndex(dxIdx >= 0 ? dxIdx : 0);
    connect(dxRule, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, dxRule](int) {
        m_settings.ftAutoQsoCqDxRule = dxRule->currentData().toString(); m_settings.save();
    });
    dxRow->addRow(uiText("automodes_dx_rule", "Do not answer CQ DX from"), dxRule);
    rl->addLayout(dxRow);
    auto *onlyTarget = new QCheckBox(uiText("automodes_only_target", "Work only stations in my CQ destination"), ruleBox);
    onlyTarget->setToolTip(uiText("automodes_only_target_tip", "With a CQ destination such as CQ USA selected, Auto answers only CQs from that area and the sequencer ignores callers from outside it."));
    onlyTarget->setChecked(m_settings.ftAutoQsoOnlyCqTarget);
    connect(onlyTarget, &QCheckBox::toggled, this, [this](bool on) { m_settings.ftAutoQsoOnlyCqTarget = on; m_settings.save(); });
    rl->addWidget(onlyTarget);
    tl->addWidget(ruleBox);
    auto *noteTargets = new QLabel(uiText("automodes_targets_note", "Excluded countries and continents are set in the Filter menu; the watch list and the colours stay in Settings -> Display."), targets);
    noteTargets->setWordWrap(true);
    tl->addWidget(noteTargets);
    tl->addStretch(1);
    tabs->addTab(targets, uiText("automodes_tab_targets", "Targets"));

    // ---- JS8: the automatic-operation group moves here
    auto *js8 = new QWidget(tabs);
    auto *jl = new QVBoxLayout(js8);
    if (m_grpJs8Auto != nullptr) { m_grpJs8Auto->setParent(js8); jl->addWidget(m_grpJs8Auto); m_grpJs8Auto->show(); }
    else jl->addWidget(new QLabel(uiText("automodes_js8_missing", "JS8 automatic operation is not available."), js8));
    jl->addStretch(1);
    tabs->addTab(js8, QStringLiteral("JS8"));

    // ---- WSPR and safety: pointers to the places that own those settings
    auto *other = new QWidget(tabs);
    auto *ol = new QVBoxLayout(other);
    auto *wsprInfo = new QLabel(uiText("automodes_wspr_info", "WSPR beacon: switch it on in the WSPR page (\\, TX share). It needs Auto mode and is paused by the QSY plan unless the plan entry allows it."), other);
    wsprInfo->setWordWrap(true);
    ol->addWidget(wsprInfo);
    auto *safetyInfo = new QLabel(uiText("automodes_safety_info", "TX bands, SWR protection and the Tune length are set in Settings (Bands and Audio / PTT / CAT)."), other);
    safetyInfo->setWordWrap(true);
    ol->addWidget(safetyInfo);
    auto *settingsButton = new QPushButton(uiText("automodes_open_settings", "Open settings ..."), other);
    connect(settingsButton, &QPushButton::clicked, this, [this]() { showAppSettingsDialogPage(AppSettingsDialog::InitialPage::Bands); });
    ol->addWidget(settingsButton, 0, Qt::AlignLeft);
    ol->addStretch(1);
    tabs->addTab(other, uiText("automodes_tab_wspr_safety", "WSPR / Safety"));

    auto *close = new QPushButton(uiText("automodes_close", "Close"), dialog);
    connect(close, &QPushButton::clicked, dialog, &QDialog::hide);
    root->addWidget(close, 0, Qt::AlignRight);
    updateFt8AutoModeVisibility();   // the moved widgets are visible now
    refreshAutomodesDialog();
}

void MainWindow::showAutomodesDialog()
{
    buildAutomodesDialog();
    refreshAutomodesDialog();
    m_automodesDialog->show();
    m_automodesDialog->raise();
    m_automodesDialog->activateWindow();
}

void MainWindow::refreshAutomodesDialog()
{
    if (m_menuAutomodes != nullptr) {
        m_menuAutomodes->setTitle(uiText("menu_automodes", "Automodes"));
        for (QAction *action : m_menuAutomodes->actions()) {
            if (action->objectName() == QLatin1String("automodesOpen")) {
                action->setText(uiText("automodes_open", "Automodes settings ..."));
                continue;
            }
            const int stage = action->data().isValid() ? action->data().toInt() : -1;
            if (stage >= 0 && m_cmbAutoStage != nullptr) {
                const int idx = m_cmbAutoStage->findData(stage);
                if (idx >= 0) action->setText(m_cmbAutoStage->itemText(idx));
                const QSignalBlocker block(action);
                action->setChecked(m_autoPlanner.stage() == stage);
            }
        }
    }
    if (m_automodesAutoCheck != nullptr) {
        const QSignalBlocker block(m_automodesAutoCheck);
        m_automodesAutoCheck->setChecked(m_ft8AutoModeEnabled);
    }
    if (m_automodesStageCombo != nullptr) {
        const QSignalBlocker block(m_automodesStageCombo);
        const int idx = m_automodesStageCombo->findData(m_autoPlanner.stage());
        if (idx >= 0) m_automodesStageCombo->setCurrentIndex(idx);
    }
    if (m_automodesStatus != nullptr && m_lblAutoStageStatus != nullptr) m_automodesStatus->setText(m_lblAutoStageStatus->text());
    if (m_automodesRecent != nullptr) {
        m_automodesRecent->clear();
        if (m_recentTxOrigins.isEmpty())
            m_automodesRecent->addItem(uiText("automodes_recent_none", "(no FT transmission since the start)"));
        else
            m_automodesRecent->addItems(m_recentTxOrigins);
    }
}
