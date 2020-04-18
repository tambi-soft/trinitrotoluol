#ifndef GROWINGTEXTEDIT_H
#define GROWINGTEXTEDIT_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QTimer>

/*
 * A QTextEdit which is resizing itself to its content.
 * */

class GrowingTextEdit : public QTextEdit
{
    Q_OBJECT
    
public:
    GrowingTextEdit(QTextEdit *parent = 0);
    
    void resizeEvent(QResizeEvent *event);
    
private:
    int heightMin = 0;
    int heightMax = 65000;
    
private slots:
    void sizeChanged();
};

#endif // GROWINGTEXTEDIT_H
