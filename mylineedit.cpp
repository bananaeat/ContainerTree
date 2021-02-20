#include "mylineedit.h"
#include <QLineEdit>
#include <QResizeEvent>
#include <QPainter>

MyLineEdit::MyLineEdit(QWidget *parent) : Container(parent)
{
    le = new QLineEdit(this);
    le->setEnabled(false);
    this->Container::containing=false;
}

void MyLineEdit::resizeEvent(QResizeEvent *event){
    int w = event->size().width() - 4;
    int h = event->size().height() - 4;
    le->move(2,2);
    le->resize(w,h);
}
