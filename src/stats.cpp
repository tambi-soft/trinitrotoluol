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
    
    grid_people->addWidget(new QLabel("<b>People</b>"), 0, 0);
    grid_people->addWidget(new QLabel("<b>Agreed Mail</b>"), 0, 1);
    grid_people->addWidget(new QLabel("<b>Agreed Prayer</b>"), 0, 2);
    
    grid_people->addWidget(new QLabel(data["sum_all"].toString()), 1, 0);
    grid_people->addWidget(new QLabel(data["sum_agreed_mail"].toString()), 1, 1);
    grid_people->addWidget(new QLabel(data["sum_agreed_prayer"].toString()), 1, 2);
    
    // spacer
    grid_people->addWidget(new QLabel(""), 2, 0);
    
    grid_people->addWidget(new QLabel("<b>Donating</b>"), 3, 0);
    grid_people->addWidget(new QLabel("<b>Promised</b>"), 3, 1);
    grid_people->addWidget(new QLabel("<b>Sum</b>"), 3, 2);
    
    int dp = data["donation_partners"].toInt();
    int dpp = data["donation_partners_promised"].toInt();
    grid_people->addWidget(new QLabel(QString::number(dp)), 4, 0);
    grid_people->addWidget(new QLabel(QString::number(dpp)), 4, 1); 
    grid_people->addWidget(new QLabel(QString::number(dp + dpp)), 4, 2);
    
    this->layout->addWidget(group_people);
}

void Stats::addMoneyStats()
{
    QGroupBox* group_money = new QGroupBox("Finances");
    
    QMap<QString,QVariant> money = this->db->selectMoneyStats();
    int monthly_sum = money["monthly_sum"].toInt();
    int monthly_sum_promised = money["monthly_sum_promised"].toInt();
    
    QGridLayout *grid_money = new QGridLayout;
    group_money->setLayout(grid_money);
    this->layout->addWidget(group_money);
    
    grid_money->addWidget(new QLabel("<b>Monthly Transfers</b>"), 0, 0);
    grid_money->addWidget(new QLabel("<b>Monthly Promised</b>"), 0, 1);
    grid_money->addWidget(new QLabel("<b>Monthly Sum</b>"), 0, 2);
    
    grid_money->addWidget(new QLabel(QString::number(monthly_sum)), 1, 0);
    grid_money->addWidget(new QLabel(QString::number(monthly_sum_promised)), 1, 1);
    grid_money->addWidget(new QLabel(QString::number(monthly_sum + monthly_sum_promised)), 1, 2);
    
    // spacer
    grid_money->addWidget(new QLabel(""), 2, 0);
    
    grid_money->addWidget(new QLabel("<b>Minimum</b>"), 3, 0);
    grid_money->addWidget(new QLabel("<b>Maximum</b>"), 3, 1);
    grid_money->addWidget(new QLabel("<b>Average</b>"), 3, 2);
    qDebug() << money;
    int donations_min = money["donations_min"].toInt();
    grid_money->addWidget(new QLabel(QString::number(donations_min)), 4, 0);
    
    int donations_max = money["donations_max"].toInt();
    grid_money->addWidget(new QLabel(QString::number(donations_max)), 4, 1);
    
    int donations_average = money["donations_average"].toInt();
    grid_money->addWidget(new QLabel(QString::number(donations_average)), 4, 2);
    
    QString needed_money = this->db->selectSettings("money_target");
    double current_percentage = (monthly_sum + monthly_sum_promised) / needed_money.toDouble() * 100;
    
    // spacer
    grid_money->addWidget(new QLabel(), 5, 0);
    // round the percentage value to two decimal values
    QString percentage_display = QString::number(round(current_percentage * 100) / 100);
    QLabel *percentage = new QLabel(percentage_display + "%");
    grid_money->addWidget(new QLabel("<b>Percentage</b>"), 6, 1);
    grid_money->addWidget(percentage, 7, 1);
    percentage->setToolTip("monthly_sum / needed_moned * 100");
    
}

void Stats::addRemainingStats()
{
    QGroupBox* group = new QGroupBox("Remaining");
    
    QGridLayout *grid = new QGridLayout;
    group->setLayout(grid);
    this->layout->addWidget(group);
    
    grid->addWidget(new QLabel("<b>Needed Money</b>"), 0, 0);
    grid->addWidget(new QLabel("<b>Remaining Need</b>"), 0, 1);
    grid->addWidget(new QLabel("<b>Needed Donors</b"), 0, 2);
    
    this->label_need_remaining = new QLabel;
    this->label_need_donors = new QLabel;
    
    grid->addWidget(this->label_need_remaining, 1, 1);
    grid->addWidget(this->label_need_donors, 1, 2);
    
    this->label_need_donors->setToolTip("= (needed_money * people_sum) / monthly_sum");
    
    QString needed_money = this->db->selectSettings("money_target");
    this->edit_money_needed = new QLineEdit();
    edit_money_needed->setPlaceholderText("type here your money-target");
    edit_money_needed->setText(needed_money);
    edit_money_needed->setMaximumWidth(100);
    grid->addWidget(this->edit_money_needed, 1, 0, Qt::AlignLeft);
    
    QString money_target = this->edit_money_needed->text();
    connect(this->edit_money_needed, &QLineEdit::textChanged, this, &Stats::onMoneyTargetChanged);
    
    onMoneyTargetChanged(needed_money);
}

void Stats::onMoneyTargetChanged(QString target)
{
    QMap<QString,QVariant> money = this->db->selectMoneyStats();
    int monthly_sum = money["monthly_sum"].toInt();
    int monthly_sum_promised = money["monthly_sum_promised"].toInt();
    
    QMap<QString,QVariant> data = this->db->selectPeopleStats();
    int dp = data["donation_partners"].toInt();
    int dpp = data["donation_partners_promised"].toInt();
    
    this->db->insertSettings("money_target", target);
    
    this->label_need_remaining->setText(QString::number( this->edit_money_needed->text().toInt() - monthly_sum - monthly_sum_promised ));
    
    if (monthly_sum + monthly_sum_promised > 0)
    {
        this->label_need_donors->setText(QString::number( ((dp + dpp) * this->edit_money_needed->text().toInt()) / (monthly_sum + monthly_sum_promised) ));
    }
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
    addRemainingStats();
    
    this->layout->addStretch(100);
}
