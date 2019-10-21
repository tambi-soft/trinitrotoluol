#include "mail_message.h"

MailMessage::MailMessage(QObject *parent) : QObject(parent)
{
    
}

void MailMessage::setSMTPPort(int smtp_port)
{
    this->smtp_port = smtp_port;
}

void MailMessage::setSMTPAddress(QString smtp_address)
{
    this->smtp_address = smtp_address;
}

void MailMessage::setSMTPUser(QString smtp_user)
{
    this->smtp_user = smtp_user;
}

void MailMessage::setSMTPPassword(QString smtp_password)
{
    this->smtp_password = smtp_password;
}

void MailMessage::addTo(QString address_to)
{
    this->address_to = address_to;
}

void MailMessage::setFrom(QString address_from)
{
    this->address_from = address_from;
}

void MailMessage::setSubject(QString subject)
{
    this->subject = subject;
}

void MailMessage::setAlternativeText(QString text)
{
    this->payload_text = text;
}

void MailMessage::setHTML(QString html)
{
    this->payload_html = html;
}

void MailMessage::addAttachment(QString attachment_path, QString filename)
{
    //this->attachments_base64.append();
}

void MailMessage::generateMessage()
{
    this->message = "To: "+ this->address_to +"\r\n"
            "From: "+ this->address_from +"\r\n"
            "Subject: "+ this->subject +"\r\n"
            "Content-Type: multipart/mixed; boundary=MixedBoundary"
            "\r\n"
            "--MixedBoundary"
            "Content-Type: multipart/alternative; boundary=AlternativeBoundary"
            "\r\n"
            "--AlternativeBoundary"
            "Content-Type: text/plain; charset=\"utf-8\""
            "\r\n"+ this->payload_text +"\r\n"
            "--AlternativeBoundary"
            "Content-Type: text/html; charset=\"utf-8\""
            "\r\n"+ this->payload_html +"\r\n"
            "--AlternativeBoundary--"
            "\r\n"
            "--MixedBoundary";
    /*
            "Content-Type: application/pdf; name=\"HelloWorld.pdf\""
            "Content-Transfer-Encoding: base64"
            "Content-Disposition: attachment; filename=\"HelloWorld.pdf\"";
    */
    this->message.append("\r\n"
            "--MixedBoundary--");
}

int MailMessage::sendMail()
{
    
    
    return 0;
}
