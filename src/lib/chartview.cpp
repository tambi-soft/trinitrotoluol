#include "chartview.h"

ChartView::ChartView(QChart *chart, QWidget *parent) : QChartView(chart, parent)
{
    setDragMode(QGraphicsView::NoDrag);
    this->setMouseTracking(true);
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
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
        //chart()->scroll(-dPos.x(), dPos.y());
        this->chart()->scroll(-dPos.x(), this->chart()->pos().y());

        lastMousePos_ = event->pos();
        event->accept();

        QApplication::restoreOverrideCursor();
    }

    QChartView::mouseMoveEvent(event);
}
