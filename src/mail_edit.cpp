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
    this->line_cover = new QTextEdit;
    this->line_cover->setMaximumHeight(200);
    
    this->line_content = new QLineEdit;
    this->line_content->setEnabled(false);
    this->line_content->setPlaceholderText("select Path to a Text/HTML file");
    
    this->line_attachment = new QLineEdit;
    this->line_attachment->setEnabled(false);
    this->line_attachment->setPlaceholderText("select Path to a File to be sent as attachment");
    
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
    
    this->grid->addWidget(new QLabel("Cover:"), 2, 0);
    this->grid->addWidget(this->line_cover, 2, 1, 1, 3);
    
    this->grid->addWidget(new QLabel("Content:"), 3, 0);
    this->grid->addWidget(this->line_content, 3, 1);
    
    QPushButton *button_add_content = new QPushButton("select Content Path");
    connect(button_add_content, &QPushButton::clicked, this, &MailEdit::onContentPathButton);
    this->grid->addWidget(button_add_content, 3, 2);
    
    this->grid->addWidget(new QLabel("Attachment:"), 4, 0);
    this->grid->addWidget(this->line_attachment, 4, 1);
    
    QPushButton *button_add_attachment = new QPushButton("select Attachment Path");
    connect(button_add_attachment, &QPushButton::clicked, this, &MailEdit::onAttachmentPathButton);
    this->grid->addWidget(button_add_attachment, 4, 2);
    
    this->grid->addWidget(new QLabel("Date Created:"), 5, 0);
    this->grid->addWidget(this->line_date, 5, 1, 1, 3);
    this->grid->addWidget(new QLabel("Date last Edit:"), 6, 0);
    this->grid->addWidget(this->line_date_last_edit, 6, 1, 1, 3);
    
    this->grid->addWidget(this->preview, 7, 0, 1, 3);
    //this->preview->setReadOnly(true);
}

void MailEdit::loadData()
{
    QMap<QString,QVariant> data = this->db->selectMail(this->rowid);
    // number, subject, cover, content_path, attachment_path, date
    
    this->line_number->setText(data["number"].toString());
    this->line_subject->setText(data["subject"].toString());
    this->line_cover->setText(data["cover"].toString());
    this->line_content->setText(data["content_path"].toString());
    this->line_attachment->setText(data["attachment_path"].toString());
    this->line_date->setText(data["date"].toString());
    this->line_date_last_edit->setText(data["date_last_edit"].toString());
}

void MailEdit::addEditListeners()
{
    connect(this->line_number, &QLineEdit::textChanged, this, &MailEdit::saveData);
    connect(this->line_subject, &QLineEdit::textChanged, this, &MailEdit::saveData);
    connect(this->line_cover, &QTextEdit::textChanged, this, &MailEdit::saveData);
    connect(this->line_content, &QLineEdit::textChanged, this, &MailEdit::saveData);
    connect(this->line_attachment, &QLineEdit::textChanged, this, &MailEdit::saveData);
    
    connect(this->line_cover, &QTextEdit::textChanged, this, &MailEdit::updatePreview);
    connect(this->line_content, &QLineEdit::textChanged, this, &MailEdit::updatePreview);
    connect(this->line_attachment, &QLineEdit::textChanged, this, &MailEdit::updatePreview);
}

void MailEdit::saveData()
{
    QMap<QString,QVariant> data;
    data["number"] = this->line_number->text();
    data["subject"] = this->line_subject->text();
    data["cover"] = this->line_cover->toHtml();
    data["content_path"] = this->line_content->text();
    data["attachment_path"] = this->line_attachment->text();
    
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

void MailEdit::onAttachmentPathButton()
{
    QString last_mail_path = this->db->selectSettings("last_mail_path");
    last_mail_path = QFileDialog::getOpenFileName(this, tr("Select Directory"),
            last_mail_path);
    
    this->line_attachment->setText(last_mail_path);
    
    this->db->insertSettings("last_mail_path", last_mail_path);
}

void MailEdit::updatePreview()
{
    QFile file(this->line_content->text());
    if(!file.open(QIODevice::ReadOnly)) {
        //QMessageBox::information(nullptr, "error", file.errorString());
    }
    
    QTextStream in(&file);
    
    QString html;// = this->line_cover->toPlainText();
    while(!in.atEnd()) {
        QString line = in.readLine();
        
        if (line.trimmed() == "<!-- INCLUDE COVER -->")
        {
            html.append(this->line_cover->toPlainText());
        }
        else
        {
            //html.append(in.readLine());
            html.append(line);
        }
        
        /*
        QString line = in.readLine();    
        QStringList fields = line.split(",");    
        model->appendRow(fields);
        */
    }
    
    file.close();
    
    this->preview->setHtml(html);
}
