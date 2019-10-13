#ifndef MENUBAR_H
#define MENUBAR_H

#include <QDebug>

#include <QObject>
#include <QWidget>
#include <QMenuBar>
#include <QAction>
#include <QKeySequence>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QTransform>
#include <QImage>
#include <QMessageBox>

#include "mail_curl.h"

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QMenuBar *parent = nullptr);
    
private:
    void addFileMenu();
    void addMailMenu();
    void addJourneysMenu();
    void addExpensesMenu();
    void addTNTMenu();
    void addToolsMenu();
    void addHelpMenu();
    
    void quitApplication();
    
signals:
    void signalNewMail();
    void signalMailList();
    void signalSettings();
    void signalSQLEditor();
    void signalJourneyList();
    void signalCurrencies();
    
public slots:
    void newMail();
    void mailList();
    void sendMail();
    void showSQLEditor();
    void journeyList();
    void currencies();
};

#endif // MENUBAR_H
