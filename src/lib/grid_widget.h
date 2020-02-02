#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QObject>
#include <QWidget>

#include <QGridLayout>
#include <QScrollArea>

#include <QDebug>

class GridWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GridWidget(QWidget *parent = nullptr);
    
    void initGrid();
    // show data is the main function you want to reimplement
    void showData();
    void recreateView();
    
    void setHorizontalSpacing(int spacing);
    
    int horizontalSpacing = 20;
    
    QVBoxLayout *layout = new QVBoxLayout;
    QGridLayout *grid = new QGridLayout;
    
    QScrollArea *scroll_area = new QScrollArea;
    QWidget *scroll_widget = nullptr;
    
private:
    
signals:
    
};

#endif // GRIDWIDGET_H
