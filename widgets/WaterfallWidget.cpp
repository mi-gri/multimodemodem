#include "FrequencyScaleTicks.h"
#include "WaterfallWidget.h"
#include "CqSpots.h"
#include "../utils/SystemResourceManager.h"

#include <QDateTime>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QtGlobal>
#include <QtMath>

#include <cmath>
#include <cstring>
#include <cstddef>

// -----------------------------------------------------------------------------
// Construction
// -----------------------------------------------------------------------------

WaterfallWidget::WaterfallWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_chordClickTimer.setSingleShot(true);
    m_chordClickTimer.setInterval(140);
    connect(&m_chordClickTimer, &QTimer::timeout, this, &WaterfallWidget::emitPendingSingleClick);
    setMinimumHeight(160);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // The circular texture and all overlays reconstruct the complete frame.
    // Do not preserve the previous QOpenGLWidget framebuffer: with partial
    // updates, stale QPainter glyphs can survive outside a wrongly clipped GL
    // viewport and appear as fuzzy/ghosted labels on HiDPI displays.
    setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setMouseTracking(true);

    m_frequencyScrollBar = new QScrollBar(Qt::Horizontal, this);
    m_frequencyScrollBar->setObjectName(QStringLiteral("waterfallFrequencyScroll"));
    m_frequencyScrollBar->setRange(0, 10000);
    m_frequencyScrollBar->setSingleStep(120);
    m_frequencyScrollBar->setStyleSheet(QStringLiteral(
        "QScrollBar:horizontal { background:palette(window); height:12px; border:1px solid palette(mid); }"
        "QScrollBar::handle:horizontal { background:palette(highlight); min-width:24px; border:1px solid palette(dark); }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width:0px; }"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background:palette(base); }"));
    m_frequencyScrollBar->hide();
    // One spectrum column per physical pixel, once a resize has settled.
    m_columnsTimer.setSingleShot(true);
    m_columnsTimer.setInterval(300);
    connect(&m_columnsTimer, &QTimer::timeout, this, [this]() {
        if (width() > 0) emit displayColumnsWanted(qRound(width() * devicePixelRatioF()));
    });
    connect(m_frequencyScrollBar, &QScrollBar::valueChanged, this, [this](int value) {
        if (m_updatingFrequencyScrollBar || m_maxHz <= m_minHz) return;
        const double fullSpan = m_maxHz - m_minHz;
        const double viewSpan = visibleMaxHz() - visibleMinHz();
        const double movable = qMax(0.0, fullSpan - viewSpan);
        if (movable <= 0.0) return;
        const double start = m_minHz + movable *
            static_cast<double>(value) / 10000.0;
        m_viewMinHz = start;
        m_viewMaxHz = start + viewSpan;
        requestRepaint();
    });

    // Coalesce waterfall repaints to one GL-backed presentation per display
    // frame.  FFT/DSP lines may arrive faster than an old GPU/CPU can repaint;
    // drawing every line immediately causes UI lag without improving decode.
    m_repaintTimer.setSingleShot(true);
    connect(&m_repaintTimer, &QTimer::timeout, this, [this]() {
        m_repaintQueued = false;
        update();
    });

    buildColorTable();
    clear();
}

WaterfallWidget::~WaterfallWidget()
{
    if (context() != nullptr && context()->isValid()) {
        makeCurrent();
        destroyGpuRenderer();
        doneCurrent();
    }
}

// -----------------------------------------------------------------------------
// Public slots
// -----------------------------------------------------------------------------

void WaterfallWidget::addLine(const QVector<quint8> &line, double minHz, double maxHz)
{
    if (line.isEmpty()) {
        return;
    }

    const bool rangeChanged = std::abs(m_minHz - minHz) > 1.0e-6 ||
                              std::abs(m_maxHz - maxHz) > 1.0e-6;
    m_minHz = minHz;
    m_maxHz = maxHz;
    if (!m_viewInitialized || rangeChanged) {
        resetFrequencyZoom();
    }
    if (m_viewInitialized && rangeChanged) {
        // Rows of another audio range (WSPR 1200-1800 Hz vs 100-3000 Hz)
        // would be drawn at the wrong frequencies: start a fresh history.
        m_gpuClearPending = true;
        m_pendingGpuRows.clear();
        m_periodLabels.clear();
    }

    // QOpenGLWidget does not paint while the top-level window is minimized.
    // Presentation rows must not build up while no frame can be shown, but the
    // circular GL history itself must remain untouched: clearing/resetting the
    // ring on visibility changes or transient layout resizes makes the waterfall
    // repeatedly empty and refill (the visible "accordion" effect). Decoder
    // audio is independent and is never altered here.
    QWidget *topLevel = window();
    const bool presentationSuppressed = !isVisible() ||
        (topLevel != nullptr && topLevel->isMinimized());
    if (presentationSuppressed) {
        if (!m_presentationSuspended) {
            m_presentationSuspended = true;
            m_pendingGpuRows.clear();
            m_repaintTimer.stop();
            m_repaintQueued = false;
            emit runtimeDiagnostic(QStringLiteral(
                "Waterfall presentation suspended: window hidden/minimized; display-only FFT rows are discarded while GL history is preserved"));
        }
        return;
    }
    if (m_presentationSuspended) {
        m_presentationSuspended = false;
        m_pendingGpuRows.clear();
        emit runtimeDiagnostic(QStringLiteral(
            "Waterfall presentation resumed: existing GL history preserved; live rows continue from the current ring position"));
    }

    agePeriodLabels();
    const bool periodStart = m_scrollDirection == ScrollDirection::Down && takePeriodBoundary();

    if (m_scrollDirection == ScrollDirection::Down && !m_gpuFailed) {
        const int newWidth = qBound(2, line.size(), 16384);
        if (m_frequencyBins != newWidth) {
            m_frequencyBins = newWidth;
            m_gpuTextureNeedsRecreate = true;
            m_gpuClearPending = true;
            m_pendingGpuRows.clear();
            m_periodLabels.clear();
        }
        QByteArray row = rgbaRowForLine(line);
        if (periodStart) {
            // the period line: the row lightened towards grey, as in JTDX
            for (int i = 0; i + 2 < row.size(); i += 4) {
                for (int c = 0; c < 3; ++c) {
                    const int v = static_cast<unsigned char>(row[i + c]);
                    row[i + c] = static_cast<char>((v + 3 * 200) / 4);
                }
            }
        }
        if (!row.isEmpty()) {
            const int queueLimit = qBound(32, qMax(1, height() / 2), 128);
            while (m_pendingGpuRows.size() >= queueLimit) {
                m_pendingGpuRows.dequeue();
                ++m_droppedGpuRows;
            }
            m_pendingGpuRows.enqueue(std::move(row));
        }
        ageVerticalTextTrails();
        requestRepaint();
        return;
    }

    ensureImage(m_scrollDirection == ScrollDirection::Down ? line.size() : -1);

    if (m_image.isNull()) {
        return;
    }

    const int imageWidth = m_image.width();
    const int imageHeight = m_image.height();

    if (imageWidth <= 0 || imageHeight <= 0) {
        return;
    }

    const int lineLast = line.size() - 1;

    auto sampleLine = [&line, lineLast](double ratio) -> int {
        if (lineLast <= 0) {
            return static_cast<int>(line.first());
        }
        const double sourcePosition = qBound(0.0, ratio, 1.0) * static_cast<double>(lineLast);
        const int i0 = qBound(0, static_cast<int>(sourcePosition), lineLast);
        const int i1 = qBound(0, i0 + 1, lineLast);
        const double frac = qBound(0.0, sourcePosition - static_cast<double>(i0), 1.0);
        const int interpolated = static_cast<int>(
            ((1.0 - frac) * static_cast<double>(line[i0])) +
            (frac * static_cast<double>(line[i1]))
            );
        return interpolated;
    };

    if (m_scrollDirection == ScrollDirection::Right) {
        if (imageWidth > 1) {
            for (int y = 0; y < imageHeight; ++y) {
                QRgb *row = reinterpret_cast<QRgb *>(m_image.scanLine(y));
                std::memmove(row, row + 1, static_cast<size_t>(imageWidth - 1) * sizeof(QRgb));
            }
        }

        for (int y = 0; y < imageHeight; ++y) {
            const double ratio = 1.0 - (static_cast<double>(y) / qMax(1, imageHeight - 1));
            const int value = static_cast<int>(sampleLine(ratio));
            QRgb *row = reinterpret_cast<QRgb *>(m_image.scanLine(y));
            row[imageWidth - 1] = colorForIntensity(static_cast<quint8>(qBound(0, value, 255)));
        }

        ageVerticalTextTrails();
        requestRepaint();
        return;
    }

    const int bytesPerLine = m_image.bytesPerLine();

    if (imageHeight > 1) {
        std::memmove(
            m_image.bits(),
            m_image.bits() + bytesPerLine,
            static_cast<size_t>(bytesPerLine) * static_cast<size_t>(imageHeight - 1)
            );
    }

    QRgb *dst = reinterpret_cast<QRgb *>(m_image.scanLine(imageHeight - 1));

    for (int x = 0; x < imageWidth; ++x) {
        const double ratio = static_cast<double>(x) / qMax(1, imageWidth - 1);
        const int value = static_cast<int>(sampleLine(ratio));
        dst[x] = colorForIntensity(static_cast<quint8>(qBound(0, value, 255)));
        if (periodStart) {
            dst[x] = qRgb((qRed(dst[x]) + 600) / 4, (qGreen(dst[x]) + 600) / 4, (qBlue(dst[x]) + 600) / 4);
        }
    }

    ageVerticalTextTrails();
    requestRepaint();
}

