#include "line_edit_vcard.h"

LineEditVCard::LineEditVCard(qlonglong user_id, DbAdapter *db, qlonglong vcard_id, QString vcard_key, QLineEdit *parent) : QLineEdit(parent)
{
    this->db = db;
    this->user_id = user_id;
    this->vcard_id = vcard_id;
    this->vcard_key = vcard_key;
    
    connect(this, &QLineEdit::textEdited, this, &LineEditVCard::saveText);
}

void LineEditVCard::saveText(QString text)
{
    this->db->personUpdateVCardEntry(this->vcard_id, text);
}
