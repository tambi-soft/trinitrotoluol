#ifndef DONATIONSLIST_H
#define DONATIONSLIST_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>

#include "db_adapter.h"

class DonationsList : public QWidget
{
    Q_OBJECT
public:
    explicit DonationsList(DbAdapter *db, QWidget *parent = nullptr);

    void showData();
    void refresh();
    
    
private:
    DbAdapter *db;
    QGridLayout *layout = new QGridLayout;
    
    QGridLayout *grid; // inner layout containing the qscrollarea
    QScrollArea *scroll_area = new QScrollArea(this);
    QWidget *scroll_widget = nullptr;
    int scrollbar_pos;
    
signals:
    void signalImportDonations();
};

#endif // DONATIONSLIST_H
