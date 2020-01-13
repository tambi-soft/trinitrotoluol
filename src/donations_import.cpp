#include "donations_import.h"

DonationsImport::DonationsImport(DbAdapter *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    
    setLayout(this->layout);
    
    this->layout->addWidget(this->stack);
    
    this->stack->addWidget(stackSelectSourceFormat());
    this->stack->addWidget(stackAssignmentMatrix());
    this->stack->addWidget(stackImportProgress());
}

QWidget *DonationsImport::stackSelectSourceFormat()
{
    QWidget *widget = new QWidget;
    
    
    
    return widget;
}

QWidget *DonationsImport::stackAssignmentMatrix()
{
    QWidget *widget = new QWidget;
    
    
    
    return widget;
}

QWidget *DonationsImport::stackImportProgress()
{
    QWidget *widget = new QWidget;
    
    
    
    return widget;
}
