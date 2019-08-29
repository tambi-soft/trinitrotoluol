#include "menubar.h"

MenuBar::MenuBar(QMenuBar *parent) : QMenuBar(parent)
{
    addFileMenu();
    addMailMenu();
    addTNTMenu();
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
    QAction *sendMailAction = new QAction(QIcon::fromTheme("mail-send"), "Send &Mail");
    sendMailAction->setShortcut(QKeySequence::fromString("Ctrl+M"));
    sendMailAction->setStatusTip("Send a new Mailing");
    
    
    QMenu *menu = addMenu("&Mail");
    menu->addAction(sendMailAction);
}

void MenuBar::addTNTMenu()
{
    
    
    QMenu *menu = addMenu("&TNT");
    
}

void MenuBar::addHelpMenu()
{
    
    
    QMenu *menu = addMenu("&Help");
}

void MenuBar::quitApplication()
{
    QApplication::quit();
}
