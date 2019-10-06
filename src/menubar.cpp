#include "menubar.h"

MenuBar::MenuBar(QMenuBar *parent) : QMenuBar(parent)
{
    addFileMenu();
    addMailMenu();
    addJourneysMenu();
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
    QAction *newMailAction = new QAction(QIcon::fromTheme("document-new"), "Prepare New Mail");
    connect(newMailAction, &QAction::triggered, this, &MenuBar::newMail);
    
    QAction *mailListAction = new QAction("Mail List");
    connect(mailListAction, &QAction::triggered, this, &MenuBar::mailList);
    
    QAction *sendMailAction = new QAction(QIcon::fromTheme("mail-send"), "Send &Mail");
    sendMailAction->setShortcut(QKeySequence::fromString("Ctrl+M"));
    sendMailAction->setStatusTip("Send a new Mailing");
    connect(sendMailAction, &QAction::triggered, this, &MenuBar::sendMail);
    
    QMenu *menu = addMenu("&Mail");
    menu->addAction(newMailAction);
    menu->addAction(mailListAction);
    menu->addAction(sendMailAction);
}

void MenuBar::addJourneysMenu()
{
    QAction *newJourneyAction = new QAction("New Journey");
    
    
    QMenu *menu = addMenu("&Journey");
    menu->addAction(newJourneyAction);
}

void MenuBar::addTNTMenu()
{
    
    
    QMenu *menu = addMenu("TNT");
    
}

void MenuBar::addToolsMenu()
{
    QAction *settingsAction = new QAction(QIcon::fromTheme("applications-system"), "&Settings");
        //settingsAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
        connect(settingsAction, &QAction::triggered, this, &MenuBar::signalSettings);
    
    QMenu *menu = addMenu("&Tools");
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

void MenuBar::newMail()
{
    emit signalNewMail();
}

void MenuBar::mailList()
{
    emit signalMailList();
}

void MenuBar::sendMail()
{
    MailCurl *mail_curl = new MailCurl();
}
