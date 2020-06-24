#include "menubar.h"

MenuBar::MenuBar(QMenuBar *parent) : QMenuBar(parent)
{
    addFileMenu();
    //addJourneysMenu();
    //addTNTMenu();
    addToolsMenu();
    addHelpMenu();
}

void MenuBar::addFileMenu()
{
    QAction *importDonationsTntWareCSVAction = new QAction(QIcon::fromTheme("emblem-downloads"), "&Import Donations from TntWare");
    importDonationsTntWareCSVAction->setStatusTip("Import Donations from TntConnect");
    connect(importDonationsTntWareCSVAction, &QAction::triggered, this, &MenuBar::signalImportDonationsTntWareCSV);
    
    QAction *exportDonationsAction = new QAction(QIcon::fromTheme("document-save-as"), "&Export");
    connect(exportDonationsAction, &QAction::triggered, this, &MenuBar::signalExport);
    
    QAction *importTNTConnect = new QAction(QIcon::fromTheme("document-save-as"), "Import Users from TntConnect");
    
    QAction *importPeopleFromVCard = new QAction("Import People from vCard (.vcf)");
    
    QAction *exportPeopleToVCard = new QAction("Export People to vCard (.vcf)");
    
    //QAction *mergeDatabaseAction = new QAction(QIcon::fromTheme("emblem-downloads"), "Merge from other Database");
    //connect(mergeDatabaseAction, &QAction::triggered, this, &MenuBar::signalMergeDatabase);
    
    QAction *userManage = new QAction(QIcon::fromTheme("document-save-as"), "Manage Users");
    connect(userManage, &QAction::triggered, this, &MenuBar::signalUserManagement);
    
    
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit Application");
    connect(exitAction, &QAction::triggered, this, &MenuBar::quitApplication);
    
    QMenu *menu = addMenu("&File");
    menu->addAction(importDonationsTntWareCSVAction);
    menu->addAction(importTNTConnect);
    menu->addSeparator();
    menu->addAction(importPeopleFromVCard);
    menu->addAction(exportPeopleToVCard);
    menu->addSeparator();
    //menu->addAction(exportDonationsAction);
    menu->addSeparator();
    menu->addAction(userManage);
    menu->addSeparator();
    //menu->addAction(mergeDatabaseAction);
    menu->addAction(exitAction);
}

void MenuBar::addJourneysMenu()
{
    QAction *journeyListAction = new QAction("Journey List");
    //connect(journeyListAction, &QAction::triggered, this, &MenuBar::journeyList);
    connect(journeyListAction, &QAction::triggered, this, &MenuBar::signalJourneyList);
    
    QMenu *menu = addMenu("&Journeys");
    menu->addAction(journeyListAction);
}

void MenuBar::addTNTMenu()
{
    
    
    QMenu *menu = addMenu("TNT");
    
}

void MenuBar::addToolsMenu()
{
    QAction *mailListAction = new QAction(QIcon::fromTheme("mail-send"), "Mailings");
    //connect(mailListAction, &QAction::triggered, this, &MenuBar::mailList);
    connect(mailListAction, &QAction::triggered, this, &MenuBar::signalMailList);
    
    QAction *journeyListAction = new QAction(QIcon(":icon_journey"), "Journeys");
    //connect(journeyListAction, &QAction::triggered, this, &MenuBar::journeyList);
    connect(journeyListAction, &QAction::triggered, this, &MenuBar::signalJourneyList);
    
    QAction *expensesListAction = new QAction(QIcon(":icon_expenses"), "Expenses");
    //connect(expensesListAction, &QAction::triggered, this, &MenuBar::expensesList);
    connect(expensesListAction, &QAction::triggered, this, &MenuBar::signalExpensesList);
    
    QAction *sqlEditorAction = new QAction(QIcon(":icon_sqlite-logo"), "SQL-Editor");
    //connect(sqlEditorAction, &QAction::triggered, this, &MenuBar::showSQLEditor);
    connect(sqlEditorAction, &QAction::triggered, this, &MenuBar::signalSQLEditor);
    
    QAction *currenciesAction = new QAction(QIcon(":icon_currencies"), "Currencies");
    //connect(currenciesAction, &QAction::triggered, this, &MenuBar::currencies);
    connect(currenciesAction, &QAction::triggered, this, &MenuBar::signalCurrencies);
    
    QAction *donationsAction = new QAction(QIcon(":icon_money_receive"), "Donations");
    connect(donationsAction, &QAction::triggered, this, &MenuBar::signalDonations);
    
    QAction *settingsAction = new QAction(QIcon::fromTheme("applications-system"), "&Settings");
    connect(settingsAction, &QAction::triggered, this, &MenuBar::signalSettings);
    
    QMenu *menu = addMenu("&Tools");
    menu->addAction(mailListAction);
    menu->addAction(journeyListAction);
    menu->addAction(expensesListAction);
    //menu->addAction(sqlEditorAction);
    menu->addAction(currenciesAction);
    menu->addAction(donationsAction);
    menu->addAction(settingsAction);
}

void MenuBar::addHelpMenu()
{
    QAction *aboutAction = new QAction(QIcon(":logo"), "About Trinitrotoluol");
    connect(aboutAction, &QAction::triggered, this, &MenuBar::signalAbout);
    
    QMenu *menu = addMenu("&Help");
    menu->addAction(aboutAction);
}

void MenuBar::quitApplication()
{
    QApplication::quit();
}
/*
void MenuBar::mailList()
{
    emit signalMailList();
}
*/
/*
void MenuBar::showSQLEditor()
{
    emit signalSQLEditor();
}
*/
/*
void MenuBar::journeyList()
{
    emit signalJourneyList();
}
*/
/*
void MenuBar::expensesList()
{
    emit signalExpensesList();
}
*/
/*
void MenuBar::currencies()
{
    emit signalCurrencies();
}
*/
