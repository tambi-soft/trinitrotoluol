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
    
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPointF lastMousePos_;
};

#endif // CHARTVIEW_H
