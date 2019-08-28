
#include "main_window.h"

QTNTMainWindow::QTNTMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
{
    resize(1080, 1080);
    
    this->config = new Config();
    this->db = new DbAdapter(this->config);
    this->people_list = new PeopleList(this->db, this);
    connect(this->people_list, &PeopleList::editPersonSignal, this, &QTNTMainWindow::addPersonEditTab);
    
    setCentralWidget(this->tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    QTabBar *tab_bar = this->tab_widget->tabBar();
    connect(tab_bar, &QTabBar::tabMoved, this, &QTNTMainWindow::onTabMoved);
    
    tab_widget->addTab(this->people_list, "people");
    deactivatePeopleListCloseButton();
}

void QTNTMainWindow::onTabMoved(int from, int to)
{
    
}

void QTNTMainWindow::activateNewTab()
{
    this->tab_widget->setCurrentIndex(tab_widget->count()-1);
}

void QTNTMainWindow::closeTab(int tab_id)
{
    
}

void QTNTMainWindow::deactivatePeopleListCloseButton()
{
    QTabBar *tb = this->tab_widget->tabBar();
    tb->tabButton(0, QTabBar::RightSide)->hide();
}

void QTNTMainWindow::addPersonEditTab(qlonglong rowid, QString name)
{
    PersonEdit *person = new PersonEdit(this->db, rowid);
    this->tab_widget->addTab(person, "Edit: "+name);
}
