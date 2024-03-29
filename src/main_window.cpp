
#include "main_window.h"

QTNTMainWindow::QTNTMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
{
    this->config = new Config(this);
    connect(this->config, &Config::signalAbort, this, &QTNTMainWindow::abort);
    connect(this->config, &Config::signalAmArsch, this, [this]{ qDebug() << "SIGNAL AM ARSCH"; });
    this->db = new DbAdapter(this->config);
    
    connect(this->tab_widget, &QTabWidget::tabCloseRequested, this, &QTNTMainWindow::closeTab);
    
    this->menu_bar = new MenuBar();
    setMenuBar(this->menu_bar);
    //connect(this->menu_bar, &MenuBar::signalDonations, this, &QTNTMainWindow::addDonationsListTab);
    connect(this->menu_bar, &MenuBar::signalDonations, this, &QTNTMainWindow::addDonationsTab);
    connect(this->menu_bar, &MenuBar::signalImportDonationsBankCSV, this, &QTNTMainWindow::donationsImportBankCSV);
    connect(this->menu_bar, &MenuBar::signalImportDonationsTntWareCSV, this, &QTNTMainWindow::addDonationsImportTntWareCSVTab);
    connect(this->menu_bar, &MenuBar::signalExport, this, &QTNTMainWindow::addExportTab);
    connect(this->menu_bar, &MenuBar::signalExportVCard, this, &QTNTMainWindow::addVcardExportTab);
    connect(this->menu_bar, &MenuBar::signalImportVCard, this, &QTNTMainWindow::addVcardImportTab);
    connect(this->menu_bar, &MenuBar::signalMergeDatabase, this, &QTNTMainWindow::addMergeDatabaseTab);
    connect(this->menu_bar, &MenuBar::signalUserManagement, this, &QTNTMainWindow::showUserManagementDialog);
    connect(this->menu_bar, &MenuBar::signalMailList, this, &QTNTMainWindow::addMailListTab);
    connect(this->menu_bar, &MenuBar::signalSettings, this, &QTNTMainWindow::addSettingsTab);
    connect(this->menu_bar, &MenuBar::signalCopyMailAdresses, this, &QTNTMainWindow::addCopyMailTab);
    connect(this->menu_bar, &MenuBar::signalSQLEditor, this, &QTNTMainWindow::addSQLEditorTab);
    connect(this->menu_bar, &MenuBar::signalJourneyList, this, &QTNTMainWindow::addJourneyListTab);
    connect(this->menu_bar, &MenuBar::signalExpensesList, this, &QTNTMainWindow::addExpensesListTab);
    connect(this->menu_bar, &MenuBar::signalCurrencies, this, &QTNTMainWindow::addCurrenciesTab);
    connect(this->menu_bar, &MenuBar::signalAbout, this, &QTNTMainWindow::addHelpAboutTab);
    
    setCentralWidget(this->tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    QTabBar *tab_bar = this->tab_widget->tabBar();
    connect(tab_bar, &QTabBar::tabMoved, this, &QTNTMainWindow::onTabMoved);
    
    addStatsTab();
    addPeopleTab();
    //addMailListTab();
    
    deactivateCloseButtons();
    
    calculateWindowSize();
}

void QTNTMainWindow::calculateWindowSize()
{
    /*
    int height_ = 1080;
    int width_ = 1080;
    
    qDebug() << QGuiApplication::screens();
    qDebug() << QGuiApplication::screenAt({0, 0});
    //qDebug() << "b " << this->mapToGlobal({this->width(), 0});
    qDebug() << QGuiApplication::screenAt(this->mapToGlobal({this->width()/2, 0}));
    QScreen* pScreen = QGuiApplication::screenAt(this->mapToGlobal({this->width()/2, 0}));
    QRect rec = pScreen->availableGeometry();
    if  (rec.height() < height_)
    {
        height_ = rec.height();
    }
    if (rec.width() < width_)
    {
        width_ = rec.width();
    }
    int title_bar_height = QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
    
    resize(width_,  height_ - title_bar_height);
    */
    
    resize(1080, 1080);
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
    // make shure, everything is saved to the db
    this->db->commit();
    int id = this->tab_widget->currentIndex();
    closeTab(id);
}

void QTNTMainWindow::deactivateCloseButtons()
{
    QTabBar *tb = this->tab_widget->tabBar();
    tb->tabButton(0, QTabBar::RightSide)->hide();
    tb->tabButton(1, QTabBar::RightSide)->hide();
}

void QTNTMainWindow::addVcardExportTab()
{
    VCardExport *vcard = new VCardExport(this->db);
    
    createSingleTab("Export vCard", vcard);
    activateNewTab();
}

void QTNTMainWindow::addVcardImportTab()
{
    VCardImport *vcard = new VCardImport(this->db);
    
    createSingleTab("Import vCard", vcard);
    activateNewTab();
}

void QTNTMainWindow::addJourneyListTab()
{
    JourneyList *journey = new JourneyList(this->db);
    connect(journey, &JourneyList::signalJourneyEdit, this, &QTNTMainWindow::addJourneyEditTab);
    
    QIcon *icon = new QIcon(":icon_journey");
    
    createSingleTab("Journey List", journey, icon);
    activateNewTab();
}

void QTNTMainWindow::addJourneyEditTab(qlonglong rowid, QString name)
{
    JourneyEdit *widget = new JourneyEdit(rowid, this->db);
    
    createSingleTab("Edit Journey: "+name, widget);
    activateNewTab();
}

void QTNTMainWindow::addExpensesListTab()
{
    ExpensesList *expenses = new ExpensesList(this->db);
    
    QIcon *icon = new QIcon(":icon_expenses");
    createSingleTab("Expenses", expenses, icon);
    activateNewTab();
}

void QTNTMainWindow::addPersonEditTab(qlonglong rowid, QString name)
{
    PersonDetails *person = new PersonDetails(this->db, rowid);
    connect(person, &PersonDetails::dataChanged, this, &QTNTMainWindow::onPeopleDataChanged);
    
    createSingleTab("Edit: "+name, person);
}

void QTNTMainWindow::addSettingsTab()
{
    SettingsWidget *settings = new SettingsWidget(this->config, this->db);
    
    QIcon *icon = new QIcon(QIcon::fromTheme("applications-system"));
    createSingleTab("Settings", settings, icon);
}

void QTNTMainWindow::addCopyMailTab()
{
    ToolCopyEmailAddresses *w = new ToolCopyEmailAddresses(this->db);
    
    createSingleTab("Copy Email Addresses", w);
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
    //connect(this->people_list, &PeopleList::addNewPersonSignal, this, &QTNTMainWindow::addNewPersonTab);
    
    QIcon *icon = new QIcon(QIcon::fromTheme("x-office-address-book"));
    createSingleTab("people", this->people_list, icon);
    
    activateNewTab();
}
void QTNTMainWindow::onPeopleDataChanged()
{
    this->people_list->dataChanged();
}

void QTNTMainWindow::addDonationsTab()
{
    Donations *don = new Donations(this->db);
    QIcon *icon = new QIcon(":icon_money_receive");
    createSingleTab("Donations", don, icon);
}

/*
void QTNTMainWindow::addDonationsListTab()
{
    DonationsList *don = new DonationsList(this->db);
    connect(don, &DonationsList::signalImportDonations, this, &QTNTMainWindow::addDonationsImportTab);
    
    QIcon *icon = new QIcon(":icon_money_receive");
    createSingleTab("Donations", don, icon);
}
*/

void QTNTMainWindow::addDonationsImportTntWareCSVTab()
{
    DonationsImportTNT *don = new DonationsImportTNT(this->db);
    QIcon *icon = new QIcon(QIcon::fromTheme("emblem-downloads"));
    createSingleTab("Import Donations from TntWare", don, icon);
}

void QTNTMainWindow::donationsImportBankCSV()
{
    DonationsImportBank *don = new DonationsImportBank(this->config, this->db);
    QIcon *icon = new QIcon(QIcon::fromTheme("emblem-downloads"));
    createSingleTab("Import Donations from Bank", don, icon);
}

void QTNTMainWindow::addExportTab()
{
    Export *ex = new Export(this->db);
    QIcon *icon = new QIcon(QIcon::fromTheme("document-save-as"));
    createSingleTab("Export", ex, icon);
}

void QTNTMainWindow::addMergeDatabaseTab()
{
    MergeDatabaseWidget *merge = new MergeDatabaseWidget(this->db);
    QIcon *icon = new QIcon(QIcon::fromTheme("emblem-downloads"));
    createSingleTab("Merge from other Database", merge, icon);
}

void QTNTMainWindow::showUserManagementDialog()
{
    this->config->showUserManagementDialog();
}

void QTNTMainWindow::addMailListTab()
{
    MailList *list = new MailList(this->db);
    connect(list, &MailList::signalEditMail, this, &QTNTMainWindow::addMailEditTab);
    connect(list, &MailList::signalSendMail, this, &QTNTMainWindow::addMailSendTab);
    
    QIcon *icon = new QIcon(QIcon::fromTheme("emblem-mail"));
    createSingleTab("Mail List", list, icon);
}

void QTNTMainWindow::addMailEditTab(qlonglong rowid, QString name)
{
    MailEdit *mail = new MailEdit(this->db, rowid);
    QIcon *icon = new QIcon(QIcon::fromTheme("document-new"));
    createSingleTab(name, mail, icon);
}

void QTNTMainWindow::addMailSendTab(qlonglong rowid, QString number)
{
    MailSend *send = new MailSend(this->db, rowid);
    QIcon *icon = new QIcon(QIcon::fromTheme("document-send"));
    createSingleTab("Send Email No. "+number, send, icon);
}

void QTNTMainWindow::addCurrenciesTab()
{
    CurrenciesList *currencies = new CurrenciesList(this->db);
    
    QIcon *icon = new QIcon(":icon_currencies");
    
    createSingleTab("Currencies", currencies, icon);
}

void QTNTMainWindow::addHelpAboutTab()
{
    HelpAboutWidget *widget = new HelpAboutWidget();
    
    QIcon *icon = new QIcon(":logo");
    
    createSingleTab("About", widget, icon);
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

void QTNTMainWindow::closeEvent( QCloseEvent* event )
{
    // just make shure all database changes are really are applied before closing
    this->db->commit();
    
    event->accept();
}

void QTNTMainWindow::abort()
{
    qDebug() << "AM ARSCH";
    QCoreApplication::quit();
}
