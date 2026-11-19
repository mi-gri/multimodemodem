// SPDX-License-Identifier: GPL-3.0-or-later
#include "AutoModeNoticeDialog.h"

#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace AutoModeNotice {

bool confirm(QWidget *parent, const std::function<QString(const QString &)> &translate)
{
    auto L = [&translate](const QString &source) { return translate ? translate(source) : source; };

    QDialog dialog(parent);
    dialog.setObjectName(QStringLiteral("autoModeNoticeDialog"));
    dialog.setWindowTitle(L(QStringLiteral("Enable Auto mode")));
    dialog.setModal(true);
    auto *layout = new QVBoxLayout(&dialog);

    auto *top = new QHBoxLayout();
    auto *icon = new QLabel(&dialog);
    icon->setPixmap(dialog.style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(48, 48));
    icon->setAlignment(Qt::AlignTop);
    top->addWidget(icon);

    auto *text = new QLabel(&dialog);
    text->setObjectName(QStringLiteral("autoModeNoticeText"));
    text->setWordWrap(true);
    text->setTextFormat(Qt::PlainText);
    text->setMinimumWidth(460);
    text->setText(L(QStringLiteral("Auto mode lets m³ transmit without your intervention:"))
                  + QStringLiteral("\n• ")
                  + L(QStringLiteral("FT8/FT4: answers CQ calls (Auto QSO) and completes QSOs, continues a running CQ series after a QSO or an unanswered attempt, sends RR73 again when the partner repeats his report, works the queue of callers and - if switched on in the Filter menu - answers calls without your own CQ."))
                  + QStringLiteral("\n• ")
                  + L(QStringLiteral("JS8: answers queries, relays messages and sends heartbeats or CQ by timer."))
                  + QStringLiteral("\n• ")
                  + L(QStringLiteral("WSPR: beacon transmissions of QSY plan entries that are explicitly marked for it."))
                  + QStringLiteral("\n• ")
                  + L(QStringLiteral("Always in force: no automatic transmission on receive-only bands, to excluded countries/continents, after STOP or after a stop for high SWR. Other modes never transmit automatically."))
                  + QStringLiteral("\n\n• ")
                  + L(QStringLiteral("Automatic operation may not be permitted in every country. Check the rules of your licence."))
                  + QStringLiteral("\n• ")
                  + L(QStringLiteral("In contests, automatic operation can lead to disqualification. Check the contest rules."))
                  + QStringLiteral("\n• ")
                  + L(QStringLiteral("Do not leave the station unattended for long periods. You remain responsible for every transmission."))
                  + QStringLiteral("\n• ")
                  + L(QStringLiteral("You alone are responsible for any damage that may result.")));
    top->addWidget(text, 1);
    layout->addLayout(top);

    auto *understood = new QCheckBox(L(QStringLiteral("I have read and understood your notice and want to continue")), &dialog);
    understood->setObjectName(QStringLiteral("autoModeNoticeUnderstood"));
    layout->addWidget(understood);

    auto *buttons = new QDialogButtonBox(&dialog);
    QPushButton *enable = buttons->addButton(L(QStringLiteral("Enable Auto mode")), QDialogButtonBox::AcceptRole);
    enable->setObjectName(QStringLiteral("autoModeNoticeEnable"));
    QPushButton *cancel = buttons->addButton(L(QStringLiteral("Cancel")), QDialogButtonBox::RejectRole);
    cancel->setDefault(true);
    enable->setEnabled(false);   // only after the notice was confirmed
    QObject::connect(understood, &QCheckBox::toggled, enable, &QPushButton::setEnabled);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    layout->activate();
    const int initialWidth = qMax(600, layout->minimumSize().width());
    dialog.resize(initialWidth, layout->totalHeightForWidth(initialWidth));

    return dialog.exec() == QDialog::Accepted && understood->isChecked();
}

} // namespace AutoModeNotice
