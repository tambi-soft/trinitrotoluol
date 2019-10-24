#ifndef MailEdit_H
#define MailEdit_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include <QFileDialog>
#include <QUrl>

/*
#include <QMessageBox>
#include <QTextBrowser>
#include <QWebEngineView>
*/

#include "db_adapter.h"
#include "mail_preview.h"

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
    QPlainTextEdit *line_alternative = new QPlainTextEdit;
    QPlainTextEdit* line_cover = new QPlainTextEdit;
    QLineEdit* line_content;
    QLineEdit* line_attachment_one;
    QLineEdit* line_attachment_two;
    QLineEdit* line_date;
    QLineEdit* line_date_last_edit;
    //QWebEngineView* preview = new QWebEngineView;
    MailPreview *preview = new MailPreview;
    
    void initializeGUI();
    void loadData();
    void addEditListeners();
    
signals:
    
public slots:
    void saveData();
    void onContentPathButton();
    void onAttachmentOnePathButton();
    void onAttachmentTwoPathButton();
    void updatePreview();
};

#endif // MailEdit_H
