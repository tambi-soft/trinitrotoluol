#include "mail_new.h"

MailNew::MailNew(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    initializeGUI();
}

MailNew::MailNew(DbAdapter *db, qlonglong rowid, QWidget* parent) : QWidget(parent)
{
    this->db = db;
    this->rowid = rowid;
    
    initializeGUI();
}

void MailNew::initializeGUI()
{
    this->grid = new QGridLayout;
    this->line_number = new QLineEdit;
    this->line_subject = new QLineEdit;
    this->line_cover = new QTextEdit;
    
    this->line_content = new QLineEdit;
    this->line_content->setReadOnly(true);
    this->line_content->setPlaceholderText("select Path to a Text/HTML file");
    
    this->line_attachment = new QLineEdit;
    this->line_attachment->setReadOnly(true);
    this->line_attachment->setPlaceholderText("select Path to a File to be sent as attachment");
    
    this->line_date = new QLineEdit;
    this->line_date->setReadOnly(true);
    this->line_date->setPlaceholderText("will be inserted automatically");
            
    setLayout(this->grid);
    
    this->grid->addWidget(new QLabel("Number:"), 0, 0);
    this->grid->addWidget(this->line_number, 0, 1);
    
    this->grid->addWidget(new QLabel("Subject:"), 1, 0);
    this->grid->addWidget(line_subject, 1, 1);
    
    this->grid->addWidget(new QLabel("Cover:"), 2, 0);
    this->grid->addWidget(line_cover, 2, 1);
    
    this->grid->addWidget(new QLabel("Content:"), 3, 0);
    this->grid->addWidget(line_content, 3, 1);
    QPushButton *button_add_content = new QPushButton("select Content Path");
    connect(button_add_content, &QPushButton::clicked, this, &MailNew::onContentPathButton);
    this->grid->addWidget(button_add_content, 3, 2);
    
    this->grid->addWidget(new QLabel("Attachment:"), 4, 0);
    this->grid->addWidget(line_attachment, 4, 1);
    QPushButton *button_add_attachment = new QPushButton("select Attachment Path");
    connect(button_add_attachment, &QPushButton::clicked, this, &MailNew::onAttachmentPathButton);
    this->grid->addWidget(button_add_attachment, 4, 2);
    
    this->grid->addWidget(new QLabel("Date:"), 5, 0);
    this->grid->addWidget(line_date, 5, 1);
    
    QPushButton* button_cancel = new QPushButton("Cancel");
    this->grid->addWidget(button_cancel, 6, 0);
    
    QPushButton* button_save = new QPushButton("Save");
    this->grid->addWidget(button_save, 6, 1);
}

void MailNew::onSaveButton()
{
    QMap<QString,QVariant> data;
    data["number"] = this->line_number->text();
    data["subject"] = this->line_subject->text();
    data["cover"] = this->line_cover->toHtml();
    data["content_path"] = this->line_content->text();
    data["attachment_path"] = this->line_attachment->text();
    
    this->db->insertNewMail(data);
}

void MailNew::onContentPathButton()
{
    QString last_mail_path = this->db->selectSettings("last_mail_path");
    last_mail_path = QFileDialog::getOpenFileName(this, tr("Select Directory"),
            last_mail_path);
    
    this->line_content->setText(last_mail_path);
    
    this->db->insertSettings("last_mail_path", last_mail_path);
}

void MailNew::onAttachmentPathButton()
{
    QString last_mail_path = this->db->selectSettings("last_mail_path");
    last_mail_path = QFileDialog::getOpenFileName(this, tr("Select Directory"),
            last_mail_path);
    
    this->line_attachment->setText(last_mail_path);
    
    this->db->insertSettings("last_mail_path", last_mail_path);
}
