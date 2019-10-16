#include "people_selector.h"

PeopleSelector::PeopleSelector(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , filter_group (new QComboBox)
    , filter_name (new QLineEdit)
{
    this->db = db;
    
    setLayout(this->layout);
    
    drawData();
}

void PeopleSelector::drawData()
{
    
    QVBoxLayout *scroll_layout = new QVBoxLayout;
    QScrollArea *scroll_area = new QScrollArea;
    QWidget *scroll_widget = new QWidget;
    scroll_widget->setLayout(scroll_layout);
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(scroll_widget);
    
    this->layout->addWidget(scroll_area);
    
    QString group = "%";
    QString name = "%";
    QString mail = "%";
    QList<QMap<QString,QVariant>> data = this->db->selectAllPersonsFiltered(-1, -1, -1, 0, -1, group, name, mail);
    
    for(int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> person = data.at(i);
        
        qlonglong rowid = person["rowid"].toLongLong();
        QString name = person["name"].toString();
        QPushButton *button_person = new QPushButton(person["name"].toString());
        scroll_layout->addWidget(button_person);
        connect(button_person, &QPushButton::clicked, this, [this, rowid, name]{ personButtonClicked(rowid, name); });
    }
}

void PeopleSelector::personButtonClicked(qlonglong rowid, QString name)
{
    emit personSelected(rowid, name);
}