void WaterfallWidget::clear()
{
    m_pendingGpuRows.clear();
    m_droppedGpuRows = 0;
    m_gpuWriteRow = 0;
    m_gpuClearPending = true;
    ensureImage();

    if (!m_image.isNull()) {
        m_image.fill(QColor(4, 6, 8));
    }

    m_verticalTextGlyphs.clear();
    m_verticalTrailLastLabelByStream.clear();
    m_periodLabels.clear();

    requestRepaint();
}

void WaterfallWidget::setNextRowUtcMs(qint64 utcMs)
{
    m_nextRowUtcMs = utcMs;
}

void WaterfallWidget::setPeriodMs(int periodMs)
{
    periodMs = qMax(0, periodMs);
    if (periodMs == m_periodMs) {
        return;
    }
    m_periodMs = periodMs;
    m_lastRowPeriod = -1;
    m_periodLabels.clear();
    requestRepaint();
}

bool WaterfallWidget::takePeriodBoundary()
{
    if (m_periodMs <= 0 || m_nextRowUtcMs <= 0) {
        return false;
    }
    const qint64 period = m_nextRowUtcMs / m_periodMs;
    const bool boundary = m_lastRowPeriod >= 0 && period != m_lastRowPeriod;
    m_lastRowPeriod = period;
    if (boundary) {
        PeriodLabel label;
        label.text = QDateTime::fromMSecsSinceEpoch(period * m_periodMs, Qt::UTC).toString(QStringLiteral("HH:mm:ss"));
        m_periodLabels.append(label);
    }
    return boundary;
}

void WaterfallWidget::agePeriodLabels()
{
    if (m_periodLabels.isEmpty()) {
        return;
    }
    const int maxAge = qMax(16, m_gpuTextureHeight > 0 ? m_gpuTextureHeight : plotHeight());
    QVector<PeriodLabel> alive;
    alive.reserve(m_periodLabels.size());
    for (PeriodLabel label : std::as_const(m_periodLabels)) {
        ++label.ageRows;
        if (label.ageRows < maxAge) alive.append(label);
    }
    m_periodLabels = alive;
}

int WaterfallWidget::plotHeight() const
{
    return qMax(1, height() - bottomScaleBandHeight());
}

void WaterfallWidget::drawPeriodLabels(QPainter &painter)
{
    if (m_periodLabels.isEmpty() || m_scrollDirection != ScrollDirection::Down) {
        return;
    }
    // The rows fill the plot above the frequency scale, the newest at the
    // bottom; a row's age therefore gives its height on the screen.
    const int rows = qMax(1, m_gpuTextureHeight > 0 && !m_gpuFailed ? m_gpuTextureHeight : plotHeight());
    const double rowPx = static_cast<double>(plotHeight()) / rows;
    QFont font = painter.font();
    font.setBold(false);
    font.setPointSize(8);
    const QFontMetrics fm(font);
    painter.save();
    painter.setFont(font);
    for (const PeriodLabel &label : std::as_const(m_periodLabels)) {
        const int y = qRound(plotHeight() - (label.ageRows + 0.5) * rowPx);
        if (y < fm.height() || y > plotHeight()) continue;
        const QRect r(4, y - fm.height() - 1, fm.horizontalAdvance(label.text) + 6, fm.height());
        painter.fillRect(r, QColor(0, 0, 0, 150));
        painter.setPen(QColor(230, 230, 230));
        painter.drawText(r, Qt::AlignCenter, label.text);
    }
    painter.restore();
}

void WaterfallWidget::setMarkers(const QVector<FrequencyMarker> &markers)
{
    m_markers = markers;
    requestRepaint();
}

void WaterfallWidget::setSkimmerSpots(const QVector<SkimmerSpot>& spots, bool cqOnly)
{
    m_skimmerOverlay.setSpots(spots, cqOnly);
    requestRepaint();
}
void WaterfallWidget::clearSkimmer()
{
    m_skimmerOverlay.clear();
    requestRepaint();
}

void WaterfallWidget::setTextOverlays(const QVector<WaterfallTextOverlay> &overlays)
{
    QVector<WaterfallTextOverlay> staticOverlays;
    staticOverlays.reserve(overlays.size());

    for (const WaterfallTextOverlay &overlay : overlays) {
        if (overlay.verticalTrail) {
            appendVerticalTextTrail(overlay);
        } else {
            staticOverlays.append(overlay);
        }
    }

    m_textOverlays = staticOverlays;
    requestRepaint();
}

void WaterfallWidget::clearTextOverlayStream(const QString &streamId)
{
    const QString key = streamId.trimmed();
    if (key.isEmpty()) {
        return;
    }

    m_verticalTrailLastLabelByStream.remove(key);
    QVector<VerticalTextGlyph> kept;
    kept.reserve(m_verticalTextGlyphs.size());
    for (const VerticalTextGlyph &glyph : m_verticalTextGlyphs) {
        if (glyph.streamId != key) {
            kept.append(glyph);
        }
    }
    m_verticalTextGlyphs = kept;
    requestRepaint();
}

void WaterfallWidget::clearVerticalTextTrails()
{
    m_verticalTextGlyphs.clear();
    m_verticalTrailLastLabelByStream.clear();
    requestRepaint();
}

void WaterfallWidget::setColorScalePercent(int percent)
{
    const int clamped = qBound(0, percent, 100);
    if (m_colorScalePercent == clamped) {
        return;
    }

    m_colorScalePercent = clamped;
    requestRepaint();
}


void WaterfallWidget::setPaletteName(const QString &name)
{
    QString normalized = name.trimmed().toLower();
    if (normalized == QStringLiteral("default") ||
        normalized == QStringLiteral("wsjt-x") ||
        normalized == QStringLiteral("wsjtx")) {
        normalized = QStringLiteral("wsjtx");
    }

    if (normalized != QStringLiteral("wsjtx") &&
        normalized != QStringLiteral("mshv") &&
        normalized != QStringLiteral("fldigi") &&
        normalized != QStringLiteral("raptor") &&
        normalized != QStringLiteral("grayscale") &&
        normalized != QStringLiteral("m3")) {
        normalized = QStringLiteral("m3");
    }

    if (m_paletteName == normalized) {
        return;
    }

    m_paletteName = normalized;
    buildColorTable();
    requestRepaint();
}


void WaterfallWidget::setScrollDirection(ScrollDirection direction)
{
    if (m_scrollDirection == direction) {
        return;
    }

    m_scrollDirection = direction;
    m_frequencyBins = 0;
    m_frequencyPanning = false;
    unsetCursor();
    resetFrequencyZoom();
    clear();
    updateFrequencyScrollBar();
}

void WaterfallWidget::setChordClickEnabled(bool enabled)
{
    if (m_chordClickEnabled == enabled) {
        return;
    }

    m_chordClickEnabled = enabled;
    m_chordClickTimer.stop();
    m_pendingSingleClick = false;
    m_pendingClickButton = Qt::NoButton;
}

// -----------------------------------------------------------------------------
// QOpenGLWidget events
// -----------------------------------------------------------------------------

void WaterfallWidget::initializeGL()
{
    initializeOpenGLFunctions();
    initializeGpuRenderer();
}

