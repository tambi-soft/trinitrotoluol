
#include "main_window.h"

QTNTMainWindow::QTNTMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
{
    resize(1080, 1080);
    
    this->config = new Config();
    this->db = new DbAdapter(this->config);
    
    connect(this->tab_widget, &QTabWidget::tabCloseRequested, this, &QTNTMainWindow::closeTab);
    
    this->menu_bar = new MenuBar();
    setMenuBar(this->menu_bar);
    connect(this->menu_bar, &MenuBar::signalNewMail, this, &QTNTMainWindow::addNewMailTab);
    connect(this->menu_bar, &MenuBar::signalMailList, this, &QTNTMainWindow::addMailListTab);
    connect(this->menu_bar, &MenuBar::signalSettings, this, &QTNTMainWindow::addSettingsTab);
    connect(this->menu_bar, &MenuBar::signalSQLEditor, this, &QTNTMainWindow::addSQLEditorTab);
    connect(this->menu_bar, &MenuBar::signalJourneyList, this, &QTNTMainWindow::addJourneyListTab);
    
    setCentralWidget(this->tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    QTabBar *tab_bar = this->tab_widget->tabBar();
    connect(tab_bar, &QTabBar::tabMoved, this, &QTNTMainWindow::onTabMoved);
    
    addStatsTab();
    addPeopleTab();
    
    deactivateCloseButtons();
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

void QTNTMainWindow::closeCurrentTab()
{
    int id = this->tab_widget->currentIndex();
    closeTab(id);
}

void QTNTMainWindow::deactivateCloseButtons()
{
    QTabBar *tb = this->tab_widget->tabBar();
    tb->tabButton(0, QTabBar::RightSide)->hide();
    tb->tabButton(1, QTabBar::RightSide)->hide();
}

void QTNTMainWindow::addJourneyListTab()
{
    JourneyList *journey = new JourneyList(this->db);
    
    createSingleTab("Journey List", journey);
    activateNewTab();
}

void QTNTMainWindow::addJourneyNewTab()
{
    JourneyEdit *widget = new JourneyEdit(this->db);
    
    createSingleTab("New Journey", widget);
    activateNewTab();
}

void QTNTMainWindow::addJourneyEditTab(qlonglong rowid, QString name)
{
    JourneyEdit *widget = new JourneyEdit(rowid, this->db);
    
    createSingleTab("Edit Journey "+name, widget);
    activateNewTab();
}

void QTNTMainWindow::addNewPersonTab()
{
    PersonEdit *person = new PersonEdit(this->db);
    connect(person, &PersonEdit::closeCurrentTabSignal, this, &QTNTMainWindow::closeCurrentTab);
    connect(person, &PersonEdit::dataChanged, this, &QTNTMainWindow::onPeopleDataChanged);
    
    this->tab_widget->addTab(person, "New Person");
    activateNewTab();
}

void QTNTMainWindow::addPersonEditTab(qlonglong rowid, QString name)
{
    PersonEdit *person = new PersonEdit(this->db, rowid);
    connect(person, &PersonEdit::closeCurrentTabSignal, this, &QTNTMainWindow::closeCurrentTab);
    connect(person, &PersonEdit::dataChanged, this, &QTNTMainWindow::onPeopleDataChanged);
    
    createSingleTab("Edit: "+name, person);
}

void QTNTMainWindow::addSettingsTab()
{
    SettingsWidget *settings = new SettingsWidget(this->config, this->db);
    
    QIcon *icon = new QIcon(QIcon::fromTheme("applications-system"));
    createSingleTab("Settings", settings, icon);
}

void QTNTMainWindow::addSQLEditorTab()
{
    SQLiteEditorWidget *editor = new SQLiteEditorWidget(this->db);
    
    QIcon *icon = new QIcon(":sqlite-logo");
    this->tab_widget->addTab(editor, *icon, "SQLite Editor");
    activateNewTab();
}

void QTNTMainWindow::addStatsTab()
{
    Stats *stats = new Stats(this->db);
    
    QIcon *icon = new QIcon(QIcon::fromTheme("dialog-question"));
    createSingleTab("stats", stats, icon);
}

void QTNTMainWindow::addPeopleTab()
{
    this->people_list = new PeopleList(this->db, this);
    connect(this->people_list, &PeopleList::editPersonSignal, this, &QTNTMainWindow::addPersonEditTab);
    connect(this->people_list, &PeopleList::addNewPersonSignal, this, &QTNTMainWindow::addNewPersonTab);
    
    QIcon *icon = new QIcon(QIcon::fromTheme("x-office-address-book"));
    createSingleTab("people", this->people_list, icon);
    
    activateNewTab();
}
void QTNTMainWindow::onPeopleDataChanged()
{
    this->people_list->dataChanged();
}

void QTNTMainWindow::addNewMailTab()
{
    MailNew *mail = new MailNew(this->db);
    connect(mail, &MailNew::closeCurrentTabSignal, this, &QTNTMainWindow::closeCurrentTab);
    
    QIcon *icon = new QIcon(QIcon::fromTheme("document-new"));
    createSingleTab("New Mail", mail, icon);
}

void QTNTMainWindow::addMailListTab()
{
    MailList *list = new MailList(this->db);
    connect(list, &MailList::closeCurrentTabSignal, this, &QTNTMainWindow::closeCurrentTab);
    connect(list, &MailList::signalNewMail, this, &QTNTMainWindow::addNewMailTab);
    
    //QIcon *icon = new QIcon(QIcon::fromTheme("emblem-mail"));
    createSingleTab("Mail List", list);
}

void QTNTMainWindow::createSingleTab(QString tab_name, QWidget *widget, QIcon *icon)
{
    if (this->open_tabs.contains(tab_name))
    {
        int index = this->open_tabs[tab_name];
        this->tab_widget->setCurrentIndex(index);
    }
    else
    {
        if (icon == nullptr)
        {
            this->tab_widget->addTab(widget, tab_name);
        }
        else
        {
            this->tab_widget->addTab(widget, *icon, tab_name);
        }
        activateNewTab();
        this->open_tabs[tab_name] = this->tab_widget->currentIndex();
    }
}
