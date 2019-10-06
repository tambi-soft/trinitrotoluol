#include "sqlite_editor_widget.h"

SQLiteEditorWidget::SQLiteEditorWidget(DbAdapter *db, QWidget *parent)
    : QWidget(parent)
    , grid (new QGridLayout)
{
    this->db = db;
    setLayout(this->grid);
    
    
}
