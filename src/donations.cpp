#include "donations.h"

Donations::Donations(DbAdapter *db, QTabWidget *parent) : QTabWidget(parent)
{
    this->db = db;
    
    DonationsList *donations_list = new DonationsList(db);
    DonationsChart *donations_chart = new DonationsChart(db);
    
    addTab(donations_list, "List");
    addTab(donations_chart, "Charts");
}




DonationsChart::DonationsChart(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(new QVBoxLayout);
    
    QList<QMap<QString,QVariant>> data = this->db->donationsByMonth();
    
    QLineSeries *series = new QLineSeries;
    for (int i=0; i < data.length(); i++)
    {
        int amount = data.at(i)["amount"].toInt();
        QString date_str = data.at(i)["year-month"].toString();
        QDate date = QDate::fromString(date_str, "yyyy-MM");
        QDateTime datetime;
        datetime.setDate(date);
        series->append(datetime.toMSecsSinceEpoch(), amount);
    }
    
    QChart *chart = new QChart;
    chart->legend()->hide();
    chart->addSeries(series);
    //chart->createDefaultAxes();
    chart->setTitle("Donations / Month");
    
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("EUR");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    this->layout()->addWidget(chartView);
}
