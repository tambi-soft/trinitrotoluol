#include "donations_import.h"

DonationsImport::DonationsImport(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(this->layout);
    
    this->layout->addWidget(this->stack);
    
    this->stack_select_source = stackSelectSourceFormat();
    this->stack_assignment_matrix_csv = stackAssignmentMatrixCSV();
    this->stack_import_progress = stackImportProgress();
    
    this->stack->addWidget(this->stack_select_source);
    this->stack->addWidget(this->stack_assignment_matrix_csv);
    this->stack->addWidget(this->stack_import_progress);
    
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

QWidget *DonationsImport::stackAssignmentMatrixCSV()
{
    QWidget *widget = new QWidget;
    
    QVBoxLayout *lay = new QVBoxLayout;
    widget->setLayout(lay);
    
    //https://stackoverflow.com/questions/27318631/parsing-through-a-csv-file-in-qt
    
    return widget;
}

QWidget *DonationsImport::stackImportProgress()
{
    QWidget *widget = new QWidget;
    
    QVBoxLayout *lay = new QVBoxLayout;
    widget->setLayout(lay);
    
    QProgressBar *progress = new QProgressBar;
    
    QTextEdit *edit_log = new QTextEdit;
    
    lay->addWidget(progress);
    lay->addWidget(edit_log);
    
    return widget;
}

void DonationsImport::showCSVFileDialog()
{
    QString url_csv = QFileDialog::getOpenFileName(this, "Please select a CSV-File");
    if (url_csv != nullptr)
    {
        this->filepath = url_csv;
        this->stack->setCurrentWidget(this->stack_assignment_matrix_csv);
    }
}
