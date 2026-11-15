#include "FaxImageWidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>

#include <cstring>
#include <QWheelEvent>

// -----------------------------------------------------------------------------
// Construction
// -----------------------------------------------------------------------------

FaxImageWidget::FaxImageWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(420, 260);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);
}

// -----------------------------------------------------------------------------
// Public getters
// -----------------------------------------------------------------------------

bool FaxImageWidget::fitToWindow() const
{
    return m_fitToWindow;
}

int FaxImageWidget::zoomPercent() const
{
    return m_zoomPercent;
}

// -----------------------------------------------------------------------------
// Public slots
// -----------------------------------------------------------------------------

void FaxImageWidget::setImage(const QImage &image)
{
    m_image = image;
    clampPanOffset();
    update();
}

void FaxImageWidget::clear()
{
    m_image = QImage();
    m_panOffset = QPoint();
    updatePanCursor();
    m_txProgressVisible = false;
    m_txProgress = 0.0;
    update();
}

void FaxImageWidget::setFitToWindow()
{
    if (m_fitToWindow && m_panOffset.isNull()) {
        return;
    }

    m_fitToWindow = true;
    m_panOffset = QPoint();
    updatePanCursor();
    emit zoomChanged(m_zoomPercent, true);
    update();
}

void FaxImageWidget::setActualSize()
{
    setZoomPercent(100);
}

void FaxImageWidget::zoomIn()
{
    const int nextZoom = qMin(800, m_fitToWindow ? 125 : m_zoomPercent + 25);
    setZoomPercent(nextZoom);
}

void FaxImageWidget::zoomOut()
{
    const int nextZoom = qMax(25, m_fitToWindow ? 75 : m_zoomPercent - 25);
    setZoomPercent(nextZoom);
}

void FaxImageWidget::setZoomPercent(int percent)
{
    const int boundedPercent = qBound(25, percent, 800);

    if (!m_fitToWindow && m_zoomPercent == boundedPercent) {
        return;
    }

    m_fitToWindow = false;
    m_zoomPercent = boundedPercent;
    clampPanOffset();
    emit zoomChanged(m_zoomPercent, false);
    update();
}

// -----------------------------------------------------------------------------
// QWidget events
// -----------------------------------------------------------------------------


void FaxImageWidget::setTransmitProgress(double progress)
{
    m_txProgressVisible = true;
    m_txProgress = qBound(0.0, progress, 1.0);
    update();
}

void FaxImageWidget::clearTransmitProgress()
{
    if (!m_txProgressVisible) {
        return;
    }

    m_txProgressVisible = false;
    m_txProgress = 0.0;
    update();
}

void FaxImageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), palette().window());

    if (!m_image.isNull()) {
        const QRect targetRect = imageTargetRect();
        painter.fillRect(targetRect, QColor(18, 18, 18));

        painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
        const int w = m_image.width();
        const int offset = w > 0 ? ((m_lineStartOffset % w) + w) % w : 0;
        if (offset == 0) {
            painter.drawImage(targetRect, m_image);
        } else {
            // Rotated rows in two pieces: [offset, w) first, then [0, offset).
            const double sx = double(targetRect.width()) / w;
            const QRectF first(targetRect.left(), targetRect.top(), (w - offset) * sx, targetRect.height());
            const QRectF second(first.right(), targetRect.top(), offset * sx, targetRect.height());
            painter.drawImage(first, m_image, QRectF(offset, 0, w - offset, m_image.height()));
            painter.drawImage(second, m_image, QRectF(0, 0, offset, m_image.height()));
        }

        if (m_txProgressVisible) {
            const int transmittedHeight = static_cast<int>(targetRect.height() * m_txProgress);

            if (transmittedHeight > 0) {
                const QRect sentRect(targetRect.left(),
                                     targetRect.top(),
                                     targetRect.width(),
                                     qMin(transmittedHeight, targetRect.height()));
                painter.fillRect(sentRect, QColor(0, 0, 0, 90));
            }

            const int y = targetRect.top() +
                          qBound(0, transmittedHeight, targetRect.height() - 1);
            painter.setPen(QPen(QColor(0, 255, 120), 2));
            painter.drawLine(targetRect.left(), y, targetRect.right(), y);
        }
    }

    painter.setPen(QColor(130, 150, 160));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
}

void FaxImageWidget::wheelEvent(QWheelEvent *event)
{
    if (m_image.isNull()) {
        event->ignore();
        return;
    }

    const int delta = event->angleDelta().y();

    if (delta > 0) {
        zoomIn();
    } else if (delta < 0) {
        zoomOut();
    }

    event->accept();
}

