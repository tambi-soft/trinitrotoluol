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
    
    //recreateView();
}

void DonationsList::showData()
{
    setUpdatesEnabled(false);
    
    QList<QMap<QString,QVariant>> data_monthly = this->db->donationsByMonth();
    QMap<QString,QVariant> data_monthly_map;
    for (int i=0; i < data_monthly.length(); i++)
    {
        data_monthly_map[data_monthly.at(i)["year-month"].toString()] = data_monthly.at(i)["amount"].toString();
    }
    
    QList<QMap<QString,QVariant>> data = this->db->donationsSelect();
    
    QDate date_last = QDate::currentDate();
    int y = 0;
    for (int i=0; i < data.length(); ++i)
    {
        ++y;
        
        QMap<QString,QVariant> donation = data.at(i);
        
        QDate date_current = QDate::fromString(donation["date"].toString(), "yyyy-MM-dd");
        
        if (date_last.month() != date_current.month())
        {
            QString sum = data_monthly_map[date_last.toString("yyyy-MM")].toString();
            this->grid->addWidget(new QLabel("<b>Σ: "+sum+"</b>"), y, 0);
            y++;
            this->grid->addWidget(new QLabel(), y, 0);
            y++;
        }
        
        date_last = date_current;
        
        this->grid->addWidget(new QLabel(donation["name"].toString()), y, 0);
        this->grid->addWidget(new QLabel(donation["amount"].toString()), y, 1);
        this->grid->addWidget(new QLabel(donation["code"].toString()), y, 2);
        this->grid->addWidget(new QLabel(donation["date"].toString()), y, 3);
    }
    QString sum = data_monthly_map[date_last.toString("yyyy-MM")].toString();
    this->grid->addWidget(new QLabel("<b>Σ: "+sum+"</b>"), y+1, 0);
    
    this->grid->setColumnStretch(100, 100);
    // push everything up
    this->grid->setRowStretch(data.length(), 100);
    
    setUpdatesEnabled(true);
}

void DonationsList::recreateView()
{
    if (this->scroll_widget != nullptr)
    {
        this->scroll_widget->deleteLater();
    }
    
    this->grid = nullptr;
    this->grid = new QGridLayout(this);
    initGrid();
    
    this->scroll_widget = new QWidget(this);
    this->scroll_widget->setLayout(this->grid);
    this->scroll_area->setWidget(this->scroll_widget);
    
    showData();
}

void DonationsList::initGrid()
{
    
}

void DonationsList::hideEvent(QHideEvent */**event**/)
{
    
}

void DonationsList::showEvent(QShowEvent */**event**/)
{
    recreateView();
}
