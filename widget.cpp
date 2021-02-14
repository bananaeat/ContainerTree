#include "widget.h"
#include "ui_widget.h"
#include "mytabwidget.h"
#include "simplespeedmeter.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QTabWidget>
#include <QTextEdit>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(500, 500);
    this->setMouseTracking(true);

    fakeRoot = new Container();
    chosen = fakeRoot;
    bPressed = false;
    nDrag = 0;

    QTabWidget *qtw = new QTabWidget(this);
    QTextEdit *qtb = new QTextEdit(this);
    SimpleSpeedMeter *ssm = new SimpleSpeedMeter(this);
    this->addContainer(100,100,200,300,qtw);
    this->addContainer(300,300,100,100,qtb);
    this->addContainer(150,200,250,250,ssm);
    qDebug() << qtw->frameGeometry().x() << " " << qtw->frameGeometry().y();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::addContainer(int x, int y, int w, int h, QWidget *qw){
    Container* c = new Container(this);
    qDebug() << "bp 1";
    if(qw!=NULL){
        qDebug() << "bp 1.5";
        c->widget = qw;
    }
    qDebug() << "bp 2";
    c->resize(w,h);
    c->move(x,y);
    c->setMouseTracking(true);
    for(Container *oc : rootList){
        if(oc->frameGeometry().contains(c->frameGeometry().topLeft()) && oc->frameGeometry().contains(c->frameGeometry().bottomRight())){
            oc->addContainer(c);
        }
        if(c->frameGeometry().contains(oc->frameGeometry().topLeft()) && c->frameGeometry().contains(oc->frameGeometry().bottomRight())){
            c->addContainer(oc);
        }
    }

    c->savePos = QPoint(0, 0);
    rootList.append(c);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
    if(bPressed)
    {
        QPoint newPos;
        if((chosen != NULL) && (chosen->frameGeometry().contains(event->pos())) && (nDrag == 0))
        {
            setCursor(Qt::ArrowCursor);
            newPos = event->pos();
            QPoint A = chosen->pos() + newPos - chosen->savePos;
            chosen->move(A);
            for(Container *childc : chosen->containerList){
                QPoint B = childc->pos() + newPos - childc->savePos;
                childc->move(B);
                childc->savePos = newPos;
            }
            chosen->savePos = newPos;
            update();

            for(Container *oc : rootList){
                if(oc != chosen){
                    if(oc->frameGeometry().contains(chosen->frameGeometry().topLeft()) && oc->frameGeometry().contains(chosen->frameGeometry().bottomRight())){
                        oc->addContainer(chosen);
                    } else {
                        oc->removeContainer(chosen);
                    }
                    if(chosen->frameGeometry().contains(oc->frameGeometry().topLeft()) && chosen->frameGeometry().contains(oc->frameGeometry().bottomRight()) && !chosen->hasContainer(oc)){
                        chosen->addContainer(oc);
                        oc->savePos = chosen->savePos;
                    }
                }
            }
            return;
        }
        bool up = false;
        if(nDrag & DRAG_TO_LEFT)
        {
            newPos = event->pos();
            int nX = chosen->pos().x() + newPos.x() - chosen->savePos.x();
            int nY = chosen->pos().y();
            int nW = chosen->geometry().width() - (newPos.x() - chosen->savePos.x());
            int nH = chosen->geometry().height();
            if(nW >10)
            {
                chosen->setGeometry(nX,nY,nW,nH);
                up = true;
            }
        }

        if(nDrag & DRAG_TO_RIGHT)
        {
            newPos = event->pos();
            int nX = chosen->pos().x();
            int nY = chosen->pos().y();
            int nW = chosen->geometry().width() + (newPos.x() - chosen->savePos.x());
            int nH = chosen->geometry().height();
            if(nW >10)
            {
                chosen->setGeometry(nX,nY,nW,nH);
                up = true;
            }
        }

        if(nDrag & DRAG_TO_TOP)
        {
            newPos = event->pos();

            int nY = chosen->pos().y() + newPos.y() - chosen->savePos.y();
            int nX = chosen->pos().x();
            int nH = chosen->geometry().height() - (newPos.y() - chosen->savePos.y());
            int nW = chosen->geometry().width();
            if(nH >10)
            {
                chosen->setGeometry(nX,nY,nW,nH);
                up = true;
            }
        }

        if(nDrag & DRAG_TO_BOTTOM)
        {
            newPos = event->pos();

            int nY = chosen->pos().y();
            int nX = chosen->pos().x();
            int nH = chosen->geometry().height() + newPos.y() - chosen->savePos.y();
            int nW = chosen->geometry().width();
            if(nH > 10)
            {
                chosen->setGeometry(nX,nY,nW,nH);
                up = true;
            }
        }

        if(up)
        {
            chosen->savePos = newPos;
            update();
        }
    }

    nDrag       = 0;
    chosen->savePos     = event->pos();
    QPoint aPt  = event->pos();
    QPoint aPt0 = chosen->frameGeometry().topLeft();
    QRect  aRt0 = QRect(aPt0+QPoint(-5,-5),QSize(10,10));

    if(aRt0.contains(aPt))
    {
        nDrag = DRAG_TO_LEFT | DRAG_TO_TOP;
        setCursor(Qt::SizeFDiagCursor);
        return;
    }

    aRt0.setRect(aPt0.x()+5,aPt0.y()-5,chosen->width()-10,10);
    if(aRt0.contains(aPt))
    {
        nDrag = DRAG_TO_TOP;
        setCursor(Qt::SizeVerCursor);
        return;
    }

    aRt0.setRect(aPt0.x()-5,aPt0.y()+5,10,chosen->height()-10);
    if(aRt0.contains(aPt))
    {
        nDrag = DRAG_TO_LEFT;
        setCursor(Qt::SizeHorCursor);
        return;
    }


    QPoint aPt1 = chosen->frameGeometry().topRight();
    QRect  aRt1 = QRect(aPt1+QPoint(5,-5),QSize(10,10));

    if(aRt1.contains(aPt))
    {
        nDrag = DRAG_TO_RIGHT | DRAG_TO_TOP;
        setCursor(Qt::SizeBDiagCursor);
        return;
    }

    aRt1.setRect(aPt1.x()-5,aPt1.y()+5,10,chosen->height()-10);
    if(aRt1.contains(aPt))
    {
        nDrag = DRAG_TO_RIGHT;
        setCursor(Qt::SizeHorCursor);
        return;
    }

    QPoint aPt2 = chosen->frameGeometry().bottomRight();
    QRect  aRt2 = QRect(aPt2+QPoint(-5,-5),QSize(10,10));

    if(aRt2.contains(aPt))
    {
        nDrag = DRAG_TO_RIGHT | DRAG_TO_BOTTOM;
        setCursor(Qt::SizeFDiagCursor);
        return;
    }

    QPoint aPt3 = chosen->frameGeometry().bottomLeft();
    QRect  aRt3 = QRect(aPt3+QPoint(-5,-5),QSize(10,10));

    if(aRt3.contains(aPt))
    {
        nDrag = DRAG_TO_LEFT | DRAG_TO_BOTTOM;
        setCursor(Qt::SizeBDiagCursor);
        return;
    }

    aRt3.setRect(aPt3.x()+5,aPt3.y()-5,chosen->width()-10,10);
    if(aRt3.contains(aPt))
    {
        nDrag = DRAG_TO_BOTTOM;
        setCursor(Qt::SizeVerCursor);
        return;
    }

    setCursor(Qt::ArrowCursor);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    QPoint aPt = event->pos();
    for(Container* c:rootList){
        QRect  aRect = c->frameGeometry();
        aRect.adjust(-5,-5,5,5);
        if(aRect.contains(aPt))
        {
            c->savePos = event->pos();
            for(Container* childc : c->containerList){
                childc->savePos = event->pos();
            }
            bPressed = true;
        }
        if(c->noChildAtClicked(aPt) && aRect.contains(aPt)){
            qDebug()<<"toggled";
            qDebug()<<(chosen!=NULL);
            if(chosen!=NULL){
                qDebug()<<"unchoose";
                chosen->unChoose();
            }
            qDebug() << "choose";
            c->choose();
            chosen = c;
            qDebug() << "return";
            return;
        }
    }
    chosen->unChoose();
    chosen = fakeRoot;
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    bPressed = false;
    nDrag = 0;
    setCursor(Qt::ArrowCursor);
}

//void Widget::paintEvent(QPaintEvent *event)
//{
//   QWidget::paintEvent(event);

//   QRect  aRect = c1->frameGeometry();

//   aRect.adjust(-5,-5,5,5);
//   QPainter painter(this);

//   painter.setPen(QPen(Qt::red,2,Qt::DotLine));
//   painter.setBrush(Qt::NoBrush);
//   painter.drawRect(aRect);
//}

