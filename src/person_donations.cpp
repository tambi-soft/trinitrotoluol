#include "person_donations.h"

PersonDonations::PersonDonations(DbAdapter *db, qlonglong rowid_person, QTabWidget *parent) : QTabWidget(parent)
{
    this->db = db;
    this->rowid_person = rowid_person;
    
    PersonDonationsChart *donations_chart = new PersonDonationsChart(db, rowid_person);
    PersonDonationsList *donations_list = new PersonDonationsList(db, rowid_person);
    
    this->addTab(donations_chart, "Chart");
    this->addTab(donations_list, "List");
}





PersonDonationsList::PersonDonationsList(DbAdapter *db, qlonglong rowid_person, GridWidget *parent) : GridWidget(parent)
{
    this->db = db;
    this->rowid_person = rowid_person;
    
    showData();
}

void PersonDonationsList::showData()
{
    recreateView();
    
    QList<QMap<QString,QVariant>> data = this->db->donationsSelectForPerson(this->rowid_person);
    
    this->grid->addWidget(new QLabel("<b>Amount</b>"), 0, 0);
    this->grid->addWidget(new QLabel("<b>Currency</b>"), 0, 1);
    this->grid->addWidget(new QLabel("<b>Date</b>"), 0, 2);
    this->grid->addWidget(new QLabel("<b>Memo</b>"), 0, 3);
    
    QMap<QString,QVariant> currency_default = this->db->currencySelectDefault();
    double sum = 0;
    for (int i=0; i < data.length(); ++i)
    {
        sum += data.at(i)["amount"].toDouble() * data.at(i)["exchange_rate"].toDouble();
        
        QLabel *label_amount = new QLabel(data.at(i)["amount"].toString());
        QLabel *label_currency = new QLabel(data.at(i)["code"].toString());
        QLabel *label_date = new QLabel(data.at(i)["date"].toString());
        QLabel *label_memo = new QLabel(data.at(i)["memo"].toString());
        
        this->grid->addWidget(label_amount, i+1, 0);
        this->grid->addWidget(label_currency, i+1, 1);
        this->grid->addWidget(label_date, i+1, 2);
        this->grid->addWidget(label_memo, i+1, 3);
    }
    
    QLabel *label_sum_name = new QLabel("<b>Σ:</b>");
    QLabel *label_sum_value = new QLabel(QString::number(sum));
    QLabel *label_sum_currency = new QLabel(currency_default["code"].toString());
    
    // spacer
    this->grid->addWidget(new QLabel(""), data.length()+9, 0);
    
    this->grid->addWidget(label_sum_name, data.length()+10, 0);
    this->grid->addWidget(label_sum_value, data.length()+10, 1);
    this->grid->addWidget(label_sum_currency, data.length()+10, 2);
}





PersonDonationsChart::PersonDonationsChart(DbAdapter *db, qlonglong rowid_person, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    this->rowid_person = rowid_person;
    
    setLayout(new QVBoxLayout);
    
    this->chart->setContentsMargins(0, 0, 0, 0);
    this->chart->setBackgroundRoundness(0);
    this->chart->setMargins({0, 0, 0, 0});
    this->chart->setMinimumHeight(200);
    
    this->currency_code = this->db->currencySelectDefault()["code"].toString();
    this->data = this->db->donationsSelectForPerson(this->rowid_person);
    
    if (this->data.length() > 0)
    {
        showData();
    }
}

void PersonDonationsChart::showData()
{
    QLineSeries *series_line = new QLineSeries;
    series_line->setColor(Qt::darkBlue);
    
    this->series_scatter = new QScatterSeries;
    series_scatter->setColor(Qt::darkBlue);
    series_scatter->setMarkerSize(10);
    
    int max = 0;
    for (int i=0; i < this->data.length(); i++)
    {
        int amount = this->data.at(i)["amount"].toInt();
        if (amount > max)
        {
            max = amount;
        }
        
        QString date_str = this->data.at(i)["date"].toString();
        QDate date = QDate::fromString(date_str, "yyyy-MM-dd");
        QDateTime datetime;
        datetime.setDate(date);
        
        series_line->append(datetime.toMSecsSinceEpoch(), amount);
        series_scatter->append(datetime.toMSecsSinceEpoch(), amount);
    }
    connect(series_scatter, &QScatterSeries::hovered, this, &PersonDonationsChart::onScatterHover);
    
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("yyyy-MM-dd");
    axisX->setTitleText("Date");
    
    // data is ORDER BY date DESC, so the last element is the oldest
    axisX->setMin(QDateTime::fromString(data.at(data.length()-1)["date"].toString(), "yyyy-MM-dd"));
    axisX->setMax(QDateTime::currentDateTime());
    
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText(currency_code);
    axisY->setMin(0);
    axisY->setMax(max + 20);
    
    this->chart->addAxis(axisX, Qt::AlignBottom);
    this->chart->addAxis(axisY, Qt::AlignLeft);
    
    this->chart->addSeries(series_line);
    this->chart->addSeries(series_scatter);
    //this->chart->setTitle("Donations / Month");
    this->chart->legend()->markers(series_line)[0]->setVisible(false);
    this->chart->legend()->markers(series_scatter)[0]->setVisible(false);
    
    ChartView *chartView = new ChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    series_line->attachAxis(axisX);
    series_line->attachAxis(axisY);
    series_scatter->attachAxis(axisX);
    series_scatter->attachAxis(axisY);
    
    this->layout()->addWidget(chartView);
    
    QWidget *controls = new QWidget;
    QHBoxLayout *layout_controls = new QHBoxLayout;
    controls->setLayout(layout_controls);
    
    QPushButton *button_zoom_reset = new QPushButton("reset zoom");
    connect(button_zoom_reset, &QPushButton::clicked, this, [chartView]{ chartView->resetZoomAndMove(); });
    button_zoom_reset->setMaximumWidth(100);
    
    layout_controls->addWidget(this->label_value);
    layout_controls->addWidget(button_zoom_reset);
    
    this->layout()->addWidget(controls);
}

void PersonDonationsChart::onScatterHover(QPointF pos, bool state)
{
    if (state)
    {
        QList<QPointF> points = this->series_scatter->points();
        for (int i=0; i < points.length(); i++)
        {
            if (points.at(i) == pos)
            {
                QMap<QString,QVariant> value = this->data.at(i);
                QString date = value["date"].toString();
                QString amount = value["amount"].toString();
                
                this->label_value->setText(amount + " " + this->currency_code + " (" +date +")");
            }
        }
    }
    else
    {
        this->label_value->setText("");
    }
}
