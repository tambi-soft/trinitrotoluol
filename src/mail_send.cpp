#include "mail_send.h"

MailSend::MailSend(DbAdapter *db, qlonglong rowid, QWidget *parent)
    : QWidget(parent)
{
    this->db = db;
    this->rowid = rowid;
    
    setLayout(this->layout);
    
    addRecipientsArea();
    addPreviewArea();
    
    this->layout->addWidget(this->recipients);
    this->layout->addWidget(this->preview);
}

void MailSend::addRecipientsArea()
{
    QGridLayout *grid = new QGridLayout;
    QWidget *scroll_widget = new QWidget;
    scroll_widget->setLayout(grid);
    this->recipients->setWidgetResizable(true);
    this->recipients->setWidget(scroll_widget);
    
    scroll_widget->setMaximumWidth(500);
    
    QList<QMap<QString,QVariant>> people = this->db->selectAllPersonsForMail(1);
    
    for (int i=0; i < people.length(); ++i)
    {
        QMap<QString,QVariant> person = people.at(i);
        QString name = person["name"].toString();
        QString email = person["email"].toString();
        
        QLabel *label_name = new QLabel(name);
        QLabel *label_email = new QLabel(email);
        
        QCheckBox *check_send_mail = new QCheckBox;
        check_send_mail->setChecked(true);
        
        grid->addWidget(check_send_mail, i, 0);
        grid->addWidget(label_name, i, 1);
        grid->addWidget(label_email, i, 2);
    }
}

void MailSend::addPreviewArea()
{
    QMap<QString,QVariant> data = this->db->selectMail(this->rowid);
    QString cover = data["cover"].toString();
    QString content_path = data["content_path"].toString();
    
    this->preview->updateContent(cover, content_path);
}
