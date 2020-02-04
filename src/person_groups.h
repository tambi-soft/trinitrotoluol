#ifndef PERSONGROUPS_H
#define PERSONGROUPS_H

#include <QObject>
#include <QWidget>

#include "lib/grid_widget.h"

class PersonGroups : public GridWidget
{
    Q_OBJECT
public:
    explicit PersonGroups(GridWidget *parent = nullptr);
    
    void showData();
    
signals:
    
};

#endif // PERSONGROUPS_H