void WaterfallWidget::paintGL()
{
    const double presentationLatencyMs = m_repaintLatencyClock.isValid()
        ? static_cast<double>(m_repaintLatencyClock.nsecsElapsed()) / 1000000.0
        : 0.0;
    m_repaintLatencyClock.invalidate();
    const int queuedRowsBeforeUpload = m_pendingGpuRows.size();

    QElapsedTimer frameTimer;
    frameTimer.start();

    // QPainter and raw OpenGL must be mixed through beginNativePainting() /
    // endNativePainting().  Starting QPainter only after custom GL commands
    // leaves parts of the texture/sampler state visible to Qt's glyph cache on
    // some drivers.  The result is exactly the striped/repeated glyphs seen in
    // FT callouts, while simpler labels may still look normal.
    QPainter painter(this);
    painter.beginNativePainting();

    bool gpuDrawn = false;
    if (m_scrollDirection == ScrollDirection::Down && m_gpuReady && !m_gpuFailed) {
        ensureGpuTexture();
        if (m_gpuReady && m_gpuTexture != 0) {
            if (m_gpuClearPending) {
                clearGpuTexture();
            }
            uploadPendingGpuRows();
            drawGpuWaterfall();
            gpuDrawn = true;
        }
    }

    if (!gpuDrawn) {
        ensureImage();
        // QOpenGLWidget renders into a device-pixel framebuffer.  width()/height()
        // are logical pixels, so using them directly clips the GL layer on
        // HiDPI displays.
        const qreal dpr = devicePixelRatioF();
        glViewport(0, 0,
                   qMax(1, qRound(static_cast<qreal>(width()) * dpr)),
                   qMax(1, qRound(static_cast<qreal>(height()) * dpr)));
        glDisable(GL_SCISSOR_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClearColor(4.0f / 255.0f, 6.0f / 255.0f, 8.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    // Restore a neutral texture/pixel-store state before Qt resumes its own GL
    // paint engine.  endNativePainting() performs the full Qt-side reset.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    painter.endNativePainting();

    // Geometry remains pixel-aligned; text is rasterized for the active DPR.
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    if (!gpuDrawn) {
        painter.fillRect(rect(), QColor(4, 6, 8));
        if (!m_image.isNull()) {
            painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
            if (m_scrollDirection == ScrollDirection::Down) {
                painter.drawImage(QRectF(rect()), m_image, waterfallSourceRect());
            } else {
                painter.drawImage(rect(), m_image);
            }
        }
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    drawFrequencyScale(painter);
    drawPeriodLabels(painter);
    drawMarkers(painter);
    drawTextOverlays(painter);
    QVector<double> protectedFrequencies;
    for (const auto& marker : m_markers) protectedFrequencies.append(marker.frequencyHz);
    m_skimmerOverlay.draw(painter, QRect(0, 32, width(), qMax(0, height() - bottomScaleBandHeight() - 36)),
                            visibleMinHz(), visibleMaxHz(), protectedFrequencies);
    drawVerticalTextTrails(painter);
    painter.setPen(QColor(130, 150, 160));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
    painter.end();

    // Normal audio callbacks can deliver two adjacent FFT rows together.  The
    // ring renderer intentionally presents one row per frame at low queue depth,
    // so request the next frame instead of jumping two rows at once.
    if (!m_pendingGpuRows.isEmpty()) {
        requestRepaint();
    }

    const double frameMs = static_cast<double>(frameTimer.nsecsElapsed()) / 1000000.0;
    auto &resources = M3Runtime::SystemResourceManager::instance();
    resources.observeGuiFrame(presentationLatencyMs > 0.0 ? presentationLatencyMs : frameMs);
    resources.observeWaterfallFrame(frameMs, queuedRowsBeforeUpload, gpuDrawn);

    if (!m_gpuDiagnosticClock.isValid()) {
        m_gpuDiagnosticClock.start();
    }
    if (m_gpuDiagnosticClock.elapsed() >= 10000) {
        m_gpuDiagnosticClock.restart();
        const QString backend = gpuDrawn
            ? QStringLiteral("OpenGL circular texture")
            : QStringLiteral("CPU QImage fallback");
        emit runtimeDiagnostic(QStringLiteral("Waterfall runtime: %1, render %2 ms, presentation latency %3 ms, queued rows %4, dropped rows %5, texture %6x%7")
                                   .arg(backend)
                                   .arg(frameMs, 0, 'f', 2)
                                   .arg(presentationLatencyMs, 0, 'f', 2)
                                   .arg(queuedRowsBeforeUpload)
                                   .arg(m_droppedGpuRows)
                                   .arg(m_gpuTextureWidth)
                                   .arg(m_gpuTextureHeight));
    }
}


double WaterfallWidget::frequencyAtMouseEvent(const QMouseEvent *event) const
{
    if (event == nullptr) {
        return 0.0;
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPoint pos = event->position().toPoint();
#else
    const QPoint pos = event->pos();
#endif
    return (m_scrollDirection == ScrollDirection::Right)
               ? yToFrequency(pos.y())
               : xToFrequency(pos.x());
}

void WaterfallWidget::emitPendingSingleClick()
{
    if (!m_pendingSingleClick || m_pendingClickButton == Qt::NoButton) {
        return;
    }

    const double frequencyHz = m_pendingClickFrequencyHz;
    const Qt::MouseButton button = m_pendingClickButton;
    m_pendingSingleClick = false;
    m_pendingClickButton = Qt::NoButton;
    emit frequencyClicked(frequencyHz, button);
}

void WaterfallWidget::mousePressEvent(QMouseEvent *event)
{
    if (event == nullptr) return;

    if (m_scrollDirection == ScrollDirection::Down &&
        event->button() == Qt::MiddleButton) {
        m_frequencyPanning = true;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        m_panStartPos = event->position().toPoint();
#else
        m_panStartPos = event->pos();
#endif
        m_panStartMinHz = visibleMinHz();
        m_panStartMaxHz = visibleMaxHz();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }

    const Qt::MouseButton button = event->button();
    if (button != Qt::LeftButton && button != Qt::RightButton) {
        QOpenGLWidget::mousePressEvent(event);
        return;
    }

    const double frequencyHz = frequencyAtMouseEvent(event);

    if (m_chordClickEnabled) {
        const Qt::MouseButtons buttons = event->buttons();
        const bool chord = buttons.testFlag(Qt::LeftButton) && buttons.testFlag(Qt::RightButton);
        if (chord) {
            m_chordClickTimer.stop();
            m_pendingSingleClick = false;
            m_pendingClickButton = Qt::NoButton;
            emit frequencyChordClicked(frequencyHz);
            event->accept();
            return;
        }

        // If the previous button was already released, this is a fast sequential
        // click rather than a chord. Deliver the pending action before arming the
        // new one so quick A/B tuning never loses a click.
        if (m_pendingSingleClick) {
            m_chordClickTimer.stop();
            emitPendingSingleClick();
        }

        // Delay only while chord detection is enabled.  This avoids changing an
        // RX marker before a near-simultaneous second button establishes TX.
        m_pendingClickFrequencyHz = frequencyHz;
        m_pendingClickButton = button;
        m_pendingSingleClick = true;
        m_chordClickTimer.start();
        event->accept();
        return;
    }

    emit frequencyClicked(frequencyHz, button);
    event->accept();
}

void WaterfallWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event == nullptr || !m_frequencyPanning ||
        m_scrollDirection != ScrollDirection::Down) {
        QOpenGLWidget::mouseMoveEvent(event);
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPoint pos = event->position().toPoint();
#else
    const QPoint pos = event->pos();
#endif
    const int dx = pos.x() - m_panStartPos.x();
    const double span = m_panStartMaxHz - m_panStartMinHz;
    const double deltaHz = -static_cast<double>(dx) * span /
                           static_cast<double>(qMax(1, width()));
    setVisibleRange(m_panStartMinHz + deltaHz,
                    m_panStartMaxHz + deltaHz);
    event->accept();
}

void WaterfallWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event != nullptr && event->button() == Qt::MiddleButton &&
        m_frequencyPanning) {
        m_frequencyPanning = false;
        unsetCursor();
        event->accept();
        return;
    }
    QOpenGLWidget::mouseReleaseEvent(event);
}

void WaterfallWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event != nullptr && m_scrollDirection == ScrollDirection::Down &&
        event->button() == Qt::LeftButton) {
        m_chordClickTimer.stop();
        m_pendingSingleClick = false;
        m_pendingClickButton = Qt::NoButton;
        resetFrequencyZoom();
        event->accept();
        return;
    }
    QOpenGLWidget::mouseDoubleClickEvent(event);
}

void WaterfallWidget::wheelEvent(QWheelEvent *event)
{
    if (event == nullptr || m_scrollDirection != ScrollDirection::Down) {
        QOpenGLWidget::wheelEvent(event);
        return;
    }
    const QPoint delta = event->angleDelta();
    const double span = visibleMaxHz() - visibleMinHz();
    if ((event->modifiers() & Qt::ShiftModifier) ||
        std::abs(delta.x()) > std::abs(delta.y())) {
        const int wheel = delta.x() != 0 ? delta.x() : delta.y();
        panFrequency(-static_cast<double>(wheel) / 120.0 * 0.10 * span);
    } else if (delta.y() != 0) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        const int x = qRound(event->position().x());
#else
        const int x = event->pos().x();
#endif
        const double anchorHz = xToFrequency(x);
        const double steps = static_cast<double>(delta.y()) / 120.0;
        zoomAt(anchorHz, std::pow(1.22, -steps));
    }
    event->accept();
}

void WaterfallWidget::resizeGL(int width, int height)
{
    m_columnsTimer.start();
    if (width <= 0 || height <= 0) {
        m_image = QImage();
        return;
    }

    if (m_scrollDirection == ScrollDirection::Down && !m_gpuFailed) {
        // Keep the circular texture and write position stable across QWidget
        // resizes. Qt layouts can emit small/transient resizeGL() events even
        // while the application remains visible; recreating the texture here
        // cleared the history and made it refill repeatedly. The existing
        // texture is simply scaled to the new viewport.
    } else {
        const int targetWidth = m_scrollDirection == ScrollDirection::Down && m_frequencyBins > 0
            ? m_frequencyBins : width;
        if (m_image.isNull()) {
            m_image = QImage(targetWidth, height, QImage::Format_RGB32);
            m_image.fill(QColor(4, 6, 8));
        } else if (m_image.width() != targetWidth || m_image.height() != height) {
            m_image = m_image.scaled(targetWidth, height,
                                     Qt::IgnoreAspectRatio, Qt::FastTransformation);
        }
    }
    if (m_frequencyScrollBar != nullptr) {
        m_frequencyScrollBar->setGeometry(5, qMax(0, height - 17), qMax(20, width - 10), 12);
        m_frequencyScrollBar->raise();
    }
    updateFrequencyScrollBar();
}


// -----------------------------------------------------------------------------
// Drawing helpers
// -----------------------------------------------------------------------------

