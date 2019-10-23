#include "mail_send.h"

MailSend::MailSend(DbAdapter *db, qlonglong rowid, QWidget *parent)
    : QWidget(parent)
{
    this->db = db;
    this->rowid = rowid;
    
    setLayout(this->grid);
    
    addRecipientsArea();
    addControlsArea();
    addPreviewArea();
    
    this->grid->addWidget(this->recipients, 0, 0);
    this->grid->addWidget(this->controls, 0, 1);
    this->grid->addWidget(this->preview, 1, 0, 1, 2);
}

void MailSend::addRecipientsArea()
{
    QGridLayout *grid = new QGridLayout;
    this->recipients_widget = new QWidget;
    this->recipients_widget->setLayout(grid);
    this->recipients->setWidgetResizable(true);
    this->recipients->setWidget(this->recipients_widget);
    
    this->recipients_widget->setMaximumWidth(500);
    
    QList<QMap<QString,QVariant>> people = this->db->selectAllPersonsForMail(this->flag_mail_agreed);
    
    this->list_checkboxes.clear();
    this->list_emails.clear();
    
    for (int i=0; i < people.length(); ++i)
    {
        QMap<QString,QVariant> person = people.at(i);
        QString name = person["name"].toString();
        QString email = person["email"].toString();
        
        QLabel *label_name = new QLabel(name);
        QLabel *label_email = new QLabel(email);
        
        QCheckBox *check_send_mail = new QCheckBox;
        if (email.length() > 0)
        {
            check_send_mail->setChecked(true);
        }
        
        this->list_checkboxes.append(check_send_mail);
        this->list_emails.append(email);
        
        grid->addWidget(check_send_mail, i, 0);
        grid->addWidget(label_name, i, 1);
        grid->addWidget(label_email, i, 2);
    }
}

void MailSend::addControlsArea()
{
    QVBoxLayout *layout = new QVBoxLayout;
    this->controls->setLayout(layout);
    
    QPushButton *button_show_agreed = new QPushButton("show agreed");
    QPushButton *button_show_not_agreed = new QPushButton("show not agreed");
    
    QPushButton *button_select_all = new QPushButton("select all");
    QPushButton *button_deselect_all = new QPushButton("deselect all");
    
    QPushButton *button_send_mail = new QPushButton("SEND MAIL!");
    
    connect(button_show_agreed, &QPushButton::clicked, this, &MailSend::showAgreed);
    connect(button_show_not_agreed, &QPushButton::clicked, this, &MailSend::showNotAgreed);
    
    connect(button_select_all, &QPushButton::clicked, this, &MailSend::selectAll);
    connect(button_deselect_all, &QPushButton::clicked, this, &MailSend::deselectAll);
    
    connect(button_send_mail, &QPushButton::clicked, this, &MailSend::sendMail);
    
    layout->addWidget(button_show_agreed);
    layout->addWidget(button_show_not_agreed);
    layout->addWidget(new QLabel);
    layout->addWidget(button_select_all);
    layout->addWidget(button_deselect_all);
    layout->addWidget(new QLabel);
    layout->addWidget(button_send_mail);
}

void MailSend::addPreviewArea()
{
    this->mail = this->db->selectMail(this->rowid);
    QString cover = mail["cover"].toString();
    QString content_path = mail["content_path"].toString();
    
    this->preview->updateContent(cover, content_path);
}

void MailSend::showAgreed()
{
    this->flag_mail_agreed = true;
    
    this->recipients_widget->deleteLater();
    addRecipientsArea();
}

void MailSend::showNotAgreed()
{
    this->flag_mail_agreed = false;
    
    this->recipients_widget->deleteLater();
    addRecipientsArea();
}

void MailSend::selectAll()
{
    for (int i=0; i < this->list_checkboxes.length(); ++i)
    {
        this->list_checkboxes.at(i)->setChecked(true);
    }
}

void MailSend::deselectAll()
{
    for (int i=0; i < this->list_checkboxes.length(); ++i)
    {
        this->list_checkboxes.at(i)->setChecked(false);
    }
}

void MailSend::sendMail()
{
    int reply = QMessageBox::question(this, "Send Mail", "This will really and actually send your Mail to all these selected People! Proceed?", QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        // gather all emails checked in the gui
        QList<QString> emails;
        for (int i=0; i < this->list_emails.length(); ++i)
        {
            if (this->list_checkboxes.at(i)->isChecked())
            {
                emails.append(this->list_emails.at(i));
            }
        }
        
        qDebug() << emails;
        
        MailMessage *message = new MailMessage;
        message->setSMTPPort(this->db->selectSettings("email_port"));
        message->setSMTPAddress(this->db->selectSettings("email_server"));
        message->setSMTPUser(this->db->selectSettings("email_username"));
        
        SimpleCrypt processSimpleCrypt(KEY);
        QString email_pw_dec = processSimpleCrypt.decryptToString(this->db->selectSettings("email_password"));
        message->setSMTPPassword(email_pw_dec);
        
        message->addTo(emails);
        message->setFrom(this->db->selectSettings("email_username"));
        message->setReplyTo(this->db->selectSettings("email_reply"));
        message->setSubject(this->mail["subject"].toString());
        
        message->setAlternativeText(this->mail["alternative_text"].toString());
        message->setHTML(this->preview->html);
        message->addAttachment(this->mail["attachment_path"].toString());
        
        message->generateMessage();
        //message->saveMessage("/tmp/bla.eml");
        
        message->sendMailWithExternalCURL();
        
        /*
        int error_happened = message->sendMail();
        if (error_happened == 0)
        {
            //this->db->insertMailSent();
        }
        */
    }
}
