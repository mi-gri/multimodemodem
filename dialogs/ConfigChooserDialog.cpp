// SPDX-License-Identifier: GPL-3.0-or-later
#include "ConfigChooserDialog.h"

#include "../utils/InstallationPaths.h"
#include "../utils/InstanceLock.h"
#include "../utils/RuntimeI18n.h"

#include <QCoreApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include <algorithm>
#include <memory>

namespace M3ConfigChooser {
namespace {

QString tr(const char *key, const char *fallback)
{
    return M3I18n::translate(QString::fromLatin1(key), QString::fromUtf8(fallback));
}

QString lastChoiceFile()
{
    return QDir(M3Installation::configDirectory()).filePath(QStringLiteral(".last-config"));
}

bool lockedByAnotherInstance(const QString &settingsFile)
{
    std::unique_ptr<QLockFile> probe;
    const auto result = M3InstanceLock::acquire(settingsFile, probe);
    return result == M3InstanceLock::Result::AlreadyRunning;   // the probe lock is released on return
}

} // namespace

QVector<Entry> entries()
{
    const QString configDir = M3Installation::configDirectory();
    QVector<Entry> list;
    Entry standard;
    standard.settingsFile = M3Installation::defaultSettingsFile(QCoreApplication::applicationDirPath(), configDir);
    standard.label = tr("cfgchooser_default", "Default configuration");
    standard.running = lockedByAnotherInstance(standard.settingsFile);
    list.append(standard);

    QStringList names;
    const QDir configs(QDir(configDir).filePath(QStringLiteral("configs")));
    for (const QFileInfo &info : configs.entryInfoList(QStringList{QStringLiteral("*") + M3Installation::configExtension(),
                                                                    QStringLiteral("*") + M3Installation::legacyConfigExtension()},
                                                       QDir::Files, QDir::Name | QDir::IgnoreCase)) {
        const QString name = info.completeBaseName();
        if (M3Installation::isValidConfigArgument(name) && !names.contains(name)) names << name;   // .mad converts to .m3cfg at start
    }
    std::sort(names.begin(), names.end(), [](const QString &a, const QString &b) { return a.compare(b, Qt::CaseInsensitive) < 0; });
    for (const QString &name : names) {
        Entry entry;
        entry.name = name;
        entry.label = name;
        entry.settingsFile = configs.filePath(name + M3Installation::configExtension());
        entry.running = lockedByAnotherInstance(entry.settingsFile);
        list.append(entry);
    }
    return list;
}

bool choose(QString *config)
{
    // The language comes from the default configuration (the menu opens before any settings are read).
    const QString defaultFile = M3Installation::defaultSettingsFile(QCoreApplication::applicationDirPath(), M3Installation::configDirectory());
    M3I18n::setLanguageCode(QSettings(defaultFile, QSettings::IniFormat).value(QStringLiteral("UI/language"), QStringLiteral("en")).toString());

    const QVector<Entry> list = entries();
    QString last;
    {
        QFile file(lastChoiceFile());
        if (file.open(QIODevice::ReadOnly)) last = QString::fromUtf8(file.readAll()).trimmed();
    }

    QDialog dialog;
    dialog.setObjectName(QStringLiteral("configChooserDialog"));
    dialog.setWindowTitle(tr("cfgchooser_title", "Select configuration"));
    auto *layout = new QVBoxLayout(&dialog);
    layout->addWidget(new QLabel(tr("cfgchooser_text", "Which configuration should MultiModeModem start with?"), &dialog));
    auto *listWidget = new QListWidget(&dialog);
    listWidget->setObjectName(QStringLiteral("configChooserList"));
    int preselect = -1;
    for (int i = 0; i < list.size(); ++i) {
        const Entry &entry = list.at(i);
        auto *item = new QListWidgetItem(entry.running ? entry.label + QStringLiteral("  ") + tr("cfgchooser_running", "(already running)") : entry.label, listWidget);
        item->setData(Qt::UserRole, entry.name);
        if (entry.running) item->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        if (!entry.running && entry.name == last && preselect < 0) preselect = i;
    }
    if (preselect < 0) {
        for (int i = 0; i < list.size(); ++i) if (!list.at(i).running) { preselect = i; break; }
    }
    if (preselect >= 0) listWidget->setCurrentRow(preselect);
    listWidget->setMinimumSize(360, 220);
    layout->addWidget(listWidget, 1);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Cancel, &dialog);
    QPushButton *start = buttons->addButton(tr("cfgchooser_start", "Start"), QDialogButtonBox::AcceptRole);
    start->setObjectName(QStringLiteral("configChooserStart"));
    start->setDefault(true);
    start->setEnabled(preselect >= 0);
    QObject::connect(listWidget, &QListWidget::currentItemChanged, &dialog, [start](QListWidgetItem *current) {
        start->setEnabled(current != nullptr && (current->flags() & Qt::ItemIsEnabled));
    });
    QObject::connect(listWidget, &QListWidget::itemDoubleClicked, &dialog, [&dialog](QListWidgetItem *item) {
        if (item->flags() & Qt::ItemIsEnabled) dialog.accept();
    });
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);

    if (dialog.exec() != QDialog::Accepted || listWidget->currentItem() == nullptr) return false;
    const QString chosen = listWidget->currentItem()->data(Qt::UserRole).toString();
    QFile file(lastChoiceFile());
    QDir().mkpath(QFileInfo(file).absolutePath());
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) file.write(chosen.toUtf8());
    if (config != nullptr) *config = chosen;
    return true;
}

} // namespace M3ConfigChooser
