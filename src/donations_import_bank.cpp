#include "donations_import_bank.h"

DonationsImportBank::DonationsImportBank(Config *conf, DbAdapter *db, QWidget *parent)
    : QWidget{parent}
{
    this->conf = conf;
    this->db = db;
    
    this->text = new QTextEdit;
    connect(this->text, &QTextEdit::textChanged, this, &DonationsImportBank::parseCSV);
    this->table = new QTableWidget;
    
    QPushButton *button_write_to_db = new QPushButton("Write Values to DataBase");
    connect(button_write_to_db, &QPushButton::clicked, this, &DonationsImportBank::writeValueToDB);
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(this->text);
    layout->addWidget(this->table);
    layout->addWidget(button_write_to_db);
    setLayout(layout);
    
    openCSV();
    
    
}

void DonationsImportBank::openCSV()
{
    QString last_import_path = this->db->selectSettings("last_bank_csv_import_path");
    QString url_csv = QFileDialog::getOpenFileName(this, "Please select a CSV-File", last_import_path, "csv(*.csv *.CSV)");
    
    if (url_csv != nullptr)
    {
        this->db->insertSettings("last_bank_csv_import_path", url_csv);
        
        QFile file(url_csv);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << file.errorString();
        }
        else
        {
            QString csv_content;
            while (! file.atEnd())
            {
                QByteArray line_input_b = file.readLine();
                QString line_input = QString(line_input_b);
                // we want to filter out empty lines with just line break "\n"
                if (line_input.length() > 2)
                {
                    csv_content.append(line_input);
                }
            }
            
            //this->text->blockSignals(true);
            this->text->setText(csv_content);
            //this->text->blockSignals(false);
        }
    }
}

void DonationsImportBank::parseCSV()
{
    this->table->clearContents();
    
    ParseCSV *parser = new ParseCSV;
    QStringList text_splitted = this->text->toPlainText().split('\n',QString::SkipEmptyParts);
    this->data = parser->processCSVStringList(text_splitted);
    //qDebug() << data;
    
    QStringList keys = this->data.at(0).keys();
    //qDebug() << keys;
    
    this->table->setRowCount(this->data.length());
    this->table->setColumnCount(keys.length());
    
    /*
    for (int i=0; i < keys.length(); i++)
    {
        this->table->setItem(0, i, new QTableWidgetItem(keys.at(i)));
    }
    */
    
    this->table->setHorizontalHeaderLabels(keys);
    
    for (int x=0; x < this->data.length(); x++)
    {
        for (int y=0; y < keys.length(); y++)
        {
            this->table->setItem(x, y, new QTableWidgetItem(this->data.at(x)[keys.at(y)]));
        }
    }
}

QMap<QString, QVariant> DonationsImportBank::remapDataForDB(QMap<QString,QString> map)
{
    QMap<QString,QVariant> result;
    
    // check, if person is found in the map
    //QMap<QString,QVariant> person_found = this->db->personSelectDonationsMap(this->memo);
    //this->rowid_people = person_found["rowid_people"].toLongLong();
    
    qlonglong currency_rowid = this->db->currencyROWIDForCode(map["Waehrung"]);
    QString date_src = map["Valutadatum"];
    QString date = QDate::fromString(date_src, "d.M.yyyy").toString("yyyy-MM-dd");
    
    
    result["person_name"] = map["Name Zahlungsbeteiligter"];
    result["amount"] = map["Betrag"].replace(",", ".").toFloat();
    result["rowid_currencies"] = currency_rowid;
    result["date"] = date;
    result["memo"] = map["Verwendungszweck"];
    result["bank_name"] = map["Bankname Auftragskonto"];
    
    return result;
}

void DonationsImportBank::writeValueToDB()
{
    for (int i=0; i < this->data.length(); i++)
    {
        QMap<QString,QVariant> map = remapDataForDB(this->data.at(i));
        
        this->db->donationInsert(map);
    }
    
    QMessageBox *msgbox = new QMessageBox(this);
    msgbox->setWindowTitle("Import Completed");
    msgbox->setText("Import Data from CSV Completed");
    msgbox->open();
}