void WaterfallWidget::drawFrequencyScale(QPainter &painter)
{
    if (m_scrollDirection == ScrollDirection::Right) {
        const QColor gridColor(35, 50, 55, 145);
        const QColor bandBackground(0, 0, 0, 95);
        const QColor labelBackground(235, 235, 235, 225);
        const QColor labelBorder(80, 80, 80, 210);
        const QColor labelText(15, 15, 15);
        int previousBottom = -100;
        const int labelPaddingX = 4;
        const int labelPaddingY = 1;
        QFont labelFont = painter.font();
        labelFont.setBold(true);
        labelFont.setPointSize(7);
        painter.setFont(labelFont);
        const QFontMetrics fm(labelFont);
        const int sideWidth = rightScaleBandWidth();
        const int sideLeft = qMax(0, width() - sideWidth);
        painter.fillRect(QRect(sideLeft, 0, sideWidth, height()), bandBackground);
        auto ticks = FrequencyScaleTicks::values(m_minHz, m_maxHz, height()*2);
        std::sort(ticks.begin(), ticks.end(), [this](double a, double b) { return frequencyToY(a)<frequencyToY(b); });
        for (double freq : ticks) {
            const int y = frequencyToY(freq);
            painter.setPen(gridColor);
            painter.drawLine(0, y, qMax(0, sideLeft - 1), y);
            const QString label = QString::number(qRound(freq)) + " Hz";
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
            const int textWidth = fm.horizontalAdvance(label);
#else
            const int textWidth = fm.width(label);
#endif
            const int labelHeight = fm.height() + (labelPaddingY * 2);
            QRect labelRect(qMax(2, width() - textWidth - (labelPaddingX * 2) - 3),
                            qBound(2, y - labelHeight / 2, qMax(2, height() - labelHeight - 2)),
                            textWidth + (labelPaddingX * 2),
                            labelHeight);
            if (labelRect.top() <= previousBottom + 4) continue;
            previousBottom = labelRect.bottom();
            painter.fillRect(labelRect, labelBackground);
            painter.setPen(labelBorder);
            painter.drawRect(labelRect.adjusted(0, 0, -1, -1));
            painter.setPen(labelText);
            painter.drawText(labelRect.adjusted(labelPaddingX, labelPaddingY, -labelPaddingX, -labelPaddingY),
                             Qt::AlignVCenter | Qt::AlignLeft,
                             label);
        }
        return;
    }
    const QColor gridColor(35, 50, 55, 145);
    const QColor bandBackground(0, 0, 0, 95);
    const QColor labelBackground(235, 235, 235, 225);
    const QColor labelBorder(80, 80, 80, 210);
    const QColor labelText(15, 15, 15);

    int previousRight = -100;
    const int labelPaddingX = 4;
    const int labelPaddingY = 1;

    QFont labelFont = painter.font();
    labelFont.setBold(true);
    labelFont.setPointSize(7);
    painter.setFont(labelFont);

    const QFontMetrics fm(labelFont);
    const int labelHeight = fm.height() + (labelPaddingY * 2);
    const int bandHeight = bottomScaleBandHeight();
    const int bandTop = qMax(0, height() - bandHeight);

    painter.fillRect(QRect(0, bandTop, width(), bandHeight), bandBackground);

    for (double freq : FrequencyScaleTicks::values(visibleMinHz(), visibleMaxHz(), width())) {
        const int x = frequencyToX(freq);

        painter.setPen(gridColor);
        painter.drawLine(x, 0, x, qMax(0, bandTop - 1));

        const QString label = QString::number(qRound(freq)) + " Hz";

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
        const int textWidth = fm.horizontalAdvance(label);
#else
        const int textWidth = fm.width(label);
#endif

        int labelLeft = x + 3;

        if (labelLeft + textWidth + (labelPaddingX * 2) > width()) {
            labelLeft = width() - textWidth - (labelPaddingX * 2) - 3;
        }

        if (labelLeft < 2) {
            labelLeft = 2;
        }

        const int labelTop = bandTop + qMax(3, (bandHeight - labelHeight) / 2 - 4);
        const QRect labelRect(
            labelLeft,
            qBound(bandTop + 2, labelTop, qMax(bandTop + 2, height() - labelHeight - 6)),
            textWidth + (labelPaddingX * 2),
            labelHeight
            );

        if (labelRect.left() <= previousRight + 5) continue;
        previousRight = labelRect.right();
        painter.fillRect(labelRect, labelBackground);
        painter.setPen(labelBorder);
        painter.drawRect(labelRect.adjusted(0, 0, -1, -1));

        painter.setPen(labelText);
        painter.drawText(
            labelRect.adjusted(labelPaddingX, labelPaddingY, -labelPaddingX, -labelPaddingY),
            Qt::AlignVCenter | Qt::AlignLeft,
            label
            );
    }

    const double fullSpan = m_maxHz - m_minHz;
    const double viewSpan = visibleMaxHz() - visibleMinHz();
    if (fullSpan > 0.0 && viewSpan > 0.0 && viewSpan < fullSpan - 0.5) {
        const double zoom = fullSpan / viewSpan;
        const QString zoomLabel = QStringLiteral("×%1  %2-%3 Hz")
            .arg(zoom, 0, 'f', zoom < 10.0 ? 1 : 0)
            .arg(qRound(visibleMinHz()))
            .arg(qRound(visibleMaxHz()));
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
        const int zoomWidth = fm.horizontalAdvance(zoomLabel);
#else
        const int zoomWidth = fm.width(zoomLabel);
#endif
        const QRect zoomRect(qMax(2, width() - zoomWidth - 12),
                             bandTop + 2,
                             zoomWidth + 8,
                             labelHeight);
        painter.fillRect(zoomRect, QColor(8, 10, 11, 220));
        painter.setPen(QColor(212, 154, 66));
        painter.drawRect(zoomRect.adjusted(0, 0, -1, -1));
        painter.drawText(zoomRect.adjusted(4, 0, -4, 0),
                         Qt::AlignVCenter | Qt::AlignRight,
                         zoomLabel);
    }
}

namespace {
// Marker label: a tag in the marker's own colour with black or white text,
// whichever contrasts more.  Coloured text on a light box was unreadable for
// light colours (yellow Mark, cyan Space).
void drawMarkerLabel(QPainter &painter, const QRect &labelRect, int paddingX, int paddingY,
                     const QColor &color, const QString &text)
{
    QColor fill = color;
    fill.setAlpha(235);
    const double luminance = 0.2126 * color.redF() + 0.7152 * color.greenF() + 0.0722 * color.blueF();
    const QColor textColor = luminance > 0.5 ? QColor(0, 0, 0) : QColor(255, 255, 255);
    painter.fillRect(labelRect, fill);
    painter.setPen(color.darker(170));
    painter.drawRect(labelRect.adjusted(0, 0, -1, -1));
    painter.setPen(textColor);
    painter.drawText(labelRect.adjusted(paddingX, paddingY, -paddingX, -paddingY),
                     Qt::AlignVCenter | Qt::AlignLeft, text);
}
} // namespace

void WaterfallWidget::drawMarkers(QPainter &painter)
{
    if (m_markers.isEmpty()) {
        return;
    }

    if (m_scrollDirection == ScrollDirection::Right) {
        QFont markerFont = painter.font();
        markerFont.setBold(true);
        markerFont.setPointSize(8);
        painter.setFont(markerFont);
        const QFontMetrics fm(markerFont);
        const int labelPaddingX = 4;
        const int labelPaddingY = 1;
        const int labelHeight = fm.height() + (labelPaddingY * 2);
        int visibleLabelIndex = 0;
        for (const FrequencyMarker &marker : m_markers) {
            if (marker.frequencyHz < m_minHz || marker.frequencyHz > m_maxHz) {
                continue;
            }
            const int y = frequencyToY(marker.frequencyHz);
            QPen markerPen(marker.color);
            markerPen.setWidth(qMax(1, marker.width));
            if (marker.dashed) {
                markerPen.setStyle(Qt::DashLine);
            }
            painter.setPen(markerPen);
            painter.drawLine(0, y, qMax(0, width() - rightScaleBandWidth() - 1), y);
            if (!marker.label.trimmed().isEmpty()) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
                const int textWidth = fm.horizontalAdvance(marker.label);
#else
                const int textWidth = fm.width(marker.label);
#endif
                const int labelLeft = 4 + ((visibleLabelIndex % 2) * (textWidth + 14));
                QRect labelRect(labelLeft,
                                qBound(2, y - labelHeight - 2, qMax(2, height() - labelHeight - 2)),
                                textWidth + (labelPaddingX * 2),
                                labelHeight);
                drawMarkerLabel(painter, labelRect, labelPaddingX, labelPaddingY, marker.color, marker.label);
                ++visibleLabelIndex;
            }
        }
        return;
    }

    QFont markerFont = painter.font();
    markerFont.setBold(true);
    markerFont.setPointSize(8);
    painter.setFont(markerFont);

    const QFontMetrics fm(markerFont);

    const int labelPaddingX = 4;
    const int labelPaddingY = 1;
    const int labelHeight = fm.height() + (labelPaddingY * 2);

    int visibleLabelIndex = 0;

    for (const FrequencyMarker &marker : m_markers) {
        const double bw = qMax(0.0, marker.bandwidthHz);
        const double lowHz = marker.centred ? marker.frequencyHz - bw / 2.0 : marker.frequencyHz;
        const double highHz = lowHz + bw;
        if (highHz < visibleMinHz() || lowHz > visibleMaxHz()) {
            continue;
        }

        int x = frequencyToX(marker.frequencyHz);

        QPen markerPen(marker.color);
        markerPen.setWidth(qMax(1, marker.width));
        if (marker.dashed) {
            markerPen.setStyle(Qt::DashLine);
        }
        const int plotBottom = qMax(0, height() - bottomScaleBandHeight() - 1);
        if (bw > 0.0) {
            // The band the signal occupies: a light tint between two edge
            // lines, so the signals in it stay visible, and a solid bar on the
            // frequency scale, like the RX/TX brackets of JTDX/WSJT-X.
            const int x1 = frequencyToX(lowHz);
            const int x2 = qMax(x1 + 2, frequencyToX(highHz));
            QColor tint = marker.color;
            tint.setAlpha(34);
            painter.fillRect(QRect(x1, 0, x2 - x1 + 1, plotBottom), tint);
            QPen edgePen = markerPen;
            edgePen.setWidth(1);
            painter.setPen(edgePen);
            painter.drawLine(x1, 0, x1, plotBottom);
            painter.drawLine(x2, 0, x2, plotBottom);
            painter.fillRect(QRect(x1, plotBottom + 1, x2 - x1 + 1, 4), marker.color);
            x = x2;   // the label goes beside the band
        } else {
            painter.setPen(markerPen);
            painter.drawLine(x, 0, x, plotBottom);
        }

        if (!marker.label.trimmed().isEmpty()) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
            const int textWidth = fm.horizontalAdvance(marker.label);
#else
            const int textWidth = fm.width(marker.label);
#endif

            int labelLeft = x + 4;

            if (labelLeft + textWidth + (labelPaddingX * 2) > width()) {
                labelLeft = x - textWidth - (labelPaddingX * 2) - 4;
            }

            if (labelLeft < 2) {
                labelLeft = 2;
            }

            const int plotBottom = qMax(2, height() - bottomScaleBandHeight() - 2);
            const int labelBottomOffset = 4 + ((visibleLabelIndex % 2) * (labelHeight + 2));
            const int labelTop = qMax(2, plotBottom - labelBottomOffset - labelHeight);

            const QRect labelRect(
                labelLeft,
                labelTop,
                textWidth + (labelPaddingX * 2),
                labelHeight
                );

            drawMarkerLabel(painter, labelRect, labelPaddingX, labelPaddingY, marker.color, marker.label);

            ++visibleLabelIndex;
        }
    }
}

