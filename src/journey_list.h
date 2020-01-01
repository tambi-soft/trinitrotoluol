#ifndef JOURNEYLIST_H
#define JOURNEYLIST_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QMessageBox>
#include <QPushButton>

#include <QStringList>
#include <QList>
#include <QMap>
#include <QDate>

#include <QShowEvent>

#include "db_adapter.h"
#include "journey_edit.h"

class JourneyList : public QWidget
{
    Q_OBJECT
public:
    explicit JourneyList(DbAdapter *db, QWidget *parent = nullptr);
    
private:
    DbAdapter *db;
    QVBoxLayout *layout;
    
    QGridLayout *grid; // inner layout containing the qscrollarea
    QScrollArea *scroll_area = new QScrollArea(this);
    QWidget *scroll_widget = nullptr;
    int scrollbar_pos;
    
    void showJourneys();
    void refresh();
    
signals:
    void signalJourneyNew();
    void signalJourneyEdit(qlonglong rowid, QString name);
    
public slots:
    void journeyNew();
    void journeyEdit(qlonglong rowid, QString name);
    void journeyDelete(qlonglong rowid, QString name);
    void showEvent(QShowEvent */*event*/);
};

#endif // JOURNEYLIST_H
