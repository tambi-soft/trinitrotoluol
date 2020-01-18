#include "donations_import_progress.h"

DonationsImportProgress::DonationsImportProgress(DbAdapter *db, QList<QMap<QString,QString>> data, QMap<QString, QString> data_map, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(this->layout);
    
    for (int i=0; i < data.length(); ++i)
    {
        
    }
}
