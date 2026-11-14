// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QAbstractScrollArea>
#include <QAbstractSpinBox>
#include <QApplication>
#include <QComboBox>
#include <QEvent>
#include <QObject>
#include <QPointer>
#include <QScrollBar>
#include <QWheelEvent>

// Settings pages scroll with the mouse wheel; Qt would otherwise change any
// combo box or spin box that passes under the pointer (a CAT port changed
// from ttyUSB0 to ttyS4 that way, 2026-11-14).  While installed, the wheel
// changes such a field only after it was clicked (has focus); otherwise it
// scrolls the surrounding page.
class WheelGuard final : public QObject
{
public:
    explicit WheelGuard(QWidget *scope) : QObject(scope), m_scope(scope)
    {
        qApp->installEventFilter(this);
    }
    ~WheelGuard() override { qApp->removeEventFilter(this); }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() != QEvent::Wheel || m_scope.isNull()) return false;
        auto *widget = qobject_cast<QWidget *>(watched);
        if (widget == nullptr || !m_scope->isAncestorOf(widget)) return false;
        QWidget *field = nullptr;
        for (QWidget *w = widget; w != nullptr && w != m_scope; w = w->parentWidget()) {
            if (qobject_cast<QComboBox *>(w) != nullptr || qobject_cast<QAbstractSpinBox *>(w) != nullptr) {
                field = w;
                break;
            }
        }
        if (field == nullptr || field->hasFocus()) return false;
        // Scroll the page under the field instead.
        for (QWidget *w = field->parentWidget(); w != nullptr; w = w->parentWidget()) {
            if (auto *area = qobject_cast<QAbstractScrollArea *>(w)) {
                const auto *wheel = static_cast<QWheelEvent *>(event);
                QScrollBar *bar = area->verticalScrollBar();
                if (bar != nullptr) bar->setValue(bar->value() - wheel->angleDelta().y() / 120 * bar->singleStep() * 3);
                break;
            }
        }
        return true;
    }

private:
    QPointer<QWidget> m_scope;
};
