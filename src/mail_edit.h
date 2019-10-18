#ifndef MailEdit_H
#define MailEdit_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include <QFileDialog>
#include <QUrl>

#include "db_adapter.h"

class MailEdit : public QWidget
{
    Q_OBJECT
public:
    explicit MailEdit(DbAdapter *db, qlonglong rowid, QWidget* parent = nullptr);
    
private:
    qlonglong rowid = -1;
    DbAdapter *db;
    QGridLayout* grid;
    
    QLineEdit* line_number;
    QLineEdit* line_subject;
    QTextEdit* line_cover;
    QLineEdit* line_content;
    QLineEdit* line_attachment;
    QLineEdit* line_date;
    QLineEdit* line_date_last_edit;
    
    void initializeGUI();
    void loadData();
    void addEditListeners();
    
signals:
    
public slots:
    void saveData();
    void onContentPathButton();
    void onAttachmentPathButton();
};

#endif // MailEdit_H
