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
    QAction *importDonationsAction = new QAction(QIcon::fromTheme("emblem-downloads"), "&Import Donations");
    importDonationsAction->setStatusTip("Import a CSV-File from TNT Connect");
    connect(importDonationsAction, &QAction::triggered, this, &MenuBar::signalImportDonations);
    
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit Application");
    connect(exitAction, &QAction::triggered, this, &MenuBar::quitApplication);
    
    QMenu *menu = addMenu("&File");
    menu->addAction(importDonationsAction);
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
    
    QAction *sqlEditorAction = new QAction(QIcon(":sqlite-logo"), "SQL-Editor");
    //connect(sqlEditorAction, &QAction::triggered, this, &MenuBar::showSQLEditor);
    connect(sqlEditorAction, &QAction::triggered, this, &MenuBar::signalSQLEditor);
    
    QAction *currenciesAction = new QAction(QIcon(":icon_currencies"), "Currencies");
    //connect(currenciesAction, &QAction::triggered, this, &MenuBar::currencies);
    connect(currenciesAction, &QAction::triggered, this, &MenuBar::signalCurrencies);
    
    QAction *donationsAction = new QAction(QIcon(":money_receive"), "Donations");
    connect(donationsAction, &QAction::triggered, this, &MenuBar::signalDonations);
    
    QAction *settingsAction = new QAction(QIcon::fromTheme("applications-system"), "&Settings");
    connect(settingsAction, &QAction::triggered, this, &MenuBar::signalSettings);
    
    QMenu *menu = addMenu("&Tools");
    menu->addAction(mailListAction);
    menu->addAction(journeyListAction);
    menu->addAction(expensesListAction);
    menu->addAction(sqlEditorAction);
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
