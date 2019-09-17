#ifndef MENUBAR_H
#define MENUBAR_H

#include <QObject>
#include <QWidget>
#include <QMenuBar>
#include <QAction>
#include <QKeySequence>
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QTransform>
#include <QImage>
#include <QMessageBox>

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QMenuBar *parent = nullptr);
    
private:
    void addFileMenu();
    void addMailMenu();
    void addTNTMenu();
    void addHelpMenu();
    
    void quitApplication();
    
signals:
    void signalNewMail();
    void signalMailList();
    
public slots:
    void newMail();
    void mailList();
};

#endif // MENUBAR_H
