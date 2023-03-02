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

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QMenuBar *parent = nullptr);
    
private:
    void addFileMenu();
    void addJourneysMenu();
    void addTNTMenu();
    void addToolsMenu();
    void addHelpMenu();
    
    void quitApplication();
    
signals:
    void signalDonations();
    void signalImportDonationsTntWareCSV();
    void signalImportDonationsBankCSV();
    void signalExport();
    void signalImportVCard();
    void signalExportVCard();
    void signalMergeDatabase();
    void signalUserManagement();
    void signalMailList();
    void signalSettings();
    void signalCopyMailAdresses();
    void signalSQLEditor();
    void signalJourneyList();
    void signalExpensesList();
    void signalCurrencies();
    void signalAbout();
    
public slots:
    //void mailList();
    //void showSQLEditor();
    //void journeyList();
    //void expensesList();
    //void currencies();
};

#endif // MENUBAR_H
