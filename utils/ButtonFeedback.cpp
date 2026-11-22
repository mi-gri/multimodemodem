#include "ButtonFeedback.h"

#include <QAbstractButton>
#include <QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPalette>
#include <QPointer>
#include <QPushButton>
#include <QTimer>
#include <QToolButton>

namespace M3ButtonFeedback {
namespace {

const char *kRole = "m3FeedbackRole";        // "stop"
const char *kLatched = "m3FeedbackLatched";
const char *kBaseStyle = "m3FeedbackBase";   // the button's own style sheet without our additions
const char *kApplied = "m3FeedbackApplied";  // what we last applied
const char *kBaseTip = "m3FeedbackBaseTip";

QString redRule() { return QStringLiteral("background-color: #C62828; color: #FFFFFF; font-weight: bold;"); }

QString ruleFor(const QAbstractButton *button, bool flash)
{
    const bool stop = button->property(kRole).toString() == QLatin1String("stop");
    const bool latched = button->property(kLatched).toBool();
    QString body;
    if (latched || (flash && stop)) {
        body = redRule();
    } else if (flash) {
        const QColor c = button->palette().color(QPalette::Highlight);
        const QColor t = button->palette().color(QPalette::HighlightedText);
        body = QStringLiteral("background-color: %1; color: %2;").arg(c.name(), t.name());
    }
    if (body.isEmpty()) return QString();
    const QString selector = qobject_cast<const QToolButton *>(button) ? QStringLiteral("QToolButton") : QStringLiteral("QPushButton");
    return QStringLiteral(" %1 { %2 }").arg(selector, body);
}

void apply(QAbstractButton *button, bool flash)
{
    if (!button->property(kBaseStyle).isValid() || button->styleSheet() != button->property(kApplied).toString())
        button->setProperty(kBaseStyle, button->styleSheet());   // someone else changed it: that is the new base
    const QString base = button->property(kBaseStyle).toString();
    const QString combined = base + ruleFor(button, flash);
    button->setProperty(kApplied, combined);
    button->setStyleSheet(combined);
}

class Filter final : public QObject
{
public:
    using QObject::QObject;
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        QAbstractButton *button = nullptr;
        if (event->type() == QEvent::MouseButtonPress) {
            auto *me = static_cast<QMouseEvent *>(event);
            if (me->button() != Qt::LeftButton) return false;
            button = qobject_cast<QAbstractButton *>(watched);
        } else if (event->type() == QEvent::KeyPress) {
            auto *ke = static_cast<QKeyEvent *>(event);
            if (ke->isAutoRepeat() || (ke->key() != Qt::Key_Space && ke->key() != Qt::Key_Return && ke->key() != Qt::Key_Enter)) return false;
            button = qobject_cast<QAbstractButton *>(watched);
        }
        if (button == nullptr || !button->isEnabled() || !button->isVisible()) return false;
        if (!qobject_cast<QPushButton *>(button) && !qobject_cast<QToolButton *>(button)) return false;
        if (button->property("m3NoFeedback").toBool()) return false;
        apply(button, true);
        QPointer<QAbstractButton> guard(button);
        QTimer::singleShot(650, button, [guard]() {
            if (guard) apply(guard, false);
        });
        return false;   // never consumes the event
    }
};

} // namespace

void install()
{
    static bool installed = false;
    if (installed || qApp == nullptr) return;
    installed = true;
    qApp->installEventFilter(new Filter(qApp));
}

void markStop(QAbstractButton *button)
{
    if (button != nullptr) button->setProperty(kRole, QStringLiteral("stop"));
}

void setLatched(QAbstractButton *button, bool latched, const QString &latchedToolTip)
{
    if (button == nullptr) return;
    if (button->property(kLatched).toBool() == latched) return;
    button->setProperty(kLatched, latched);
    if (latched) {
        button->setProperty(kBaseTip, button->toolTip());
        if (!latchedToolTip.isEmpty()) button->setToolTip(latchedToolTip);
    } else if (button->property(kBaseTip).isValid()) {
        button->setToolTip(button->property(kBaseTip).toString());
    }
    apply(button, false);
}

} // namespace M3ButtonFeedback
