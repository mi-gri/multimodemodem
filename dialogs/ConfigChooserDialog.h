// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QString>
#include <QVector>

// Start menu (--startupmenu): lists the existing configurations and lets the
// operator pick one before anything reads the settings.
namespace M3ConfigChooser {

struct Entry {
    QString name;          // value for --config; empty = the default configuration
    QString label;         // shown in the list
    QString settingsFile;
    bool running = false;  // another instance holds its lock: not selectable
};

// Default configuration first, then configs/NAME.m3cfg sorted by name.
QVector<Entry> entries();

// Shows the dialog.  Returns false when it was cancelled (the program should
// end); otherwise *config is the name to use (empty = default).
bool choose(QString *config);

} // namespace M3ConfigChooser
