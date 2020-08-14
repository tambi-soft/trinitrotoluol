#include "donations_chart.h"

DonationsChart::DonationsChart(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(new QVBoxLayout);
    
    this->data_monthly = this->db->donationsByMonth();
    this->data_full = this->db->donationsSelect();
    this->currency_code = this->db->currencySelectDefault()["code"].toString();
    
    if (this->data_monthly.length() > 0 && this->data_full.length() > 0)
    {
        drawChart();
    }
}

void DonationsChart::drawChart()
{
    QLineSeries *series_monthly = new QLineSeries;
    series_monthly->setColor(Qt::darkGreen);
    
    int max = 0;
    this->scatter_monthly = new QScatterSeries;
    this->scatter_monthly->setColor(Qt::darkGreen);
    this->scatter_monthly->setName("Monthly Sum");
    for (int i=0; i < this->data_monthly.length(); i++)
    {
        int amount = this->data_monthly.at(i)["amount"].toInt();
        if (amount > max)
        {
            max = amount;
        }
        
        QString date_str = this->data_monthly.at(i)["year-month"].toString();
        QDate date = QDate::fromString(date_str+"-15", "yyyy-MM-dd");
        QDateTime datetime;
        datetime.setDate(date);
        
        series_monthly->append(datetime.toMSecsSinceEpoch(), amount);
        this->scatter_monthly->append(datetime.toMSecsSinceEpoch(), amount);
    }
    connect(this->scatter_monthly, &QLineSeries::hovered, this, &DonationsChart::onMonthlyHover);
    
    
    QLineSeries *series_full = new QLineSeries;
    series_full->setName("All Donations");
    
    this->scatter_full = new QScatterSeries;
    this->scatter_full->setColor(Qt::darkBlue);
    this->scatter_full->setMarkerSize(10);
    this->scatter_full->setName("All Donations");
    for (int i=0; i < this->data_full.length(); i++)
    {
        int amount = this->data_full.at(i)["amount"].toInt();
        QString date_str = this->data_full.at(i)["date"].toString();
        QDate date = QDate::fromString(date_str, "yyyy-MM-dd");
        QDateTime datetime;
        datetime.setDate(date);
        
        series_full->append(datetime.toMSecsSinceEpoch(), amount);
        this->scatter_full->append(datetime.toMSecsSinceEpoch(), amount);
    }
    connect(this->scatter_full, &QLineSeries::hovered, this, &DonationsChart::onFullHover);
    
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("yyyy-MM-dd");
    axisX->setTitleText("Date");
    
    // data_full is ORDER BY date DESC, so the last element is the oldest
    axisX->setMin(QDateTime::fromString(data_full.at(data_full.length()-1)["date"].toString(), "yyyy-MM-dd"));
    // to make shure we see the last scatter-item in any case
    int days = QDateTime::currentDateTime().date().day();
    axisX->setMax(days > 15 ? QDateTime::currentDateTime() : QDateTime::currentDateTime().addDays(15));
    
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText(currency_code);
    axisY->setMin(0);
    axisY->setMax(max);
    axisY->setMinorTickCount(5);
    
    this->chart->addAxis(axisX, Qt::AlignBottom);
    this->chart->addAxis(axisY, Qt::AlignLeft);
    
    this->chart->addSeries(series_full);
    this->chart->addSeries(this->scatter_full);
    this->chart->addSeries(series_monthly);
    this->chart->addSeries(this->scatter_monthly);
    this->chart->setTitle("Donations / Month");
    
    this->chart->legend()->markers(series_monthly)[0]->setVisible(false);
    //this->chart->legend()->markers(series_full)[0]->setVisible(false);
    this->chart->legend()->markers(scatter_full)[0]->setVisible(false);
    
    //QChartView *chartView = new QChartView(chart);
    ChartView *chartView = new ChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    series_monthly->attachAxis(axisX);
    series_monthly->attachAxis(axisY);
    this->scatter_monthly->attachAxis(axisX);
    this->scatter_monthly->attachAxis(axisY);
    
    series_full->attachAxis(axisX);
    series_full->attachAxis(axisY);
    this->scatter_full->attachAxis(axisX);
    this->scatter_full->attachAxis(axisY);
    
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
    
    //this->label_value->setAlignment(Qt::AlignCenter);
    //this->layout()->addWidget(button_zoom_reset);
}

void DonationsChart::onMonthlyHover(QPointF pos, bool state)
{
    if (state)
    {
        QList<QPointF> points = this->scatter_monthly->points();
        for (int i=0; i < points.length(); i++)
        {
            if (points.at(i) == pos)
            {
                QMap<QString,QVariant> value = this->data_monthly.at(i);
                QString date = value["year-month"].toString();
                QString amount = value["amount"].toString();
                
                this->label_value->setText(date + ": " + amount +" "+ this->currency_code);
            }
        }
    }
    else
    {
        this->label_value->setText("");
    }
}

void DonationsChart::onFullHover(QPointF pos, bool state)
{
    if (state)
    {
        QList<QPointF> points = this->scatter_full->points();
        for (int i=0; i < points.length(); i++)
        {
            if (points.at(i) == pos)
            {
                QMap<QString,QVariant> value = this->data_full.at(i);
                QString name = value["name"].toString();
                QString date = value["date"].toString();
                QString amount = value["amount"].toString();
                
                this->label_value->setText(name + ": " + amount + " " + this->currency_code + " (" +date +")");
            }
        }
    }
    else
    {
        this->label_value->setText("");
    }
}