void FaxImageWidget::setLineStartOffset(int pixels)
{
    if (pixels == m_lineStartOffset) return;
    m_lineStartOffset = pixels;
    update();
}

QImage FaxImageWidget::rotateLines(const QImage &image, int pixels)
{
    const int w = image.width();
    const int offset = w > 0 ? ((pixels % w) + w) % w : 0;
    if (image.isNull() || offset == 0) return image;
    QImage out(image.size(), image.format());
    out.setColorTable(image.colorTable());
    if (image.depth() >= 8) {
        const int bpp = image.depth() / 8;
        for (int y = 0; y < image.height(); ++y) {
            const uchar *src = image.constScanLine(y);
            uchar *dst = out.scanLine(y);
            std::memcpy(dst, src + size_t(offset) * bpp, size_t(w - offset) * bpp);
            std::memcpy(dst + size_t(w - offset) * bpp, src, size_t(offset) * bpp);
        }
    } else {
        QPainter p(&out);
        p.drawImage(0, 0, image, offset, 0, w - offset, image.height());
        p.drawImage(w - offset, 0, image, 0, 0, offset, image.height());
    }
    return out;
}

void FaxImageWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && (event->modifiers() & Qt::ShiftModifier) && !m_image.isNull()) {
        const QRect target = imageTargetRect();
        if (target.contains(event->pos()) && target.width() > 0) {
            // Column in the displayed (already rotated) image -> received image.
            const int w = m_image.width();
            const int shown = qBound(0, int(double(event->pos().x() - target.left()) * w / target.width()), w - 1);
            const int offset = ((m_lineStartOffset % w) + w) % w;
            emit lineStartPicked((shown + offset) % w);
        }
        event->accept();
        return;
    }
    if (event->button() == Qt::LeftButton && canPan()) {
        m_panning = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }

    QWidget::mousePressEvent(event);
}

void FaxImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_panning) {
        const QPoint delta = event->pos() - m_lastMousePos;
        m_lastMousePos = event->pos();
        m_panOffset += delta;
        clampPanOffset();
        update();
        event->accept();
        return;
    }

    QWidget::mouseMoveEvent(event);
}

void FaxImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_panning) {
        m_panning = false;
        m_panCursorShown = false;
        unsetCursor();
        updatePanCursor();
        event->accept();
        return;
    }

    QWidget::mouseReleaseEvent(event);
}

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

QRect FaxImageWidget::imageTargetRect() const
{
    if (m_image.isNull()) {
        return QRect();
    }

    QSize targetSize;

    if (m_fitToWindow) {
        targetSize = m_image.size().scaled(size(), Qt::KeepAspectRatio);
    } else {
        targetSize = QSize(
            qMax(1, (m_image.width() * m_zoomPercent) / 100),
            qMax(1, (m_image.height() * m_zoomPercent) / 100)
            );
    }

    const QPoint topLeft(
        ((width() - targetSize.width()) / 2) + m_panOffset.x(),
        ((height() - targetSize.height()) / 2) + m_panOffset.y()
        );

    return QRect(topLeft, targetSize);
}

void FaxImageWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    clampPanOffset();
}

bool FaxImageWidget::canPan() const
{
    if (m_fitToWindow || m_image.isNull()) {
        return false;
    }
    return (m_image.width() * m_zoomPercent) / 100 > width() ||
           (m_image.height() * m_zoomPercent) / 100 > height();
}

void FaxImageWidget::updatePanCursor()
{
    const bool pan = canPan();
    if (m_panning || pan == m_panCursorShown) {
        return;
    }
    m_panCursorShown = pan;
    if (pan) setCursor(Qt::OpenHandCursor);
    else unsetCursor();
}

void FaxImageWidget::clampPanOffset()
{
    if (m_fitToWindow || m_image.isNull()) {
        m_panOffset = QPoint();
        updatePanCursor();
        return;
    }

    const QSize targetSize(
        qMax(1, (m_image.width() * m_zoomPercent) / 100),
        qMax(1, (m_image.height() * m_zoomPercent) / 100)
        );

    const int maxX = qMax(0, (targetSize.width() - width()) / 2);
    const int maxY = qMax(0, (targetSize.height() - height()) / 2);

    m_panOffset.setX(qBound(-maxX, m_panOffset.x(), maxX));
    m_panOffset.setY(qBound(-maxY, m_panOffset.y(), maxY));
    updatePanCursor();
}
