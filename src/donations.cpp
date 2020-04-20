#include "donations.h"

Donations::Donations(DbAdapter *db, QTabWidget *parent) : QTabWidget(parent)
{
    this->db = db;
    
    DonationsList *donations_list = new DonationsList(db);
    DonationsChart *donations_chart = new DonationsChart(db);
    
    addTab(donations_chart, "Charts Monthly");
    addTab(donations_list, "List");
}




DonationsChart::DonationsChart(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(new QVBoxLayout);
    
    this->data = this->db->donationsByMonth();
    this->currency_code = this->db->currencySelectDefault()["code"].toString();
    
    QLineSeries *series = new QLineSeries;
    series->setPointsVisible(true);
    series->setColor(Qt::darkGreen);
    //series->setPen(QPen(Qt::darkBlue, 3));
    
    this->scatter = new QScatterSeries;
    
    
    for (int i=0; i < this->data.length(); i++)
    {
        int amount = this->data.at(i)["amount"].toInt();
        QString date_str = this->data.at(i)["year-month"].toString();
        QDate date = QDate::fromString(date_str, "yyyy-MM");
        QDateTime datetime;
        datetime.setDate(date);
        
        series->append(datetime.toMSecsSinceEpoch(), amount);
        this->scatter->append(datetime.toMSecsSinceEpoch(), amount);
    }
    connect(this->scatter, &QLineSeries::hovered, this, &DonationsChart::onLineHover);
    
    QChart *chart = new QChart;
    chart->legend()->hide();
    chart->addSeries(series);
    chart->addSeries(this->scatter);
    //chart->createDefaultAxes();
    chart->setTitle("Donations / Month");
    
    QDateTimeAxis *axisX = new QDateTimeAxis;
    //axisX->setTickCount(20);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis;
    //axisY->setMin(0);
    //axisY->setMax(3000);
    axisY->setLabelFormat("%i");
    axisY->setTitleText(currency_code);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    this->layout()->addWidget(chartView);
    this->layout()->addWidget(this->label_value);
    this->label_value->setAlignment(Qt::AlignCenter);
}

void DonationsChart::onLineHover(QPointF pos, bool state)
{
    if (state)
    {
        QList<QPointF> points = this->scatter->points();
        for (int i=0; i < points.length(); i++)
        {
            if (points.at(i) == pos)
            {
                QMap<QString,QVariant> value = this->data.at(i);
                QString date = value["year-month"].toString();
                QString amount = value["amount"].toString();
                
                //setToolTip(date + ": " + amount +" "+ this->currency_code);
                this->label_value->setText(date + ": " + amount +" "+ this->currency_code);
            }
        }
    }
    else
    {
        this->label_value->setText("");
    }
}
