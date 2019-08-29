
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
    
    connect(this->tab_widget, &QTabWidget::tabCloseRequested, this, &QTNTMainWindow::closeTab);
    
    this->menu_bar = new MenuBar();
    setMenuBar(this->menu_bar);
    
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
    // adjust the map to match the new tab ids
    for (auto k : this->open_tabs.keys())
    {
        int value = this->open_tabs[k];
        
        if (value == from)
        {
            this->open_tabs[k] = to;
        }
        if (value == to)
        {
            this->open_tabs[k] = from;
        }
        
    }
}

void QTNTMainWindow::activateNewTab()
{
    this->tab_widget->setCurrentIndex(this->tab_widget->count()-1);
}

void QTNTMainWindow::closeTab(int tab_id)
{
    QWidget *tab_to_delete = tab_widget->widget(tab_id);
    tab_widget->removeTab(tab_id);
    tab_to_delete->deleteLater();
    
    // We want to have a "deck item widget" just open once for every deck to avoid synchronization issues. We keep track of them by storing every active instance in the map open_tabs. So on closing an according widget, we need to update the map accordingly.
    for (auto k : this->open_tabs.keys())
    {
        int value = this->open_tabs[k];
        // remove key->value pair from the map
        if (value == tab_id)
        {
            this->open_tabs.remove(k);
        }
        // adjust the map to match the new tab ids
        else if (value > tab_id)
        {
            this->open_tabs[k] = value -1;
        }
    }
}

void QTNTMainWindow::deactivatePeopleListCloseButton()
{
    QTabBar *tb = this->tab_widget->tabBar();
    tb->tabButton(0, QTabBar::RightSide)->hide();
}

void QTNTMainWindow::addPersonEditTab(qlonglong rowid, QString name)
{
    QString tab_name = "Edit: "+name;
    if (this->open_tabs.contains(tab_name))
    {
        int index = this->open_tabs[tab_name];
        this->tab_widget->setCurrentIndex(index);
    }
    else
    {
        PersonEdit *person = new PersonEdit(this->db, rowid);
        this->tab_widget->addTab(person, tab_name);
        activateNewTab();
        this->open_tabs[tab_name] = this->tab_widget->currentIndex();
    }
}
