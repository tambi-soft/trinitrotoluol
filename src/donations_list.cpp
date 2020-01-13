#include "donations_list.h"

DonationsList::DonationsList(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(this->layout);
    this->scroll_area->setWidgetResizable(true);
    
    QPushButton *button_import = new QPushButton("import donations");
    connect(button_import, &QPushButton::clicked, this, &DonationsList::signalImportDonations);
    
    //this->layout->addWidget(widget_filters);
    this->layout->addWidget(this->scroll_area, 1, 0, 1, 1);
    this->layout->addWidget(button_import, 2, 0);
    
    showData();
}


void DonationsList::showData()
{
    setUpdatesEnabled(false);
    
    this->grid = new QGridLayout(this);
    this->scroll_widget = new QWidget(this);
    this->scroll_widget->setLayout(this->grid);
    this->scroll_area->setWidget(this->scroll_widget);
    
    QList<QMap<QString,QVariant>> data = this->db->donationsSelect();
    
    for (int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> donation = data.at(i);
        
        this->grid->addWidget(new QLabel(donation["name"].toString()), i, 0);
        this->grid->addWidget(new QLabel(donation["amount"].toString()), i, 1);
        this->grid->addWidget(new QLabel(donation["code"].toString()), i, 2);
        this->grid->addWidget(new QLabel(donation["date"].toString()), i, 3);
    }
    
    this->grid->setColumnStretch(100, 100);
    // push everything up
    this->grid->setRowStretch(data.length(), 100);
    
    setUpdatesEnabled(true);
}

void DonationsList::refresh()
{
    
}
