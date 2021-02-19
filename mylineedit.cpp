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
    le->resize(event->size());
}
