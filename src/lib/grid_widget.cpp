#include "grid_widget.h"

GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
    setLayout(this->layout);
    
    this->scroll_area->setWidgetResizable(true);
    
    initGrid();
    
    this->layout->addWidget(this->scroll_area);
}

void GridWidget::initGrid()
{
    this->grid->setHorizontalSpacing(this->horizontalSpacing);
    // push all columns to the left for getting the table a bit more compact
    this->grid->setColumnStretch(100, 100);
    // push everything up
    this->grid->setRowStretch(10000, 100);
}

// to be reimplemented
void GridWidget::showData()
{
    
}

void GridWidget::deleteView()
{
    if (this->scroll_widget == nullptr)
    {
        this->scroll_widget->deleteLater();
    }
    
    this->grid = nullptr;
    this->grid = new QGridLayout;
    initGrid();
    
    this->scroll_widget = new QWidget(this);
    this->scroll_widget->setLayout(this->grid);
    this->scroll_area->setWidget(this->scroll_widget);
    
    showData();
}

void GridWidget::setHorizontalSpacing(int spacing)
{
    this->horizontalSpacing = spacing;
    
    this->grid->setHorizontalSpacing(spacing);
}
