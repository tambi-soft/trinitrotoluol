#include "vcard.h"

VCardExport::VCardExport(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    selectNewFileDialog();
}

void VCardExport::selectNewFileDialog()
{
    QString dir_new = QFileDialog::getSaveFileName(this,
                                                   "Select Export File",
                                                   QDir::homePath() + "/" + "trinitrotoluol_exported.vcf",
                                                   "",
                                                   new QString());
    
    // if the user hits "cancel", we have an empty string here
    if (!dir_new.isEmpty() || !dir_new.isNull())
    {
        VCard *vcard = new VCard(this->db);
        vcard->exportVCF(dir_new);
    }
}



VCardImport::VCardImport(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    
}



VCard::VCard(DbAdapter *db, QObject *parent) : QObject(parent)
{
    this->db = db;
}

void VCard::exportVCF(QString filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
        
    QList<QMap<QString,QVariant>> people = this->db->personSelectExport();
    for (int i=0; i < people.length(); i++)
    {
        QMap<QString,QVariant> person = people.at(i);
        QList<QMap<QString,QVariant>> people_vcf = this->db->personSelectVCardExport(person["rowid"].toLongLong());
        
        
        
        QTextStream out(&file);
        out << "BEGIN:VCARD" << "\n" <<
               "VERSION:4.0" << "\n";
        
        out << "FN:" << person["name"].toString() << "\n";
        out << "EMAIL:" << person["email"].toString() << "\n";
        out << "ADR:" << person["address"].toString() << "\n";
        out << "TEL:" << person["phone"].toString() << "\n";
        out << "NOTE:" << person["notes"].toString().replace("\n", "\\n") << "\n";
        
        for (int j=0; j < people_vcf.length(); j++)
        {
            QMap<QString,QVariant> vcf = people_vcf.at(j);
            
            out << vcf["key"].toString() << ":" << vcf["value"].toString() << "\n";
        }
        
        out << "END:VCARD" << "\n";
    }
    
    file.close();
}
