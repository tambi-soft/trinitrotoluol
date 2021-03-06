#include "mail_edit.h"

MailEdit::MailEdit(DbAdapter *db, qlonglong rowid, QWidget* parent) : QWidget(parent)
{
    this->db = db;
    this->rowid = rowid;
    
    initializeGUI();
    loadData();
    addEditListeners();
    updatePreview();
}

void MailEdit::initializeGUI()
{
    this->grid = new QGridLayout;
    this->line_number = new QLineEdit;
    this->line_subject = new QLineEdit;
    
    this->line_alternative->setMaximumHeight(200);
    this->line_cover->setMaximumHeight(200);
    
    this->line_content = new QLineEdit;
    this->line_content->setClearButtonEnabled(true);
    this->line_content->setPlaceholderText("select Path to a Text/HTML file");
    
    this->line_attachment_one = new QLineEdit;
    //this->line_attachment_one->setEnabled(false);
    this->line_attachment_one->setPlaceholderText("select Path to a File to be sent as attachment");
    this->line_attachment_one->setClearButtonEnabled(true);
    
    this->line_attachment_two = new QLineEdit;
    //this->line_attachment_two->setEnabled(false);
    this->line_attachment_two->setPlaceholderText("select Path to a File to be sent as attachment");
    this->line_attachment_two->setClearButtonEnabled(true);
    
    this->line_date = new QLineEdit;
    this->line_date->setReadOnly(true);
    this->line_date->setPlaceholderText("will be inserted automatically");
    
    this->line_date_last_edit = new QLineEdit;
    this->line_date_last_edit->setReadOnly(true);
    this->line_date_last_edit->setPlaceholderText("will be inserted automatically");
            
    setLayout(this->grid);
    
    this->grid->addWidget(new QLabel("Number:"), 0, 0);
    this->grid->addWidget(this->line_number, 0, 1, 1, 3);
    
    this->grid->addWidget(new QLabel("Subject:"), 1, 0);
    this->grid->addWidget(this->line_subject, 1, 1, 1, 3);
    
    this->grid->addWidget(new QLabel("Non-HTML Message\nAlternative:"), 2, 0);
    this->grid->addWidget(this->line_alternative, 2, 1, 1, 3);
    
    this->grid->addWidget(new QLabel("Cover:"), 3, 0);
    this->grid->addWidget(this->line_cover, 3, 1, 1, 3);
    
    this->grid->addWidget(new QLabel("Content:"), 4, 0);
    this->grid->addWidget(this->line_content, 4, 1);
    
    QPushButton *button_add_content = new QPushButton("select Content Path");
    connect(button_add_content, &QPushButton::clicked, this, &MailEdit::onContentPathButton);
    this->grid->addWidget(button_add_content, 4, 2);
    
    this->grid->addWidget(new QLabel("Attachment 1:"), 5, 0);
    this->grid->addWidget(this->line_attachment_one, 5, 1);
    
    this->grid->addWidget(new QLabel("Attachment 2:"), 6, 0);
    this->grid->addWidget(this->line_attachment_two, 6, 1);
    
    QPushButton *button_add_attachment_one = new QPushButton("select Attachment Path");
    connect(button_add_attachment_one, &QPushButton::clicked, this, &MailEdit::onAttachmentOnePathButton);
    this->grid->addWidget(button_add_attachment_one, 5, 2);
    
    QPushButton *button_add_attachment_two = new QPushButton("select Attachment Path");
    connect(button_add_attachment_two, &QPushButton::clicked, this, &MailEdit::onAttachmentTwoPathButton);
    this->grid->addWidget(button_add_attachment_two, 6, 2);
    
    this->grid->addWidget(new QLabel("Date Created:"), 7, 0);
    this->grid->addWidget(this->line_date, 7, 1, 1, 3);
    this->grid->addWidget(new QLabel("Date last Edit:"), 8, 0);
    this->grid->addWidget(this->line_date_last_edit, 8, 1, 1, 3);
    
    this->grid->addWidget(this->preview, 9, 0, 1, 3);
    //this->preview->setReadOnly(true);
}

void MailEdit::loadData()
{
    QMap<QString,QVariant> data = this->db->selectMail(this->rowid);
    // number, subject, cover, content_path, attachment_path, date
    
    this->line_number->setText(data["number"].toString());
    this->line_subject->setText(data["subject"].toString());
    this->line_alternative->setPlainText(data["alternative_text"].toString());
    this->line_cover->setPlainText(data["cover"].toString());
    this->line_content->setText(data["content_path"].toString());
    this->line_attachment_one->setText(data["attachment_path_one"].toString());
    this->line_attachment_two->setText(data["attachment_path_two"].toString());
    this->line_date->setText(data["date"].toString());
    this->line_date_last_edit->setText(data["date_last_edit"].toString());
}

void MailEdit::addEditListeners()
{
    connect(this->line_number, &QLineEdit::textChanged, this, &MailEdit::saveData);
    connect(this->line_subject, &QLineEdit::textChanged, this, &MailEdit::saveData);
    connect(this->line_alternative, &QPlainTextEdit::textChanged, this, &MailEdit::saveData);
    connect(this->line_cover, &QPlainTextEdit::textChanged, this, &MailEdit::saveData);
    connect(this->line_content, &QLineEdit::textChanged, this, &MailEdit::saveData);
    connect(this->line_attachment_one, &QLineEdit::textChanged, this, &MailEdit::saveData);
    connect(this->line_attachment_two, &QLineEdit::textChanged, this, &MailEdit::saveData);
    
    connect(this->line_cover, &QPlainTextEdit::textChanged, this, &MailEdit::updatePreview);
    connect(this->line_content, &QLineEdit::textChanged, this, &MailEdit::updatePreview);
}

void MailEdit::saveData()
{
    QMap<QString,QVariant> data;
    data["number"] = this->line_number->text();
    data["subject"] = this->line_subject->text();
    data["alternative_text"] = this->line_alternative->toPlainText();
    data["cover"] = this->line_cover->toPlainText();
    data["content_path"] = this->line_content->text();
    data["attachment_path_one"] = this->line_attachment_one->text();
    data["attachment_path_two"] = this->line_attachment_two->text();
    
    this->db->updateMail(this->rowid, data);
}

void MailEdit::onContentPathButton()
{
    QString last_mail_path = this->db->selectSettings("last_mail_path");
    last_mail_path = QFileDialog::getOpenFileName(this, tr("Select Directory"),
            last_mail_path);
    
    this->line_content->setText(last_mail_path);
    
    this->db->insertSettings("last_mail_path", last_mail_path);
}

void MailEdit::onAttachmentOnePathButton()
{
    QString last_mail_path = this->db->selectSettings("last_mail_path");
    last_mail_path = QFileDialog::getOpenFileName(this, tr("Select Directory"),
            last_mail_path);
    
    this->line_attachment_one->setText(last_mail_path);
    
    this->db->insertSettings("last_mail_path", last_mail_path);
}

void MailEdit::onAttachmentTwoPathButton()
{
    QString last_mail_path = this->db->selectSettings("last_mail_path");
    last_mail_path = QFileDialog::getOpenFileName(this, tr("Select Directory"),
            last_mail_path);
    
    this->line_attachment_two->setText(last_mail_path);
    
    this->db->insertSettings("last_mail_path", last_mail_path);
}

void MailEdit::updatePreview()
{
    qDebug() << "update";
    this->preview->updateContent(this->line_cover->toPlainText(), this->line_content->text());
}
