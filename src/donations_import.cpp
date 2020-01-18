#include "donations_import.h"

DonationsImport::DonationsImport(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(this->layout);
    
    this->layout->addWidget(this->stack);
    
    this->stack_select_source = stackSelectSourceFormat();
    
    this->stack->addWidget(this->stack_select_source);
    
    
}

QWidget *DonationsImport::stackSelectSourceFormat()
{
    QWidget *widget = new QWidget;
    
    QVBoxLayout *lay = new QVBoxLayout;
    widget->setLayout(lay);
    
    // multiple file open dialog with filter "csv"
    // (more formats maybe later)
    QPushButton *button_import_csv = new QPushButton("import CSV");
    connect(button_import_csv, &QPushButton::clicked, this, &DonationsImport::showCSVFileDialog);
    
    lay->addWidget(button_import_csv);
    
    return widget;
}

void DonationsImport::showCSVFileDialog()
{
    QString last_import_path = this->db->selectSettings("last_csv_import_path");
    QString url_csv = QFileDialog::getOpenFileName(this, "Please select a CSV-File", last_import_path, "csv(*.csv *.CSV)");
    this->db->insertSettings("last_csv_import_path", url_csv);
    
    if (url_csv != nullptr)
    {   
        ParseCSV *parser = new ParseCSV;
        QList<QMap<QString,QString>> data = parser->processCSVFile(url_csv);
        
        /*
        this->stack_assignment_matrix = new DonationsImportMatrix(url_csv);
        this->stack->addWidget(this->stack_assignment_matrix);
        // activate this widget
        this->stack->setCurrentWidget(this->stack_assignment_matrix);
        */
        
        // [this slotnames] -> [csv colname]
        QMap<QString,QString> data_map;
        // for testing
        //data_map["rowid_people"]
        //data_map["amount"]
        //data_map["rowid_currencies"]
        //data_map["date"]
        
        
        this->stack_import_progress = new DonationsImportProgress(this->db, data, data_map);
        this->stack->addWidget(this->stack_import_progress);
        // activate
        this->stack->setCurrentWidget(this->stack_import_progress);
    }
}
