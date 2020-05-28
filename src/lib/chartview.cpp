#include "chartview.h"

ChartView::ChartView(QChart *chart, QWidget *parent) : QChartView(chart, parent)
{
    setDragMode(QGraphicsView::NoDrag);
    this->setMouseTracking(true);
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    // workaround: chart()->zoomReset() does not reset scroll, if it was not zoomed before, so we have to be shure, it was
    if (! this->chart()->isZoomed())
    {
        this->chart()->zoomIn();
        this->chart()->zoomOut();
    }
    
    if (this->initialPos.x()== 0 && this->initialPos.y() == 0)
    {
        this->initialPos = event->pos();
    }
    
    if (event->button() == Qt::LeftButton)
    {
        QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
        lastMousePos_ = event->pos();
        event->accept();
    }

    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    // pan the chart
    if (event->buttons() & Qt::LeftButton)
    {
        auto dPos = event->pos() - lastMousePos_;
        this->chart()->scroll(-dPos.x(), this->chart()->pos().y());

        lastMousePos_ = event->pos();
        event->accept();

        QApplication::restoreOverrideCursor();
    }

    QChartView::mouseMoveEvent(event);
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    qreal factor = event->angleDelta().y() > 0? 0.9 : 1.1;
    QRectF rect = this->chart()->plotArea();
    qreal width_original = rect.width();
    rect.setWidth(width_original / factor);
    
    qreal xcenter = event->pos().x() - this->chart()->plotArea().x();
    qreal center_scale = xcenter / width_original;
    
    qreal left_offset = xcenter - (rect.width() * center_scale);
    rect.moveLeft(rect.x() + left_offset);
    this->chart()->zoomIn(rect);
    
    event->accept();
    
    QWidget::wheelEvent(event);
}

void ChartView::resetZoomAndMove()
{
    this->chart()->zoomReset();
}
