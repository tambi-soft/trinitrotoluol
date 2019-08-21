
#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QIcon>
#include <QMouseEvent>
#include <QToolTip>

#include "config.h"

class QTNTMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QTNTMainWindow(QWidget *parent = nullptr);
    
private:
    Config *config;
    
private slots:
    
};
