#include "people_list.h"

PeopleList::PeopleList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , table_widget (new QTableWidget)
{
    this->db = db;
    
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    layout->addWidget(this->table_widget);
    
    QPushButton *button_add_new_person = new QPushButton("add new person");
    layout->addWidget(button_add_new_person);
    
    showPeople();
    
    //this->db->insertNewPerson("name", "group", "email", "add", "phone");
}

void PeopleList::showPeople()
{
    QList<QMap<QString,QVariant>> people = this->db->selectAllPersons();
    
    this->table_widget->setRowCount(people.length());
    if (people.length() > 0)
    {
        this->table_widget->setColumnCount(people.at(1).keys().length());
    }
    
    for (int i=0; i < people.length(); ++i)
    {
        QMap<QString,QVariant> person = people.at(i);
        
        QTableWidgetItem *mail = new QTableWidgetItem();
        QTableWidgetItem *prayer = new QTableWidgetItem();
        if (person["agreed_mail"] == 1)
        {
            mail->setBackgroundColor(Qt::black);
        }
        
        if (person["agreed_prayer"] == 1)
        {
            prayer->setBackgroundColor(Qt::black);
        }
        
        QPushButton *button_edit = new QPushButton("edit");
        button_edit->setMaximumWidth(40);
        
        this->table_widget->setCellWidget(i, 0, button_edit);
        this->table_widget->setItem(i, 1, new QTableWidgetItem(person["name"].toString()));
        this->table_widget->setItem(i, 2, new QTableWidgetItem(person["group"].toString()));
        this->table_widget->setItem(i, 3, new QTableWidgetItem(person["email"].toString()));
        this->table_widget->setItem(i, 4, mail);
        this->table_widget->setItem(i, 5, prayer);
        this->table_widget->setItem(i, 6, new QTableWidgetItem(person["agreement"].toString()));
    }
    
    QStringList labels;
    labels << "" << "name" << "group" << "email" << "m" << "p" << "agreement";
    this->table_widget->setHorizontalHeaderLabels(labels);
    this->table_widget->resizeColumnsToContents();
    this->table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
