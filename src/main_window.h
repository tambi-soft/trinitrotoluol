
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

class QTNTMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QTNTMainWindow(QWidget *parent = nullptr);
    
private:
    Config *config;
    QTabWidget *tab_widget;
    PeopleList *people_list;
    DbAdapter *db;
    
private slots:
    void onTabMoved(int from, int to);
};
