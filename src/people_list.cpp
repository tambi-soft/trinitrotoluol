#include "people_list.h"

PeopleList::PeopleList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , table_widget (new QTabWidget)
{
    this->db = db;
    
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    layout->addWidget(this->table_widget);
    
    QPushButton *button_add_new_person = new QPushButton("add new person");
    layout->addWidget(button_add_new_person);
    
    
    
    this->db->insertNewPerson("name", "group", "email", "add", "phone");
}
