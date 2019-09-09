
#include <QMainWindow>
#include <QTabWidget>
#include <QTabBar>
#include <QDir>
#include <QIcon>
#include <QMouseEvent>
#include <QToolTip>

#include "config.h"
#include "people_list.h"
#include "db_adapter.h"
#include "person_edit.h"
#include "stats.h"
#include "menubar.h"

class QTNTMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QTNTMainWindow(QWidget *parent = nullptr);
    
private:
    Config *config;
    MenuBar *menu_bar;
    QTabWidget *tab_widget;
    PeopleList *people_list;
    DbAdapter *db;
    void deactivateCloseButtons();
    QMap<QString, int> open_tabs; // {tab_name} -> tab widget id
    
    
private slots:
    void onTabMoved(int from, int to);
    void activateNewTab();
    void closeTab(int tab_id);
    void closeCurrentTab();
    
    void addNewPersonTab();
    void addPersonEditTab(qlonglong rowid, QString name);
    void addStatsTab();
    void addPeopleTab();
};
