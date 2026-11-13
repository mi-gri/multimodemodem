// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// --i18n-report: builds the main window and the Settings dialog in the
// configured language and lists visible texts that are still English
// (labels, buttons, group and tab titles, combo items, placeholders,
// tooltips and What's This help).  A maintenance aid for translators.

#include "dialogs/AppSettingsDialog.h"
#include "settings/AppSettings.h"
#include "utils/RuntimeI18n.h"

#include <QAbstractButton>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QApplication>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QRegularExpression>
#include <QSet>
#include <QTabBar>
#include <QTabWidget>
#include <QTextStream>

namespace M3I18nReport {

inline bool looksEnglish(const QString &text)
{
    static const QRegularExpression words(QStringLiteral(
        "\\b(the|and|or|to|of|is|are|for|with|this|when|from|not|only|if|on|in|by|be|it|as|an|use|uses|used|"
        "select|selects|shows|show|enable|enables|open|opens|click|set|sets|keeps|stays|without|after|before)\\b"),
        QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression german(QStringLiteral(
        "[äöüßÄÖÜ]|\\b(der|die|das|und|nicht|mit|für|ist|wird|werden|oder|eine|einen|beim|nur|auf|wählen|aus|zum|zur|sich)\\b"),
        QRegularExpression::CaseInsensitiveOption);
    const QString plain = QString(text).remove(QRegularExpression(QStringLiteral("<[^>]*>"))).simplified();
    if (plain.size() < 4 || plain.count(QLatin1Char(' ')) < 1) return false;
    if (M3I18n::languageCode() == QLatin1String("de") && german.match(plain).hasMatch()) return false;
    int english = 0;
    for (auto it = words.globalMatch(plain); it.hasNext(); it.next()) ++english;
    return english >= 1;
}

inline void collect(QObject *root, QMap<QString, QSet<QString>> &found)
{
    auto add = [&found](const QString &kind, const QString &text) {
        if (!text.trimmed().isEmpty() && looksEnglish(text)) found[text.simplified()].insert(kind);
    };
    QList<QObject *> objects = root->findChildren<QObject *>();
    objects.prepend(root);
    for (QObject *object : objects) {
        if (auto *widget = qobject_cast<QWidget *>(object)) {
            add(QStringLiteral("tooltip"), widget->toolTip());
            add(QStringLiteral("whatsthis"), widget->whatsThis());
            add(QStringLiteral("statustip"), widget->statusTip());
        }
        if (auto *label = qobject_cast<QLabel *>(object)) add(QStringLiteral("label"), label->text());
        if (auto *button = qobject_cast<QAbstractButton *>(object)) add(QStringLiteral("button"), button->text());
        if (auto *group = qobject_cast<QGroupBox *>(object)) add(QStringLiteral("group"), group->title());
        if (auto *edit = qobject_cast<QLineEdit *>(object)) add(QStringLiteral("placeholder"), edit->placeholderText());
        if (auto *combo = qobject_cast<QComboBox *>(object))
            for (int i = 0; i < combo->count(); ++i) add(QStringLiteral("combo"), combo->itemText(i));
        if (auto *tabs = qobject_cast<QTabBar *>(object))
            for (int i = 0; i < tabs->count(); ++i) {
                add(QStringLiteral("tab"), tabs->tabText(i));
                add(QStringLiteral("tooltip"), tabs->tabToolTip(i));
            }
        if (auto *action = qobject_cast<QAction *>(object)) {
            add(QStringLiteral("action"), action->text());
            add(QStringLiteral("tooltip"), action->toolTip());
        }
    }
}

// M3_I18N_SWITCH=en: switch the language live from the configured one (as
// the Language menu does) and list texts that still look German.
inline int runSwitch(QApplication &app, QWidget &mainWindow, const QString &code)
{
    app.processEvents();
    QAction *target = nullptr;
    for (QAction *action : mainWindow.findChildren<QAction *>())
        if (action->isCheckable() && action->data().toString() == code && action->actionGroup()) target = action;
    if (!target) { QTextStream(stderr) << "no language action for " << code << "\n"; return 2; }
    target->trigger();
    app.processEvents();
    static const QRegularExpression german(QStringLiteral(
        "[äöüßÄÖÜ]|\\b(der|die|das|und|nicht|mit|für|ist|wird|oder|eine|einen|beim|nur|auf|wählen|aus|zum|zur|Dekodieren|Einstellungen|Datei|Hilfe|Sprache|Beenden)\\b"));
    QMap<QString, QSet<QString>> left;
    // Into English: list German leftovers.  Into another language: list
    // English-looking texts (compare with a fresh start in that language).
    auto add = [&](const QString &kind, const QString &text) {
        const QString plain = QString(text).remove(QRegularExpression(QStringLiteral("<[^>]*>"))).simplified();
        if (plain.isEmpty()) return;
        const bool leftover = code == QStringLiteral("en") ? german.match(plain).hasMatch() : looksEnglish(text);
        if (leftover) left[plain].insert(kind);
    };
    QList<QObject *> objects = mainWindow.findChildren<QObject *>();
    for (QObject *object : objects) {
        if (auto *menu = qobject_cast<QMenu *>(object)) add(QStringLiteral("menu"), menu->title());
        if (auto *action = qobject_cast<QAction *>(object)) add(QStringLiteral("action"), action->text());
        if (auto *label = qobject_cast<QLabel *>(object)) add(QStringLiteral("label"), label->text());
        if (auto *button = qobject_cast<QAbstractButton *>(object)) add(QStringLiteral("button"), button->text());
        if (auto *group = qobject_cast<QGroupBox *>(object)) add(QStringLiteral("group"), group->title());
        if (auto *tabs = qobject_cast<QTabBar *>(object))
            for (int i = 0; i < tabs->count(); ++i) add(QStringLiteral("tab"), tabs->tabText(i));
        if (auto *widget = qobject_cast<QWidget *>(object)) add(QStringLiteral("tooltip"), widget->toolTip());
    }
    QTextStream out(stdout);
    out << "I18N-SWITCH to=" << code << " leftover=" << left.size() << "\n";
    for (auto it = left.cbegin(); it != left.cend(); ++it) {
        QStringList kinds = it.value().values(); kinds.sort();
        out << kinds.join(QLatin1Char(',')) << "\t" << it.key() << "\n";
    }
    return 0;
}

inline int run(QApplication &app, QWidget &mainWindow)
{
    const QString switchTo = qEnvironmentVariable("M3_I18N_SWITCH");
    if (!switchTo.isEmpty()) return runSwitch(app, mainWindow, switchTo);
    QMap<QString, QSet<QString>> found;
    app.processEvents();
    collect(&mainWindow, found);
    AppSettings settings;
    settings.load();
    AppSettingsDialog dialog(settings, QString(), QString(), {},
        [](const QString &source) { return M3I18n::text(source); },
        [](const QString &key, const QString &fallback) { return M3I18n::translate(key, fallback); },
        AppSettingsDialog::InitialPage::AudioPtt, nullptr);
    if (auto *tabs = dialog.findChild<QTabWidget *>())
        for (int page = 0; page < tabs->count(); ++page) {
            tabs->setCurrentIndex(page);   // pages are built when first shown
            app.processEvents();
        }
    collect(&dialog, found);
    QTextStream out(stdout);
    out << "I18N-REPORT language=" << M3I18n::languageCode() << " entries=" << found.size() << "\n";
    for (auto it = found.cbegin(); it != found.cend(); ++it) {
        QStringList kinds = it.value().values();
        kinds.sort();
        out << kinds.join(QLatin1Char(',')) << "\t" << it.key() << "\n";
    }
    return 0;
}

} // namespace M3I18nReport
