#pragma once
// Visible feedback for pressed buttons: every push/tool button flashes for a moment
// when it is pressed (mouse or Space/Enter); STOP buttons flash red, the others in the
// theme's highlight colour. A button can also be "latched" (STOP is active) and then
// stays red until the latch is removed. Works on top of any theme: the button's own
// style sheet is extended and restored afterwards.
#include <QObject>

class QAbstractButton;

namespace M3ButtonFeedback {
// Installs the application-wide filter once (safe to call several times).
void install();
// Marks a button as a STOP-like button (flashes red).
void markStop(QAbstractButton *button);
// Keeps a button highlighted red (e.g. "STOP is active"); tooltip is shown while latched.
void setLatched(QAbstractButton *button, bool latched, const QString &latchedToolTip = QString());
}
