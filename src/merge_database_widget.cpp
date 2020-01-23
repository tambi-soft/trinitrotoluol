#include "merge_database_widget.h"

MergeDatabaseWidget::MergeDatabaseWidget(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    showFileOpenDialog();
}

void MergeDatabaseWidget::showFileOpenDialog()
{
    QString last_import_path = this->db->selectSettings("last_database_merge_path");
    QString url_db = QFileDialog::getOpenFileName(this, "Please select a Database File", last_import_path, "sqlite(*.sqlite)");
    
    if (url_db != nullptr)
    {   
        this->db->insertSettings("last_database_merge_path", url_db);
        
        
    }
    else
    {
        emit signalCloseTab();
    }
}
