// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QString>
#include <functional>

class QWidget;

/**
 * @brief Legal and safety notice shown before Auto mode (Auto QSO) is enabled.
 *
 * Auto mode answers CQ calls and completes QSOs without the operator.  The
 * notice states that this may not be permitted everywhere, can disqualify a
 * contest entry, must not run unattended for long, and that the operator is
 * responsible for any damage.  It returns true only when the operator ticks
 * "I have read and understood ..." and confirms.
 */
namespace AutoModeNotice {
bool confirm(QWidget *parent, const std::function<QString(const QString &)> &translate);
}
