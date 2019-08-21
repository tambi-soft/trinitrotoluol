
#include "main_window.h"

QTNTMainWindow::QTNTMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1080, 1080);
    
    this->config = new Config();
}