void WaterfallWidget::drawTextOverlays(QPainter &painter)
{
    if (m_textOverlays.isEmpty()) {
        return;
    }

    // FT callouts (CQ calls, replies to me), as in JTDX 3box: painted over the
    // waterfall's bottom edge - the newest rows, where the decoded signals
    // still are - each over its signal: a bracket as wide as the signal on
    // the bottom line, the call in a label above it. Where signals lie close
    // together the labels go in rows above one another (CqSpots.h), joined to
    // their brackets by a thin line. A call for which no row is left keeps its
    // bracket, and the top right corner counts them: "+3".
    QFont overlayFont = painter.font();
    overlayFont.setBold(false);
    overlayFont.setPointSize(8);   // small: the narrower the labels, the more of them fit
    const QFontMetrics fm(overlayFont);
    const int rowH = fm.height() + 2;
    const int base = qMax(0, height() - bottomScaleBandHeight() - 3);   // the line the brackets stand on
    if (base < rowH + 8) {
        return;
    }
    const int rows = qMax(1, qMin(cq_spots::max_rows, base / 2 / rowH));   // at most half the height

    struct Shown {
        int x1;
        int x2;
        const WaterfallTextOverlay *overlay;
    };
    std::vector<Shown> shown;
    std::vector<cq_spots::Box> boxes;
    for (const WaterfallTextOverlay &overlay : m_textOverlays) {
        const double bw = qMax(0.0, overlay.bandwidthHz);
        if (overlay.label.trimmed().isEmpty() ||
            overlay.frequencyHz + bw < visibleMinHz() || overlay.frequencyHz > visibleMaxHz()) {
            continue;
        }
        Shown s;
        s.x1 = frequencyToX(overlay.frequencyHz);
        s.x2 = frequencyToX(overlay.frequencyHz + bw);
        s.overlay = &overlay;
        shown.push_back(s);
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
        const int textWidth = fm.horizontalAdvance(overlay.label);
#else
        const int textWidth = fm.width(overlay.label);
#endif
        boxes.push_back(cq_spots::Box {(s.x1 + s.x2) / 2, textWidth + 4});
    }
    if (shown.empty()) {
        return;
    }
    const std::vector<cq_spots::Slot> placed = cq_spots::stack(boxes, width(), rows, 2);

    painter.save();
    painter.setFont(overlayFont);
    auto withAlpha = [](QColor c, int alpha) {
        c.setAlpha(qBound(0, c.alpha() * alpha / 255, 255));
        return c;
    };
    for (std::size_t i = 0; i < shown.size(); ++i) {   // brackets, and the lines up to labels in upper rows
        const Shown &s = shown[i];
        painter.setPen(QPen(withAlpha(s.overlay->textColor, s.overlay->alpha), 1));
        painter.drawLine(s.x1, base, s.x2, base);
        painter.drawLine(s.x1, base - 3, s.x1, base);
        painter.drawLine(s.x2, base - 3, s.x2, base);
        if (placed[i].row > 0) {
            painter.drawLine(boxes[i].centre, base - 3, boxes[i].centre, base - 4 - placed[i].row * rowH);
        }
    }
    int hidden = 0;
    for (std::size_t i = 0; i < shown.size(); ++i) {   // the labels, over the lines
        if (placed[i].row < 0) {
            ++hidden;
            continue;
        }
        const WaterfallTextOverlay &o = *shown[i].overlay;
        const int bottom = base - 4 - placed[i].row * rowH;
        const QRect r(placed[i].left, bottom - (rowH - 2), boxes[i].width, rowH - 2);
        painter.fillRect(r, withAlpha(o.backgroundColor, o.alpha));
        painter.setPen(withAlpha(o.textColor, o.alpha));
        painter.drawText(r, Qt::AlignCenter, o.label);
    }
    if (hidden > 0) {   // calls with no room: a count, top right of the waterfall
        const QString more = QStringLiteral("+%1").arg(hidden);
        const int w = fm.boundingRect(more).width() + 6;
        const QRect r(width() - w - 6, 34, w, rowH - 2);   // under the marker captions
        painter.fillRect(r, QColor(0, 0, 0, 170));
        painter.setPen(QColor(255, 220, 0));
        painter.drawText(r, Qt::AlignCenter, more);
    }
    painter.restore();
}


void WaterfallWidget::drawVerticalTextTrails(QPainter &painter)
{
    if (m_verticalTextGlyphs.isEmpty()) {
        return;
    }

    if (m_scrollDirection != ScrollDirection::Down) {
        return;
    }

    QFont trailFont = painter.font();
    trailFont.setBold(true);
    trailFont.setPointSize(9);
    painter.setFont(trailFont);

    const QFontMetrics fm(trailFont);
    const int paddingX = 4;
    const int paddingY = 1;
    const int charStep = qMax(12, fm.height() + 2);
    const int labelHeight = fm.height() + (paddingY * 2);
    const int plotBottom = qMax(0, height() - bottomScaleBandHeight() - 4);

    QVector<QRect> occupied;
    for (const VerticalTextGlyph &glyph : m_verticalTextGlyphs) {
        if (glyph.text.isEmpty() || glyph.frequencyHz < visibleMinHz() || glyph.frequencyHz > visibleMaxHz()) {
            continue;
        }

        const int x = frequencyToX(glyph.frequencyHz);
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
        const int textWidth = fm.horizontalAdvance(glyph.text);
#else
        const int textWidth = fm.width(glyph.text);
#endif
        const int labelWidth = textWidth + (paddingX * 2);

        // RTTY's single live stream belongs exactly between its Mark and Space
        // markers. Other possible streams remain offset beside their tone so
        // they do not cover the carrier trace itself.
        const bool centerOnFrequency = (glyph.streamId == QStringLiteral("rtty-live"));
        int left = centerOnFrequency ? (x - (labelWidth / 2)) : (x + 9);
        if (left + labelWidth + 2 > width()) {
            left = x - labelWidth - 9;
        }
        left = qBound(2, left, qMax(2, width() - labelWidth - 2));

        int top = plotBottom - glyph.ageRows - (glyph.sequenceIndex * charStep) - labelHeight;
        if (top + labelHeight < 0 || top > plotBottom) {
            continue;
        }

        QRect rect(left, top, labelWidth, labelHeight);

        // Keep the glyph beside its stream frequency, but step it sideways by a
        // few pixels if two streams collide. There is no flashing/toggling:
        // once a glyph is born it simply rides upward with the waterfall pixels.
        int guard = 0;
        while (guard < 5) {
            bool overlaps = false;
            for (const QRect &used : occupied) {
                if (rect.adjusted(-2, -1, 2, 1).intersects(used)) {
                    overlaps = true;
                    break;
                }
            }
            if (!overlaps) {
                break;
            }
            rect.translate(labelWidth + 3, 0);
            if (rect.right() >= width() - 2) {
                rect.moveLeft(qMax(2, x - labelWidth - 9 - (guard * (labelWidth + 3))));
            }
            ++guard;
        }
        occupied.append(rect);

        painter.fillRect(rect, glyph.backgroundColor);
        painter.setPen(QColor(255, 255, 255, 150));
        painter.drawRect(rect.adjusted(0, 0, -1, -1));

        const QRect textRect = rect.adjusted(paddingX, paddingY, -paddingX, -paddingY);
        // High-contrast cockpit OSD: draw a tiny black halo first, then the
        // bright glyph. This keeps live decoder characters readable on both
        // blue/green traces and the black waterfall background without blinking.
        painter.setPen(QColor(0, 0, 0, 230));
        painter.drawText(textRect.translated(1, 1), Qt::AlignCenter, glyph.text);
        painter.setPen(glyph.textColor.isValid() ? glyph.textColor : QColor(255, 244, 170));
        painter.drawText(textRect,
                         Qt::AlignCenter,
                         glyph.text);
    }
}

