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
    QMap<QString, QVariant> currency_map = this->db->currencySelectDefault();
    QString currency_code = currency_map["code"].toString();
    
    QGroupBox* group_money = new QGroupBox("Finances");
    
    QMap<QString,QVariant> money = this->db->selectMoneyStats();
    //int monthly_sum = money["monthly_sum"].toInt();
    int monthly_sum_promised = money["monthly_sum_promised"].toInt();
    
    int average_money = calculateAverageMoney();
    
    QGridLayout *grid_money = new QGridLayout;
    group_money->setLayout(grid_money);
    this->layout->addWidget(group_money);
    
    grid_money->addWidget(new QLabel("<b>Last Three Month (average)</b>"), 0, 0); //<b>Monthly Transfers</b>
    grid_money->addWidget(new QLabel("<b>Monthly Promised</b>"), 0, 1);
    grid_money->addWidget(new QLabel("<b>Monthly Sum</b>"), 0, 2);
    
    //grid_money->addWidget(new QLabel(QString::number(monthly_sum)), 1, 0);
    grid_money->addWidget(new QLabel(QString::number(average_money)+" "+currency_code), 1, 0);
    grid_money->addWidget(new QLabel(QString::number(monthly_sum_promised)+" "+currency_code), 1, 1);
    //grid_money->addWidget(new QLabel(QString::number(monthly_sum + monthly_sum_promised)), 1, 2);
    this->monthly_sum = average_money + monthly_sum_promised;
    grid_money->addWidget(new QLabel(QString::number(this->monthly_sum)+" "+currency_code), 1, 2);
    
    // spacer
    grid_money->addWidget(new QLabel(""), 2, 0);
    
    grid_money->addWidget(new QLabel("<b>Minimum</b>"), 3, 0);
    grid_money->addWidget(new QLabel("<b>Maximum</b>"), 3, 1);
    grid_money->addWidget(new QLabel("<b>Average</b>"), 3, 2);
    //qDebug() << money;
    int donations_min = money["donations_min"].toInt();
    grid_money->addWidget(new QLabel(QString::number(donations_min)+" "+currency_code), 4, 0);
    
    int donations_max = money["donations_max"].toInt();
    grid_money->addWidget(new QLabel(QString::number(donations_max)+" "+currency_code), 4, 1);
    
    int donations_average = money["donations_average"].toInt();
    grid_money->addWidget(new QLabel(QString::number(donations_average)+" "+currency_code), 4, 2);
    
    QString needed_money = this->db->selectSettings("money_target");
    double current_percentage = (this->monthly_sum) / needed_money.toDouble() * 100;
    
    // spacer
    grid_money->addWidget(new QLabel(), 5, 0);
    // round the percentage value to two decimal values
    QString percentage_display = QString::number(round(current_percentage * 100) / 100);
    QLabel *percentage = new QLabel(percentage_display + "%");
    grid_money->addWidget(new QLabel("<b>Percentage</b>"), 6, 1);
    grid_money->addWidget(percentage, 7, 1);
    percentage->setToolTip("monthly_sum / needed_money * 100");
    
}

int Stats::calculateAverageMoney()
{
    QList<QMap<QString,QVariant>> donations_monthly = this->db->donationsByMonth();
    int average_money = 0;
    int month_amount = 0;
    for (int i = 1; i < donations_monthly.length(); i++)
    {
        QMap<QString,QVariant> value = donations_monthly.at(donations_monthly.length()-i);
        // the amount in the current month is probably not complete, so better skip the current month
        if (QDate::currentDate().toString("yyyyMM").toInt() > QDate::fromString(value["year-month"].toString(), "yyyy-MM").toString("yyyyMM").toInt())
        {
            month_amount++;
            average_money += value["amount"].toInt();
        }
        
        if (month_amount >= 3)
        {
            average_money = average_money / month_amount;
            break;
        }
    }
    
    return average_money;
}

void Stats::addRemainingStats()
{
    QMap<QString, QVariant> currency_map = this->db->currencySelectDefault();
    QString currency_code = currency_map["code"].toString();
    
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
    
    
    QWidget *needed_money_widget = new QWidget();
    QLayout *needed_money_widget_layout = new QHBoxLayout();
    needed_money_widget_layout->setMargin(0);
    needed_money_widget->setLayout(needed_money_widget_layout);
    
    QString needed_money = this->db->selectSettings("money_target");
    this->edit_money_needed = new QLineEdit();
    edit_money_needed->setPlaceholderText("type here your money-target");
    edit_money_needed->setText(needed_money);
    
    needed_money_widget_layout->addWidget(this->edit_money_needed);
    needed_money_widget_layout->addWidget(new QLabel(currency_code));
    edit_money_needed->setMaximumWidth(100);
    //grid->addWidget(this->edit_money_needed, 1, 0, Qt::AlignLeft);
    grid->addWidget(needed_money_widget, 1, 0, Qt::AlignLeft);
    
    QString money_target = this->edit_money_needed->text();
    connect(this->edit_money_needed, &QLineEdit::textChanged, this, &Stats::onMoneyTargetChanged);
    
    //onMoneyTargetChanged(needed_money);
    onMoneyTargetCalculate();
}

void Stats::onMoneyTargetChanged(QString target)
{
    this->db->insertSettings("money_target", target);
    
    onMoneyTargetCalculate();
}

void Stats::onMoneyTargetCalculate()
{
    QMap<QString, QVariant> currency_map = this->db->currencySelectDefault();
    QString currency_code = currency_map["code"].toString();
    
    QMap<QString,QVariant> money = this->db->selectMoneyStats();
    //int monthly_sum = money["monthly_sum"].toInt();
    int monthly_sum_promised = money["monthly_sum_promised"].toInt();
    
    QMap<QString,QVariant> data = this->db->selectPeopleStats();
    int dp = data["donation_partners"].toInt();
    int dpp = data["donation_partners_promised"].toInt();
    
    this->label_need_remaining->setText(QString::number( this->edit_money_needed->text().toInt() - this->monthly_sum )+" "+currency_code);
    
    if (monthly_sum + monthly_sum_promised > 0)
    {
        this->label_need_donors->setText(QString::number( ((dp + dpp) * this->edit_money_needed->text().toInt()) / (this->monthly_sum + monthly_sum_promised) ));
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
