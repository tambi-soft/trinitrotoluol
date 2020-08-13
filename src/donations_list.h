#ifndef DONATIONSLIST_H
#define DONATIONSLIST_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QDate>

#include "db_adapter.h"

class DonationsList : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsList(DbAdapter *db, QWidget *parent = nullptr);

    void showData();
    void recreateView();
    void initGrid();
    
protected:
    void hideEvent(QHideEvent */**event**/);
    void showEvent(QShowEvent */**event**/);
    
private:
    DbAdapter *db;
    QGridLayout *layout = new QGridLayout;
    
    QGridLayout *grid = new QGridLayout;// inner layout containing the qscrollarea
    QScrollArea *scroll_area = new QScrollArea(this);
    QWidget *scroll_widget = nullptr;
    int scrollbar_pos;
    
signals:
    void signalImportDonations();
};

#endif // DONATIONSLIST_H
