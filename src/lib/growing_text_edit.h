#ifndef GROWINGTEXTEDIT_H
#define GROWINGTEXTEDIT_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QMouseEvent>
#include <QEvent>

#include <QDebug>

/*
 * A QTextEdit which is resizing itself to its content.
 * */

class GrowingTextEdit : public QTextEdit
{
    Q_OBJECT
    
public:
    GrowingTextEdit(QTextEdit *parent = 0);
    
    void loadTextFromAssets(QString asset_name);
    
    void resizeEvent(QResizeEvent *event);
    
private:
    int heightMin = 0;
    int heightMax = 65000;
    
public slots:
    void sizeChanged();
    void focusInEvent(QFocusEvent */*e*/);
    void showEvent(QShowEvent */*e*/);
};

#endif // GROWINGTEXTEDIT_H
