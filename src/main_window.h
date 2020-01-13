
#include <QMainWindow>
#include <QTabWidget>
#include <QTabBar>
#include <QDir>
#include <QIcon>
#include <QMouseEvent>
#include <QToolTip>
#include <QCloseEvent>

#include "config.h"
#include "people_list.h"
#include "db_adapter.h"
#include "person_edit.h"
#include "stats.h"
#include "menubar.h"

#include "mail_list.h"
#include "mail_edit.h"
#include "mail_send.h"

#include "settings.h"
#include "sqlite_editor_widget.h"
#include "journey_list.h"
#include "currencies.h"
#include "expenses_list.h"
#include "help_about_widget.h"
#include "donations_import.h"

class QTNTMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QTNTMainWindow(QWidget *parent = nullptr);
    
protected:
    void closeEvent(QCloseEvent *event) override;
    
private:
    Config *config;
    MenuBar *menu_bar;
    QTabWidget *tab_widget;
    PeopleList *people_list;
    DbAdapter *db;
    void deactivateCloseButtons();
    QMap<QString, int> open_tabs; // {tab_name} -> tab widget id
    // make shure only one of this type of tab is open at once
    void createSingleTab(QString tab_name, QWidget *widget, QIcon *icon = nullptr);
    
private slots:
    void onTabMoved(int from, int to);
    void activateNewTab();
    void closeTab(int tab_id);
    void closeCurrentTab();
    
    void addJourneyListTab();
    void addJourneyEditTab(qlonglong rowid, QString name);
    
    void addExpensesListTab();
    
    void addPersonEditTab(qlonglong rowid, QString name);
    void addSettingsTab();
    void addSQLEditorTab();
    void addStatsTab();
    void addPeopleTab();
    void onPeopleDataChanged();
    
    void addImportDonationsTab();
    void addMailListTab();
    void addMailEditTab(qlonglong rowid);
    void addMailSendTab(qlonglong rowid, QString number);
    
    void addCurrenciesTab();
    
    void addHelpAboutTab();
};
