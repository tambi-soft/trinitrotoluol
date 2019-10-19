#include "mail_list.h"

MailList::MailList(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , table (new QTableWidget)
{
    this->db = db;
    
    setLayout(this->layout);
 
    this->data = this->db->selectAllMails();
    
    this->layout->addWidget(this->table);
    
    //initView();
    
    QPushButton *button_new_mail = new QPushButton("new Mail");
    connect(button_new_mail, &QPushButton::clicked, this, &MailList::onNewMail);
    this->layout->addWidget(button_new_mail);
}

void MailList::initView()
{
    QStringList headers;
    headers << "" << "" << "" << "number" << "subject" << "date of creation" << "date last edit";
    this->table->setColumnCount(headers.length());
    this->table->setHorizontalHeaderLabels(headers);
    
    this->data = this->db->selectAllMails();
    
    if (this->data.length() > 0)
    {
        this->table->setRowCount(this->data.length());
        
        for (int i=0; i < data.length(); ++i)
        {
            qlonglong rowid = data.at(i)["rowid"].toLongLong();
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
            connect(button_send, &QPushButton::clicked, this, [this, rowid]{ onEditMail(rowid); });
            
            this->table->setCellWidget(i, 0, button_delete);
            this->table->setCellWidget(i, 1, button_edit);
            this->table->setCellWidget(i, 2, button_send);
            this->table->setItem(i, 3, new QTableWidgetItem(data.at(i)["number"].toString()));
            this->table->setItem(i, 4, new QTableWidgetItem(subject));
            this->table->setItem(i, 5, new QTableWidgetItem(data.at(i)["date"].toString()));
            this->table->setItem(i, 6, new QTableWidgetItem(data.at(i)["date_last_edit"].toString()));
        }
        
        this->table->resizeColumnsToContents();
    }
}

void MailList::onNewMail()
{
    qlonglong rowid = this->db->insertNewMail();
    
    emit signalEditMail(rowid);
    
    this->table->clear();
    initView();
}

void MailList::onEditMail(qlonglong rowid)
{
    emit signalEditMail(rowid);
    
    this->table->clear();
    initView();
}

void MailList::onDeleteMail(qlonglong rowid, QString subject)
{
    int reply = QMessageBox::question(this, "Delete "+subject, "Really delete \""+subject+"\"?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        this->db->deleteMail(rowid);
        
        this->table->clear();
        initView();
    }
}

void MailList::showEvent(QShowEvent */*event*/)
{
    this->table->clear();
    initView();
}
