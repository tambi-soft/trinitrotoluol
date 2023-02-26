#ifndef DonationsImportTNT_H
#define DonationsImportTNT_H

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
#include "donations_import_matrix.h"
#include "donations_import_progress.h"
#include "parse_csv.h"
#include "lib/growing_text_edit.h"

class DonationsImportTNT : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsImportTNT(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QVBoxLayout *layout = new QVBoxLayout;
    QStackedWidget *stack = new QStackedWidget;
    
    QWidget *stackSelectSourceFormat();
    
    void showCSVFileDialog();
    
    QWidget *stack_select_source = nullptr;
    QWidget *stack_assignment_matrix = nullptr;
    QWidget *stack_import_progress = nullptr;
    
    QString filepath;
    
signals:
    
};

#endif // DonationsImportTNT_H
