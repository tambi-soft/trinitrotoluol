#include "stats.h"

Stats::Stats(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    this->db = db;
    setLayout(this->layout);
}

void Stats::addPeopleStats()
{
    QMap<QString,QVariant> data = this->db->selectPeopleStats();
    
    QGroupBox* group_people = new QGroupBox("People");
    QGridLayout* grid_people = new QGridLayout;
    group_people->setLayout(grid_people);
    
    grid_people->addWidget(new QLabel("<b>SUM People</b>"), 0, 0);
    grid_people->addWidget(new QLabel("<b>SUM Agreed Mail</b>"), 0, 1);
    grid_people->addWidget(new QLabel("<b>SUM Agreed Prayer</b>"), 0, 2);
    
    grid_people->addWidget(new QLabel(data["sum_all"].toString()), 1, 0);
    grid_people->addWidget(new QLabel(data["sum_agreed_mail"].toString()), 1, 1);
    grid_people->addWidget(new QLabel(data["sum_agreed_prayer"].toString()), 1, 2);
    
    this->layout->addWidget(group_people);
}

void Stats::addMoneyStats()
{
    QGroupBox* group_money = new QGroupBox("Finances");
    
    QMap<QString,QVariant> money = this->db->selectMoneyStats();
    int monthly_sum = money["monthly_sum"].toInt();
    int monthly_sum_promised = money["monthly_sum_promised"].toInt();
    
    QGridLayout *grid_money = new QGridLayout;
    grid_money->setMargin(0);
    group_money->setLayout(grid_money);
    this->layout->addWidget(group_money);
    
    grid_money->addWidget(new QLabel("<b>Monthly Transfers</b>"), 0, 0);
    grid_money->addWidget(new QLabel("<b>Monthly Promised</b>"), 0, 1);
    grid_money->addWidget(new QLabel("<b>Monthly Sum</b>"), 0, 2);
    
    grid_money->addWidget(new QLabel(QString::number(monthly_sum)), 1, 0);
    grid_money->addWidget(new QLabel(QString::number(monthly_sum_promised)), 1, 1);
    grid_money->addWidget(new QLabel(QString::number(monthly_sum + monthly_sum_promised)), 1, 2);
}

void Stats::clearLayout(QLayout* layout)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (QWidget* widget = item->widget())
        {
            widget->deleteLater();
        }
        
        if (QLayout* childLayout = item->layout())
        {
            clearLayout(childLayout);
        }
    }
}

void Stats::showEvent(QShowEvent */*event*/)
{
    clearLayout(this->layout);
    
    addPeopleStats();
    addMoneyStats();
    
    this->layout->addStretch(100);
}