void WaterfallWidget::ageVerticalTextTrails()
{
    if (m_verticalTextGlyphs.isEmpty()) {
        return;
    }

    const int maxAge = qMax(220, height() + 80);
    for (VerticalTextGlyph &glyph : m_verticalTextGlyphs) {
        ++glyph.ageRows;
    }

    QVector<VerticalTextGlyph> alive;
    alive.reserve(m_verticalTextGlyphs.size());
    for (const VerticalTextGlyph &glyph : m_verticalTextGlyphs) {
        if (glyph.ageRows + (glyph.sequenceIndex * 18) < maxAge) {
            alive.append(glyph);
        }
    }
    m_verticalTextGlyphs = alive;
}

void WaterfallWidget::appendVerticalTextTrail(const WaterfallTextOverlay &overlay)
{
    if (overlay.label.trimmed().isEmpty() ||
        overlay.frequencyHz < m_minHz || overlay.frequencyHz > m_maxHz) {
        return;
    }

    QString key = overlay.streamId.trimmed();
    if (key.isEmpty()) {
        key = QStringLiteral("freq:%1").arg(qRound(overlay.frequencyHz));
    }

    QString current = overlay.label;
    current.replace(QChar('\r'), QChar(' '));
    current.replace(QChar('\n'), QChar(' '));
    current = current.simplified();
    if (current.isEmpty()) {
        return;
    }

    QString delta = newOverlaySuffix(key, current);
    if (delta.isEmpty()) {
        return;
    }

    // A late decoder may commit a short word/chunk at once.  Do not dump a whole
    // stale rolling buffer onto the waterfall; only the newest tail is rendered
    // as time-locked glyphs.
    constexpr int kMaxBurstChars = 12;
    if (delta.size() > kMaxBurstChars) {
        delta = delta.right(kMaxBurstChars);
    }

    const int glyphCount = delta.size();
    for (int i = 0; i < glyphCount; ++i) {
        const QChar ch = delta.at(i);
        if (ch.isSpace()) {
            continue;
        }

        VerticalTextGlyph glyph;
        glyph.frequencyHz = overlay.frequencyHz;
        glyph.text = QString(ch);
        glyph.textColor = overlay.textColor;
        glyph.backgroundColor = overlay.backgroundColor;
        glyph.streamId = key;
        glyph.ageRows = 0;
        // Older characters in the same committed burst are placed higher;
        // later characters then appear underneath as the stream grows.
        glyph.sequenceIndex = qMax(0, glyphCount - 1 - i);
        m_verticalTextGlyphs.append(glyph);
    }

    constexpr int kMaxGlyphs = 256;
    if (m_verticalTextGlyphs.size() > kMaxGlyphs) {
        m_verticalTextGlyphs = m_verticalTextGlyphs.mid(m_verticalTextGlyphs.size() - kMaxGlyphs);
    }
}

QString WaterfallWidget::newOverlaySuffix(const QString &key, const QString &currentLabel)
{
    const QString previous = m_verticalTrailLastLabelByStream.value(key);
    m_verticalTrailLastLabelByStream.insert(key, currentLabel);

    if (currentLabel == previous) {
        return QString();
    }

    if (previous.isEmpty()) {
        return currentLabel.right(qMin(3, currentLabel.size()));
    }

    if (currentLabel.startsWith(previous)) {
        return currentLabel.mid(previous.size());
    }

    if (previous.contains(currentLabel)) {
        return QString();
    }

    int bestOverlap = 0;
    const int maxOverlap = qMin(previous.size(), currentLabel.size());
    for (int len = 1; len <= maxOverlap; ++len) {
        if (previous.right(len) == currentLabel.left(len)) {
            bestOverlap = len;
        }
    }

    if (bestOverlap > 0) {
        return currentLabel.mid(bestOverlap);
    }

    // Rolling text was probably trimmed/reset.  Keep only a small tail to avoid
    // a visible burst of old text.
    return currentLabel.right(qMin(4, currentLabel.size()));
}

int WaterfallWidget::bottomScaleBandHeight() const
{
    // Keep the frequency scale in a reserved, readable bottom band.
    // Some fullscreen/window-manager combinations clip the last few GL pixels;
    // a taller band keeps the Hz labels away from the widget border in every mode.
    return 50;
}

int WaterfallWidget::rightScaleBandWidth() const
{
    return 66;
}

double WaterfallWidget::visibleMinHz() const
{
    return m_viewInitialized ? m_viewMinHz : m_minHz;
}

double WaterfallWidget::visibleMaxHz() const
{
    return m_viewInitialized ? m_viewMaxHz : m_maxHz;
}

void WaterfallWidget::resetFrequencyZoom()
{
    m_viewMinHz = m_minHz;
    m_viewMaxHz = m_maxHz;
    m_viewInitialized = true;
    updateFrequencyScrollBar();
    requestRepaint();
}

void WaterfallWidget::setVisibleRange(double minHz, double maxHz)
{
    if (m_maxHz <= m_minHz) return;
    const double fullSpan = m_maxHz - m_minHz;
    double span = qBound(qMax(20.0, fullSpan / 24.0),
                         maxHz - minHz, fullSpan);
    double start = minHz;
    if (start < m_minHz) start = m_minHz;
    if (start + span > m_maxHz) start = m_maxHz - span;
    m_viewMinHz = start;
    m_viewMaxHz = start + span;
    m_viewInitialized = true;
    updateFrequencyScrollBar();
    requestRepaint();
}

void WaterfallWidget::zoomAt(double anchorHz, double factor)
{
    if (m_maxHz <= m_minHz) return;
    const double oldMin = visibleMinHz();
    const double oldMax = visibleMaxHz();
    const double oldSpan = oldMax - oldMin;
    const double fullSpan = m_maxHz - m_minHz;
    const double minSpan = qMax(20.0, fullSpan / 24.0);
    const double newSpan = qBound(minSpan, oldSpan * factor, fullSpan);
    const double anchorRatio = oldSpan > 0.0
        ? qBound(0.0, (anchorHz - oldMin) / oldSpan, 1.0) : 0.5;
    setVisibleRange(anchorHz - anchorRatio * newSpan,
                    anchorHz + (1.0 - anchorRatio) * newSpan);
}

void WaterfallWidget::panFrequency(double deltaHz)
{
    setVisibleRange(visibleMinHz() + deltaHz,
                    visibleMaxHz() + deltaHz);
}

void WaterfallWidget::updateFrequencyScrollBar()
{
    if (m_frequencyScrollBar == nullptr || m_maxHz <= m_minHz) return;
    const double fullSpan = m_maxHz - m_minHz;
    const double viewSpan = visibleMaxHz() - visibleMinHz();
    const bool zoomed = viewSpan < fullSpan - 0.5;
    m_frequencyScrollBar->setVisible(zoomed &&
        m_scrollDirection == ScrollDirection::Down);
    if (!zoomed) return;

    const double movable = qMax(1.0e-9, fullSpan - viewSpan);
    const int value = qBound(0, qRound(10000.0 *
        (visibleMinHz() - m_minHz) / movable), 10000);
    m_updatingFrequencyScrollBar = true;
    m_frequencyScrollBar->setPageStep(qBound(1,
        qRound(10000.0 * viewSpan / fullSpan), 10000));
    m_frequencyScrollBar->setValue(value);
    m_updatingFrequencyScrollBar = false;
}

QRectF WaterfallWidget::waterfallSourceRect() const
{
    if (m_image.isNull() || m_maxHz <= m_minHz) return QRectF();
    const double fullSpan = m_maxHz - m_minHz;
    const double leftRatio = qBound(0.0,
        (visibleMinHz() - m_minHz) / fullSpan, 1.0);
    const double rightRatio = qBound(0.0,
        (visibleMaxHz() - m_minHz) / fullSpan, 1.0);
    const double left = leftRatio * static_cast<double>(m_image.width() - 1);
    const double right = rightRatio * static_cast<double>(m_image.width() - 1);
    return QRectF(left, 0.0, qMax(1.0, right - left + 1.0), m_image.height());
}

int WaterfallWidget::frequencyToX(double frequencyHz) const
{
    const double minHz = visibleMinHz();
    const double maxHz = visibleMaxHz();
    if (maxHz <= minHz) return 0;
    const double ratio = (frequencyHz - minHz) / (maxHz - minHz);
    return qBound(0,
        static_cast<int>(ratio * static_cast<double>(width() - 1)),
        qMax(0, width() - 1));
}

int WaterfallWidget::frequencyToY(double frequencyHz) const
{
    if (m_maxHz <= m_minHz) return height() - 1;
    const double ratio = (frequencyHz - m_minHz) / (m_maxHz - m_minHz);
    return qBound(0,
        height() - 1 - static_cast<int>(ratio * static_cast<double>(height() - 1)),
        qMax(0, height() - 1));
}

double WaterfallWidget::yToFrequency(int y) const
{
    if (height() <= 1 || m_maxHz <= m_minHz) return m_minHz;
    const double ratio = 1.0 - (static_cast<double>(qBound(0, y, height() - 1)) /
                                static_cast<double>(height() - 1));
    return m_minHz + (ratio * (m_maxHz - m_minHz));
}

double WaterfallWidget::xToFrequency(int x) const
{
    const double minHz = visibleMinHz();
    const double maxHz = visibleMaxHz();
    if (width() <= 1 || maxHz <= minHz) return minHz;
    const double ratio = static_cast<double>(qBound(0, x, width() - 1)) /
                         static_cast<double>(width() - 1);
    return minHz + (ratio * (maxHz - minHz));
}

