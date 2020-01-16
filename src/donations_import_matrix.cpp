#include "donations_import_matrix.h"

DonationsImportMatrix::DonationsImportMatrix(QString importfile_path, QWidget *parent) : QWidget(parent)
{
    setLayout(this->layout);
    
    if (importfile_path.toLower().endsWith(".csv"))
    {
        ParseCSV *parser = new ParseCSV;
        QList<QMap<QString,QString>> data = parser->processCSVFile(importfile_path);
    }
}

