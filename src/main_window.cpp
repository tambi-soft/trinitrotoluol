
#include "main_window.h"

QTNTMainWindow::QTNTMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
{
    resize(1080, 1080);
    
    this->config = new Config();
    this->db = new DbAdapter(this->config);
    this->people_list = new PeopleList(this->db);
    
    setCentralWidget(this->tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    QTabBar *tab_bar = this->tab_widget->tabBar();
    connect(tab_bar, &QTabBar::tabMoved, this, &QTNTMainWindow::onTabMoved);
    
    tab_widget->addTab(this->people_list, "people");
}

void QTNTMainWindow::onTabMoved(int from, int to)
{
    
}
