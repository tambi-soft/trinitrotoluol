#include "mail_list.h"

MailList::MailList(DbAdapter *db, GridWidget *parent) : GridWidget(parent)
{
    this->db = db;
    
    QPushButton *button_new_mail = new QPushButton("new Mail");
    connect(button_new_mail, &QPushButton::clicked, this, &MailList::onNewMail);
    this->layout->addWidget(button_new_mail);
}

void MailList::showData()
{
    recreateView();
    
    this->grid->addWidget(new QLabel("<b>Number</b>"), 0, 3);
    this->grid->addWidget(new QLabel("<b>Subject</b>"), 0, 4);
    this->grid->addWidget(new QLabel("<b>Date of Creation</b>"), 0, 5);
    this->grid->addWidget(new QLabel("<b>Date of last Edit</b>"), 0, 6);
    
    this->data = this->db->selectAllMails();
    
    for (int i=0; i < data.length(); ++i)
    {
        qlonglong rowid = data.at(i)["rowid"].toLongLong();
        QString number = data.at(i)["number"].toString();
        QString subject = data.at(i)["subject"].toString();
        
        QPushButton *button_delete = new QPushButton();
        button_delete->setIcon(QIcon::fromTheme("edit-delete"));
        button_delete->setToolTip("delete this email");
        connect(button_delete, &QPushButton::clicked, this, [this, rowid, subject]{ onDeleteMail(rowid, subject); });
        
        QPushButton *button_edit = new QPushButton();
        button_edit->setIcon(QIcon::fromTheme("document-properties"));
        button_edit->setToolTip("edit this email");
        connect(button_edit, &QPushButton::clicked, this, [this, rowid]{ onEditMail(rowid); });
        
        QPushButton *button_send = new QPushButton();
        button_send->setIcon(QIcon::fromTheme("document-send"));
        button_send->setToolTip("send this email");
        connect(button_send, &QPushButton::clicked, this, [this, rowid, number]{ onSendMail(rowid, number); });
        
        this->grid->addWidget(button_delete, i+1, 0);
        this->grid->addWidget(button_edit, i+1, 1);
        this->grid->addWidget(button_send, i+1, 2);
        this->grid->addWidget(new QLabel(number), i+1, 3);
        this->grid->addWidget(new QLabel(subject), i+1, 4);
        this->grid->addWidget(new QLabel(data.at(i)["date"].toString()), i+1, 5);
        this->grid->addWidget(new QLabel(data.at(i)["date_last_edit"].toString()), i+1, 6);
    }
}

void MailList::onNewMail()
{
    qlonglong rowid = this->db->insertNewMail();
    
    emit signalEditMail(rowid);
    
    showData();
}

void MailList::onEditMail(qlonglong rowid)
{
    emit signalEditMail(rowid);
    
    //this->table->clear();
    //initView();
}

void MailList::onSendMail(qlonglong rowid, QString number)
{
    emit signalSendMail(rowid, number);
}

void MailList::onDeleteMail(qlonglong rowid, QString subject)
{
    int reply = QMessageBox::question(this, "Delete "+subject, "Really delete \""+subject+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->deleteMail(rowid);
        
        showData();
    }
}

void MailList::showEvent(QShowEvent */*event*/)
{
    showData();
}
