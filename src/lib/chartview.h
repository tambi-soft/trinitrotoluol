#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QObject>
#include <QWidget>
#include <QtCharts>
#include <QPointF>

class ChartView : public QChartView
{
    Q_OBJECT
public:
    ChartView(QChart *chart, QWidget *parent = nullptr);
    
    void resetZoomAndMove();
    
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

private:
    QPointF lastMousePos_;
    QPoint initialPos = QPoint(0, 0);
};

#endif // CHARTVIEW_H
