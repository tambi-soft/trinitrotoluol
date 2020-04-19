#include "export.h"

Export::Export(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(new QHBoxLayout);
    
    addSelectSource();
}

void Export::addSelectSource()
{
    QGroupBox *group = new QGroupBox("Email Settings");
    QVBoxLayout *layout = new QVBoxLayout;
    group->setLayout(layout);
    this->layout()->addWidget(group);
    
    QList<QMap<QString,QVariant>> tables = this->db->SelectTableNames();
    
    for (int i=0; i < tables.length(); i++)
    {
        QRadioButton *radio = new QRadioButton(tables.at(i)["name"].toString());
        layout->addWidget(radio);
    }
}
