    #include "donations_import.h"

DonationsImport::DonationsImport(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(this->layout);
    
    this->layout->addWidget(this->stack);
    
    /* == Bypassing the selectSourceFormat, as we currently only support CSV == */
    this->stack_select_source = stackSelectSourceFormat();
    this->stack->addWidget(this->stack_select_source);
    //showCSVFileDialog();
}

QWidget *DonationsImport::stackSelectSourceFormat()
{
    QWidget *widget = new QWidget;
    
    QVBoxLayout *lay = new QVBoxLayout;
    widget->setLayout(lay);
    
    QTextEdit *help = showHelp();
    
    
    // multiple file open dialog with filter "csv"
    // (more formats maybe later)
    QPushButton *button_import_csv = new QPushButton("import CSV");
    connect(button_import_csv, &QPushButton::clicked, this, &DonationsImport::showCSVFileDialog);
    
    lay->addWidget(help);
    lay->addWidget(button_import_csv);
    
    return widget;
}

QTextEdit *DonationsImport::showHelp()
{
    QFile file(":help_tntware_import");
    
    QString lines;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            lines.append(stream.readLine() + "\n");
        }
    }
    file.close();
    
    //QTextEdit *text = new QTextEdit(this);
    GrowingTextEdit *text = new GrowingTextEdit;
    text->setText(lines);
    
    text->setReadOnly(true);
    
    return text;
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
        data_map["person_name"] = "DonorName";
        data_map["person_tnt_code"] = "DonorCode";
        data_map["amount"] = "Amount";
        data_map["currency_code"] = "TenderedCurrency";
        data_map["date"] = "GiftDate";
        data_map["memo"] = "Memo";
        data_map["tnt_code"] = "Code";
        data_map["tnt_donor_code"] = "DonorCode";
        
        
        this->stack_import_progress = new DonationsImportProgress(this->db, data, data_map);
        this->stack->addWidget(this->stack_import_progress);
        // activate
        this->stack->setCurrentWidget(this->stack_import_progress);
    }
}
