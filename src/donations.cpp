#include "donations.h"

Donations::Donations(DbAdapter *db, QTabWidget *parent) : QTabWidget(parent)
{
    this->db = db;
    
    DonationsList *donations_list = new DonationsList(db);
    //DonationsChart *donations_chart = new DonationsChart(db);
    DonationsChartContainer *donations_chart_container = new DonationsChartContainer(db);
    DonationsMapEdit *donations_map_edit = new DonationsMapEdit(db);
    
    //addTab(donations_chart, "Charts");
    addTab(donations_chart_container, "Charts");
    addTab(donations_list, "List");
    addTab(donations_map_edit, "Import Map");
}




DonationsChartContainer::DonationsChartContainer(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(new QVBoxLayout);
    
    this->layout()->setMargin(0);
    this->layout()->setSpacing(0);
}

void DonationsChartContainer::hideEvent(QHideEvent */**event**/)
{
    this->chart->deleteLater();
}

void DonationsChartContainer::showEvent(QShowEvent */**event**/)
{
    this->chart = new DonationsChart(this->db);
    this->layout()->addWidget(this->chart);
}
