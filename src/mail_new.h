#ifndef MAILNEW_H
#define MAILNEW_H

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

class MailNew : public QWidget
{
    Q_OBJECT
public:
    // new mail
    explicit MailNew(DbAdapter *db, QWidget *parent = nullptr);
    // edit mail
    explicit MailNew(DbAdapter *db, qlonglong rowid, QWidget* parent = nullptr);
    
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
    
    void initializeGUI();
    
signals:
    void closeCurrentTabSignal();
    
public slots:
    void onCancelButton();
    void onSaveButton();
    void onContentPathButton();
    void onAttachmentPathButton();
};

#endif // MAILNEW_H
