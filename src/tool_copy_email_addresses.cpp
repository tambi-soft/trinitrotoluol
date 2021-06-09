#include "tool_copy_email_addresses.h"

ToolCopyEmailAddresses::ToolCopyEmailAddresses(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    this->layout = new QVBoxLayout;
    
    QTextEdit *output = new QTextEdit;
    
    this->layout->addWidget(output);
    
    this->setLayout(this->layout);
    
    int search_agreed = -1;
    QString search_name = "%";
    QList<QMap<QString,QVariant>> people = this->db->selectAllPersonsFiltered(-1,-1, -1, 0, search_agreed, "%", "%"+search_name+"%", "%");
    
    
    //qDebug() << people;
    for (int i = 0; i < people.length(); i++)
    {
        QString mail = people[i]["email"].toString();
        //qDebug() << mail;
        if (!mail.isEmpty())
        {
            output->append(mail + ",");
        }
    }
        
}
