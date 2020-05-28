#include "people_selector.h"

PeopleSelector::PeopleSelector(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , filter_group (new QComboBox)
    , filter_name (new QLineEdit)
{
    this->db = db;
    
    setLayout(this->layout);
    this->layout->addWidget(this->label_description);
    this->label_description->hide();
    this->layout->addWidget(filter_name);
    this->filter_name->setPlaceholderText("filter for name");
    connect(this->filter_name, &QLineEdit::textChanged, this, &PeopleSelector::filterChanged);
    
    drawData();
}

void PeopleSelector::setDescription(QString description)
{
    this->label_description->setText(description);
    this->label_description->show();
}

void PeopleSelector::drawData()
{
    QVBoxLayout *scroll_layout = new QVBoxLayout;
    this->scroll_area = new QScrollArea;
    QWidget *scroll_widget = new QWidget;
    scroll_widget->setLayout(scroll_layout);
    this->scroll_area->setWidgetResizable(true);
    this->scroll_area->setWidget(scroll_widget);
    
    this->layout->addWidget(scroll_area);
    
    QString group = "%";
    QString name = "%"+this->filter_name->text()+"%";
    QString mail = "%";
    QList<QMap<QString,QVariant>> data = this->db->selectAllPersonsFiltered(-1, -1, -1, 0, -1, group, name, mail);
    
    for(int i=0; i < data.length(); ++i)
    {
        QMap<QString,QVariant> person = data.at(i);
        
        qlonglong rowid = person["rowid"].toLongLong();
        QString name = person["name"].toString();
        QPushButton *button_person = new QPushButton(person["name"].toString());
        
        connect(button_person, &QPushButton::clicked, this, [this, rowid, name]{ personButtonClicked(rowid, name); });
        
        scroll_layout->addWidget(button_person);
    }
}

void PeopleSelector::clearData()
{
    this->scroll_area->deleteLater();
}

void PeopleSelector::filterChanged()
{
    clearData();
    drawData();
}

void PeopleSelector::personButtonClicked(qlonglong rowid, QString name)
{
    emit personSelected(rowid, name);
}