void WaterfallWidget::initializeGpuRenderer()
{
    m_gpuReady = false;
    m_gpuFailed = false;

    const bool openGles = context() != nullptr && context()->isOpenGLES();
    const QSurfaceFormat format = context() != nullptr ? context()->format() : QSurfaceFormat();
    const bool desktopCore = !openGles && format.profile() == QSurfaceFormat::CoreProfile;

    QByteArray vertexShader;
    QByteArray fragmentShader;
    if (openGles) {
        vertexShader = QByteArrayLiteral(
            "#version 100\n"
            "attribute highp vec2 a_position;\n"
            "attribute highp vec2 a_texCoord;\n"
            "varying highp vec2 v_texCoord;\n"
            "void main() {\n"
            "  v_texCoord = a_texCoord;\n"
            "  gl_Position = vec4(a_position, 0.0, 1.0);\n"
            "}\n");
        fragmentShader = QByteArrayLiteral(
            "#version 100\n"
            "precision highp float;\n"
            "uniform sampler2D u_texture;\n"
            "uniform highp float u_x0;\n"
            "uniform highp float u_x1;\n"
            "uniform highp float u_ringOffset;\n"
            "uniform highp float u_invHeight;\n"
            "varying highp vec2 v_texCoord;\n"
            "void main() {\n"
            "  highp float tx = mix(u_x0, u_x1, v_texCoord.x);\n"
            "  highp float ty = fract(u_ringOffset + v_texCoord.y * (1.0 - u_invHeight));\n"
            "  ty = fract(ty + 0.5 * u_invHeight);\n"
            "  gl_FragColor = texture2D(u_texture, vec2(tx, ty));\n"
            "}\n");
    } else if (desktopCore) {
        vertexShader = QByteArrayLiteral(
            "#version 150\n"
            "in vec2 a_position;\n"
            "in vec2 a_texCoord;\n"
            "out vec2 v_texCoord;\n"
            "void main() {\n"
            "  v_texCoord = a_texCoord;\n"
            "  gl_Position = vec4(a_position, 0.0, 1.0);\n"
            "}\n");
        fragmentShader = QByteArrayLiteral(
            "#version 150\n"
            "uniform sampler2D u_texture;\n"
            "uniform float u_x0;\n"
            "uniform float u_x1;\n"
            "uniform float u_ringOffset;\n"
            "uniform float u_invHeight;\n"
            "in vec2 v_texCoord;\n"
            "out vec4 fragColor;\n"
            "void main() {\n"
            "  float tx = mix(u_x0, u_x1, v_texCoord.x);\n"
            "  float ty = fract(u_ringOffset + v_texCoord.y * (1.0 - u_invHeight));\n"
            "  ty = fract(ty + 0.5 * u_invHeight);\n"
            "  fragColor = texture(u_texture, vec2(tx, ty));\n"
            "}\n");
    } else {
        vertexShader = QByteArrayLiteral(
            "attribute vec2 a_position;\n"
            "attribute vec2 a_texCoord;\n"
            "varying vec2 v_texCoord;\n"
            "void main() {\n"
            "  v_texCoord = a_texCoord;\n"
            "  gl_Position = vec4(a_position, 0.0, 1.0);\n"
            "}\n");
        fragmentShader = QByteArrayLiteral(
            "uniform sampler2D u_texture;\n"
            "uniform float u_x0;\n"
            "uniform float u_x1;\n"
            "uniform float u_ringOffset;\n"
            "uniform float u_invHeight;\n"
            "varying vec2 v_texCoord;\n"
            "void main() {\n"
            "  float tx = mix(u_x0, u_x1, v_texCoord.x);\n"
            "  float ty = fract(u_ringOffset + v_texCoord.y * (1.0 - u_invHeight));\n"
            "  ty = fract(ty + 0.5 * u_invHeight);\n"
            "  gl_FragColor = texture2D(u_texture, vec2(tx, ty));\n"
            "}\n");
    }

    m_gpuProgram = std::make_unique<QOpenGLShaderProgram>();
    if (!m_gpuProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader) ||
        !m_gpuProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader) ||
        !m_gpuProgram->link()) {
        m_gpuFailed = true;
        const QString error = m_gpuProgram->log();
        m_gpuProgram.reset();
        emit runtimeDiagnostic(QStringLiteral("Waterfall backend: OpenGL circular renderer unavailable (%1); using CPU fallback")
                                   .arg(error));
        return;
    }

    const float vertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 1.0f
    };
    if (!m_gpuVertexArray.create()) {
        m_gpuFailed = true;
        m_gpuProgram.reset();
        emit runtimeDiagnostic(QStringLiteral("Waterfall backend: OpenGL vertex array creation failed; using CPU fallback"));
        return;
    }
    m_gpuVertexArray.bind();
    if (!m_gpuVertexBuffer.create() || !m_gpuVertexBuffer.bind()) {
        m_gpuVertexArray.release();
        m_gpuFailed = true;
        m_gpuProgram.reset();
        emit runtimeDiagnostic(QStringLiteral("Waterfall backend: OpenGL vertex buffer creation failed; using CPU fallback"));
        return;
    }
    m_gpuVertexBuffer.allocate(vertices, static_cast<int>(sizeof(vertices)));
    m_gpuVertexBuffer.release();
    m_gpuVertexArray.release();

    m_gpuReady = true;
    m_gpuTextureNeedsRecreate = true;
    m_gpuClearPending = true;
    emit runtimeDiagnostic(QStringLiteral("Waterfall backend: OpenGL circular texture enabled (%1.%2, %3)")
                               .arg(format.majorVersion())
                               .arg(format.minorVersion())
                               .arg(openGles ? QStringLiteral("OpenGL ES")
                                             : (desktopCore ? QStringLiteral("desktop core")
                                                            : QStringLiteral("desktop compatibility"))));
}

void WaterfallWidget::destroyGpuRenderer()
{
    if (m_gpuTexture != 0) {
        glDeleteTextures(1, &m_gpuTexture);
        m_gpuTexture = 0;
    }
    if (m_gpuVertexBuffer.isCreated()) {
        m_gpuVertexBuffer.destroy();
    }
    if (m_gpuVertexArray.isCreated()) {
        m_gpuVertexArray.destroy();
    }
    m_gpuProgram.reset();
    m_gpuReady = false;
    m_gpuTextureWidth = 0;
    m_gpuTextureHeight = 0;
}

