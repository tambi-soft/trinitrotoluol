#ifndef DONATIONSIMPORT_H
#define DONATIONSIMPORT_H

#include <QObject>
#include <QWidget>
#include <QStackedWidget>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressBar>
#include <QFileDialog>

#include <QMap>
#include <QList>

#include "db_adapter.h"

class DonationsImport : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsImport(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QVBoxLayout *layout = new QVBoxLayout;
    QStackedWidget *stack = new QStackedWidget;
    
    QWidget *stackSelectSourceFormat();
    QWidget *stackAssignmentMatrixCSV();
    QWidget *stackImportProgress();
    
    void showCSVFileDialog();
    
    QWidget *stack_select_source;
    QWidget *stack_assignment_matrix_csv;
    QWidget *stack_import_progress;
    
    QString filepath;
    
signals:
    
};

#endif // DONATIONSIMPORT_H
