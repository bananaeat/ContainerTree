#include "mylineedit.h"
#include <QLineEdit>
#include <QResizeEvent>
#include <QPainter>

MyLineEdit::MyLineEdit(QWidget *parent) : Container(parent)
{
    le = new QLineEdit(this);
    le->setEnabled(false);
    this->Container::containing=false;
    this->Container::containerType = "mle";
    this->le->setText("Text...");
    this->le->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void MyLineEdit::resizeEvent(QResizeEvent *event){
    int w = event->size().width() - 4;
    int h = event->size().height() - 4;
    le->move(2,2);
    le->resize(w,h);
}
