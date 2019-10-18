#include "menubar.h"

MenuBar::MenuBar(QMenuBar *parent) : QMenuBar(parent)
{
    addFileMenu();
    addMailMenu();
    addJourneysMenu();
    addExpensesMenu();
    addTNTMenu();
    addToolsMenu();
    addHelpMenu();
}

void MenuBar::addFileMenu()
{
    QAction *exitAction = new QAction(QIcon::fromTheme("application-exit"), "&Exit");
    exitAction->setShortcut(QKeySequence::fromString("Ctrl+Q"));
    exitAction->setStatusTip("Exit Application");
    connect(exitAction, &QAction::triggered, this, &MenuBar::quitApplication);
    
    QMenu *menu = addMenu("&File");
    menu->addAction(exitAction);
}

void MenuBar::addMailMenu()
{
    QAction *mailListAction = new QAction("Mail List");
    connect(mailListAction, &QAction::triggered, this, &MenuBar::mailList);
    
    QAction *sendMailAction = new QAction(QIcon::fromTheme("mail-send"), "Send &Mail");
    sendMailAction->setShortcut(QKeySequence::fromString("Ctrl+M"));
    sendMailAction->setStatusTip("Send a new Mailing");
    connect(sendMailAction, &QAction::triggered, this, &MenuBar::sendMail);
    
    QMenu *menu = addMenu("&Mail");
    menu->addAction(mailListAction);
    menu->addAction(sendMailAction);
}

void MenuBar::addJourneysMenu()
{
    QAction *journeyListAction = new QAction("Journey List");
    connect(journeyListAction, &QAction::triggered, this, &MenuBar::journeyList);
    
    QMenu *menu = addMenu("&Journeys");
    menu->addAction(journeyListAction);
}

void MenuBar::addExpensesMenu()
{
    
    
    QMenu *menu = addMenu("&Expenses");
    
}

void MenuBar::addTNTMenu()
{
    
    
    QMenu *menu = addMenu("TNT");
    
}

void MenuBar::addToolsMenu()
{
    QAction *sqlEditorAction = new QAction(QIcon(":sqlite-logo"), "SQL-Editor");
    connect(sqlEditorAction, &QAction::triggered, this, &MenuBar::showSQLEditor);
    
    QAction *currenciesAction = new QAction("Currencies");
    connect(currenciesAction, &QAction::triggered, this, &MenuBar::currencies);
    
    QAction *settingsAction = new QAction(QIcon::fromTheme("applications-system"), "&Settings");
    connect(settingsAction, &QAction::triggered, this, &MenuBar::signalSettings);
    
    QMenu *menu = addMenu("&Tools");
    menu->addAction(sqlEditorAction);
    menu->addAction(currenciesAction);
    menu->addAction(settingsAction);
}

void MenuBar::addHelpMenu()
{
    
    
    QMenu *menu = addMenu("&Help");
}

void MenuBar::quitApplication()
{
    QApplication::quit();
}

void MenuBar::mailList()
{
    emit signalMailList();
}

void MenuBar::sendMail()
{
    MailCurl *mail_curl = new MailCurl();
}

void MenuBar::showSQLEditor()
{
    emit signalSQLEditor();
}

void MenuBar::journeyList()
{
    emit signalJourneyList();
}

void MenuBar::currencies()
{
    emit signalCurrencies();
}
