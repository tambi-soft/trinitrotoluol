#ifndef SQLITEEDITORWIDGET_H
#define SQLITEEDITORWIDGET_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "db_adapter.h"

class SQLiteEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SQLiteEditorWidget(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QGridLayout *grid;
    
signals:
    
public slots:
};

#endif // SQLITEEDITORWIDGET_H
