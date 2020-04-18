#include "growing_text_edit.h"

GrowingTextEdit::GrowingTextEdit(QTextEdit *parent)
{
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(sizeChanged()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(sizeChanged()));
    connect(this, SIGNAL(textChanged()), this, SLOT(sizeChanged()));
    
    QTimer::singleShot(1, this, SLOT(sizeChanged()));
}

void GrowingTextEdit::resizeEvent(QResizeEvent *event)
{
    sizeChanged();
    QTextEdit::resizeEvent(event);
}

void GrowingTextEdit::sizeChanged()
{
    qreal docHeight = document()->size().height();
    
    if ((heightMin <= docHeight) <= heightMax)
    {
        setFixedHeight(docHeight + 2);
    }
}