void WaterfallWidget::ensureGpuTexture()
{
    if (!m_gpuReady || m_gpuFailed || width() <= 0 || height() <= 0 || m_frequencyBins <= 1) {
        return;
    }
    const int targetWidth = qBound(2, m_frequencyBins, 16384);
    const int targetHeight = plotHeight();
    // Height changes are presentation-only. Keep the already allocated ring
    // instead of destroying its history every time a layout changes by a few
    // pixels. Recreate only for first allocation or a true spectrum-width
    // change.
    if (!m_gpuTextureNeedsRecreate && m_gpuTexture != 0 &&
        m_gpuTextureWidth == targetWidth) {
        return;
    }

    if (m_gpuTexture != 0) {
        glDeleteTextures(1, &m_gpuTexture);
        m_gpuTexture = 0;
    }
    glGenTextures(1, &m_gpuTexture);
    if (m_gpuTexture == 0) {
        m_gpuFailed = true;
        emit runtimeDiagnostic(QStringLiteral("Waterfall backend: OpenGL texture allocation failed; using CPU fallback"));
        return;
    }
    glBindTexture(GL_TEXTURE_2D, m_gpuTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    m_gpuTextureWidth = targetWidth;
    m_gpuTextureHeight = targetHeight;
    m_periodLabels.clear();
    m_gpuWriteRow = 0;
    m_gpuTextureNeedsRecreate = false;
    m_gpuClearPending = true;
    clearGpuTexture();
}

void WaterfallWidget::clearGpuTexture()
{
    if (m_gpuTexture == 0 || m_gpuTextureWidth <= 0 || m_gpuTextureHeight <= 0) {
        return;
    }
    QByteArray pixels(m_gpuTextureWidth * m_gpuTextureHeight * 4, char(0));
    for (int i = 0; i < m_gpuTextureWidth * m_gpuTextureHeight; ++i) {
        pixels[i * 4 + 0] = char(4);
        pixels[i * 4 + 1] = char(6);
        pixels[i * 4 + 2] = char(8);
        pixels[i * 4 + 3] = char(255);
    }
    glBindTexture(GL_TEXTURE_2D, m_gpuTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 m_gpuTextureWidth, m_gpuTextureHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels.constData());
    m_gpuWriteRow = 0;
    m_gpuClearPending = false;
}

void WaterfallWidget::uploadPendingGpuRows()
{
    if (m_gpuTexture == 0 || m_gpuTextureWidth <= 0 || m_gpuTextureHeight <= 0) {
        return;
    }
    glBindTexture(GL_TEXTURE_2D, m_gpuTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // One FFT row is one history row. Never change the waterfall time scale
    // according to GUI queue depth: the old 1/2/4-row adaptive upload budget
    // made the scroll alternately stretch and compress after ordinary event-loop
    // stalls. Drain the currently queued rows in order so the ring always tracks
    // the DSP timeline. Hidden-window rows are already discarded in addLine().
    const int queued = m_pendingGpuRows.size();
    const int uploadBudget = queued;
    int uploaded = 0;
    while (!m_pendingGpuRows.isEmpty() && uploaded < uploadBudget) {
        const QByteArray row = m_pendingGpuRows.dequeue();
        if (row.size() != m_gpuTextureWidth * 4) {
            ++m_droppedGpuRows;
            continue;
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, m_gpuWriteRow,
                        m_gpuTextureWidth, 1,
                        GL_RGBA, GL_UNSIGNED_BYTE, row.constData());
        m_gpuWriteRow = (m_gpuWriteRow + 1) % m_gpuTextureHeight;
        ++uploaded;
    }
}

QByteArray WaterfallWidget::rgbaRowForLine(const QVector<quint8> &line) const
{
    if (line.isEmpty()) {
        return {};
    }
    QByteArray row(line.size() * 4, char(0));
    for (int x = 0; x < line.size(); ++x) {
        const QRgb color = colorForIntensity(line.at(x));
        row[x * 4 + 0] = static_cast<char>(qRed(color));
        row[x * 4 + 1] = static_cast<char>(qGreen(color));
        row[x * 4 + 2] = static_cast<char>(qBlue(color));
        row[x * 4 + 3] = static_cast<char>(255);
    }
    return row;
}

void WaterfallWidget::drawGpuWaterfall()
{
    if (!m_gpuReady || m_gpuTexture == 0 || !m_gpuProgram || !m_gpuVertexBuffer.isCreated()) {
        return;
    }
    // The backing framebuffer is expressed in physical pixels, unlike the
    // QWidget geometry.  Scale the viewport by DPR or the circular texture is
    // rendered only into the lower-left part of the waterfall on HiDPI screens.
    const qreal dpr = devicePixelRatioF();
    // The rows go above the frequency scale only: under its translucent band
    // the newest rows - with a slow FT row rate, the last seconds - would hide.
    glViewport(0, qRound(static_cast<qreal>(bottomScaleBandHeight()) * dpr),
               qMax(1, qRound(static_cast<qreal>(width()) * dpr)),
               qMax(1, qRound(static_cast<qreal>(plotHeight()) * dpr)));
    // A QPainter pass follows every GL pass.  Reset the state that its OpenGL
    // paint engine can preserve between PartialUpdate-style frames; otherwise
    // old label pixels may remain and accumulate as fuzzy text.
    glDisable(GL_SCISSOR_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glClearColor(4.0f / 255.0f, 6.0f / 255.0f, 8.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const double fullSpan = qMax(1.0e-9, m_maxHz - m_minHz);
    const float x0 = static_cast<float>(qBound(0.0, (visibleMinHz() - m_minHz) / fullSpan, 1.0));
    const float x1 = static_cast<float>(qBound(0.0, (visibleMaxHz() - m_minHz) / fullSpan, 1.0));
    const float invHeight = 1.0f / static_cast<float>(qMax(1, m_gpuTextureHeight));
    const float ringOffset = static_cast<float>(m_gpuWriteRow) * invHeight;

    m_gpuProgram->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gpuTexture);
    m_gpuProgram->setUniformValue("u_texture", 0);
    m_gpuProgram->setUniformValue("u_x0", x0);
    m_gpuProgram->setUniformValue("u_x1", x1);
    m_gpuProgram->setUniformValue("u_ringOffset", ringOffset);
    m_gpuProgram->setUniformValue("u_invHeight", invHeight);

    m_gpuVertexArray.bind();
    m_gpuVertexBuffer.bind();
    const int positionLocation = m_gpuProgram->attributeLocation("a_position");
    const int texCoordLocation = m_gpuProgram->attributeLocation("a_texCoord");
    m_gpuProgram->enableAttributeArray(positionLocation);
    m_gpuProgram->enableAttributeArray(texCoordLocation);
    m_gpuProgram->setAttributeBuffer(positionLocation, GL_FLOAT, 0, 2, 4 * static_cast<int>(sizeof(float)));
    m_gpuProgram->setAttributeBuffer(texCoordLocation, GL_FLOAT, 2 * static_cast<int>(sizeof(float)), 2, 4 * static_cast<int>(sizeof(float)));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_gpuProgram->disableAttributeArray(positionLocation);
    m_gpuProgram->disableAttributeArray(texCoordLocation);
    m_gpuVertexBuffer.release();
    m_gpuVertexArray.release();
    m_gpuProgram->release();
}

void WaterfallWidget::requestRepaint()
{
    if (m_repaintQueued) {
        return;
    }

    m_repaintLatencyClock.restart();
    m_repaintQueued = true;
    // 8 ms lets a two-row audio burst be presented over two display frames on
    // high-refresh systems.  QOpenGLWidget/Qt still coalesces updates to the
    // actual compositor refresh rate, so this does not force busy-loop painting.
    m_repaintTimer.start(8);
}

// -----------------------------------------------------------------------------
// Color mapping
// -----------------------------------------------------------------------------

void WaterfallWidget::buildColorTable()
{
    m_colorTable.resize(256);

    auto interpolateStops = [](const QVector<QColor> &stops, int v) -> QColor {
        if (stops.isEmpty()) {
            return QColor(v, v, v);
        }
        if (stops.size() == 1) {
            return stops.first();
        }

        const double pos = (static_cast<double>(qBound(0, v, 255)) / 255.0) *
                           static_cast<double>(stops.size() - 1);
        const int i0 = qBound(0, static_cast<int>(qFloor(pos)), stops.size() - 1);
        const int i1 = qBound(0, i0 + 1, stops.size() - 1);
        const double f = qBound(0.0, pos - static_cast<double>(i0), 1.0);

        const QColor a = stops[i0];
        const QColor b = stops[i1];
        return QColor(qRound(a.red()   + (b.red()   - a.red())   * f),
                      qRound(a.green() + (b.green() - a.green()) * f),
                      qRound(a.blue()  + (b.blue()  - a.blue())  * f));
    };

    QVector<QColor> stops;
    if (m_paletteName == QStringLiteral("grayscale")) {
        stops = {QColor(0, 0, 0), QColor(255, 255, 255)};
    } else if (m_paletteName == QStringLiteral("fldigi")) {
        // WSJT-X ships a palette named "Fldigi"; the stop colours below follow
        // that file, interpolated to 256 colours here.
        stops = {
            QColor(0, 0, 0), QColor(0, 0, 177), QColor(3, 110, 227),
            QColor(0, 204, 204), QColor(223, 223, 223), QColor(0, 234, 0),
            QColor(244, 244, 0), QColor(250, 126, 0), QColor(244, 0, 0)
        };
    } else if (m_paletteName == QStringLiteral("mshv")) {
        // MSHV-like high-contrast blue/green/yellow display: black background,
        // cold weak traces, then bright green/yellow for readable FT streaks.
        stops = {
            QColor(0, 0, 0), QColor(0, 8, 44), QColor(0, 28, 105),
            QColor(0, 78, 150), QColor(0, 145, 95), QColor(70, 205, 40),
            QColor(210, 225, 45), QColor(255, 156, 24), QColor(255, 255, 230)
        };
    } else if (m_paletteName == QStringLiteral("raptor")) {
        stops = {
            QColor(0, 6, 0), QColor(0, 28, 8), QColor(16, 70, 18),
            QColor(42, 118, 30), QColor(92, 164, 46), QColor(164, 210, 74),
            QColor(230, 238, 130)
        };
    } else {
        // v1.59 default: darker noise floor, fast transition through blue/cyan,
        // then green/yellow/orange for actual traces.  This is deliberately
        // non-linear looking and closer to the practical contrast operators
        // expect from WSJT-X/MSHV/fldigi waterfalls.
        stops = {
            QColor(0, 0, 0), QColor(0, 3, 34), QColor(0, 18, 96),
            QColor(0, 55, 150), QColor(0, 130, 155), QColor(20, 185, 70),
            QColor(170, 222, 40), QColor(255, 238, 40), QColor(255, 126, 0),
            QColor(255, 250, 215)
        };
    }

    for (int v = 0; v < 256; ++v) {
        m_colorTable[v] = interpolateStops(stops, v).rgb();
    }
}

QRgb WaterfallWidget::colorForIntensity(quint8 value) const
{
    const double percent = static_cast<double>(qBound(5, m_colorScalePercent, 100));

    /*
     * Follow the WSJT-X Wide Graph gain law rather than applying a gamma curve
     * to an already levelled image.  DspEngine now supplies the same kind of
     * lower-envelope-subtracted dB value used by WSJT-X Flatten.  The saved
     * MultiModeModem default of 80% is the unity-gain position; moving the control
     * changes gain exponentially, matching 10^(0.015 * PlotGain).
     */
    const double plotGain = percent - 80.0;
    const double gain = qPow(10.0, 0.015 * plotGain);
    const int scaled = qBound(0,
                              static_cast<int>(qRound(static_cast<double>(value) * gain)),
                              255);

    if (m_colorTable.size() == 256) {
        return m_colorTable[scaled];
    }

    return QColor(scaled, scaled, scaled).rgb();
}

void WaterfallWidget::ensureImage(int preferredFrequencyBins)
{
    if (width() <= 0 || height() <= 0) return;
    if (m_scrollDirection == ScrollDirection::Down && preferredFrequencyBins > 0)
        m_frequencyBins = qBound(2, preferredFrequencyBins, 16384);
    const int targetWidth = m_scrollDirection == ScrollDirection::Down &&
                            m_frequencyBins > 0
        ? m_frequencyBins : width();
    const QSize targetSize(targetWidth, height());
    if (m_image.size() == targetSize && !m_image.isNull()) return;

    if (m_image.isNull()) {
        m_image = QImage(targetSize, QImage::Format_RGB32);
        m_image.fill(QColor(4, 6, 8));
    } else {
        m_image = m_image.scaled(targetSize, Qt::IgnoreAspectRatio,
                                 Qt::FastTransformation);
    }
}
