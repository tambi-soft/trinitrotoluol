#include "donations_import_progress.h"

DonationsImportProgress::DonationsImportProgress(DbAdapter *db, QList<QMap<QString,QString>> data, QMap<QString,QString> data_map, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(this->layout);
    
    QProgressBar *progressbar = new QProgressBar;
    progressbar->setRange(0, data.length());
    
    this->layout->addWidget(progressbar);
    
    qDebug() << data;
    
    /*
    data_map["donor_name"] = "DonorName";
    data_map["donor_tnt_code"] = "DonorCode";
    data_map["amount"] = "Amount";
    data_map["currency_code"] = "TenderedCurreny";
    data_map["date"] = "GiftDate";
    */
    
    for (int i=0; i < data.length(); ++i)
    {
        progressbar->setValue(i);
        
        /*
        TODO:
          - check, if entry already exists
          - check if person exists; create new Person, if not (show create person mask to user)
          - check if currency exists; show "create new currency" if not
        */
        
        qlonglong rowid_people = this->db->personRowidForTNTCode(data.at(i)[data_map["person_tnt_code"]]);
        qlonglong rowid_currency = this->db->currencyROWIDForCode(data.at(i)[data_map["currency_code"]]);
        
        QMap<QString,QVariant> insert_row_data;
        insert_row_data["rowid_people"] = rowid_people;
        insert_row_data["person_name"] = data.at(i)[data_map["person_name"]];
        insert_row_data["amount"] = data.at(i)[data_map["amount"]];
        insert_row_data["rowid_currencies"] = rowid_currency;
        insert_row_data["date"] = data.at(i)[data_map["date"]];
        insert_row_data["memo"] = data.at(i)[data_map["memo"]];
        
        qDebug() << insert_row_data;
        
        this->db->donationInsert(insert_row_data);
    }
    
    progressbar->setValue(data.length());
}
