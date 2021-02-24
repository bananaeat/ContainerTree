#include "widget.h"
#include "ui_widget.h"
#include "mytabwidget.h"
#include "simplespeedmeter.h"
#include "mylineedit.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QTabWidget>
#include <QTextEdit>
#include <QMenu>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mouseX = 0;
    mouseY = 0;
    this->resize(500, 500);
    this->setMouseTracking(true);
    cursor = Qt::ArrowCursor;

    fakeRoot = new Container();
    chosen = fakeRoot;
    bPressed = false;
    nDrag = 0;

    SimpleSpeedMeter *ssm = new SimpleSpeedMeter(this);
    SimpleSpeedMeter *ssm2 = new SimpleSpeedMeter(this);
    MyLineEdit *mle = new MyLineEdit(this);
    Container *c = new Container(this);
    this->addContainer(150,200,250,275,ssm);
    this->addContainer(350,200,200,220,ssm2);
    this->addContainer(200,200,100,100,mle);
    this->addContainer(100,200,300,300,c);
    initializeContextMenu();
}

void Widget::addContainer(int x, int y, int w, int h, Container *c){
    c->resize(w,h);
    c->move(x,y);
    c->setMouseTracking(true);
    for(Container *oc : rootList){
        if(oc->frameGeometry().contains(c->frameGeometry().topLeft()) && oc->frameGeometry().contains(c->frameGeometry().bottomRight()) && oc->containing==true){
            oc->addContainer(c);
        }
        if(c->frameGeometry().contains(oc->frameGeometry().topLeft()) && c->frameGeometry().contains(oc->frameGeometry().bottomRight()) && c->containing==true){
            c->addContainer(oc);
        }
    }

    c->savePos = QPoint(0, 0);
    rootList.insert(0, c);

    for(int j = rootList.size()-1; j >= 0; j--){
        Container *oc = rootList.at(j);
        oc->raise();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    mouseX = event->pos().x();
    mouseY = event->pos().y();
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
            for(Container *oc : rootList){
                if(oc != chosen){
                    if(oc->frameGeometry().contains(chosen->frameGeometry().topLeft()) && oc->frameGeometry().contains(chosen->frameGeometry().bottomRight()) && oc->containing == true){
                        oc->addContainer(chosen);
                    } else {
                        oc->removeContainer(chosen);
                    }
                    if(chosen->frameGeometry().contains(oc->frameGeometry().topLeft()) && chosen->frameGeometry().contains(oc->frameGeometry().bottomRight()) && !chosen->hasContainer(oc) && chosen->containing==true){
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

    setCursor(cursor);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint aPt = event->pos();
        for(int i = 0; i < rootList.size(); i++){
            Container *c = rootList.at(i);
            QRect  aRect = c->frameGeometry();
            aRect.adjust(-5,-5,5,5);
            if(aRect.contains(aPt))
            {
                c->savePos = event->pos();
                for(Container* childc : c->containerList){
                    childc->savePos = event->pos();
                }
                bPressed = true;
                bool isChosen = true;
                for(Container *oc : rootList){
                    if(oc->frameGeometry().contains(aPt) && c->frameGeometry().contains(oc->frameGeometry().topLeft()) && c->frameGeometry().contains(oc->frameGeometry().bottomRight()) && c!=oc){
                        isChosen = false;
                    }
                }
                if(isChosen){
                    if(chosen!=NULL){
                        chosen->unChoose();
                    }
                    c->choose();
                    chosen = c;
                    c->raise();
                    qDebug() << "i : " << i;
                    rootList.removeAt(i);
                    rootList.insert(0, c);
                    i--;
                    int offsetJ = 0;
                    for(int j = rootList.size()-1; j >= 0; j--){
                        Container *oc = rootList.at(j+offsetJ);
                        if(c->frameGeometry().contains(oc->frameGeometry().topLeft()) && c->frameGeometry().contains(oc->frameGeometry().bottomRight()) && c!=oc){
                            oc->raise();
                            rootList.removeAt(j+offsetJ);
                            rootList.insert(0, oc);
                            offsetJ++;
                        }
                    }
                    return;
                }
            }
        }
        chosen->unChoose();
        chosen = fakeRoot;
    }

    if(event->button() == Qt::RightButton){
        if(this->cursor != Qt::ArrowCursor){
            this->cursor = Qt::ArrowCursor;
            this->setContextMenuPolicy(Qt::NoContextMenu);
        } else {
            this->setContextMenuPolicy(Qt::DefaultContextMenu);
        }
    }
    QWidget::mousePressEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    bPressed = false;
    nDrag = 0;
    setCursor(Qt::ArrowCursor);
}

void Widget::initializeContextMenu(){
    QAction *pAddSpeedMeterAction = new QAction("Add Simple Speed Meter",this);
    QAction *pAddLineEditAction = new QAction("Add LineEdit", this);
    QAction *pAddContainerAction = new QAction("Add Container", this);
    pDeleteAction = new QAction("Delete", this);

    connect(pAddSpeedMeterAction, &QAction::triggered ,[=](){
        SimpleSpeedMeter *s = new SimpleSpeedMeter(this);
        this->addContainer(mouseX, mouseY, 100, 110, s);
        s->show();
    });

    connect(pAddLineEditAction, &QAction::triggered ,[=](){
        MyLineEdit *s = new MyLineEdit(this);
        this->addContainer(mouseX, mouseY, 100, 100, s);
        s->show();
    });

    connect(pAddContainerAction, &QAction::triggered ,[=](){
        Container *s = new Container(this);
        this->addContainer(mouseX, mouseY, 100, 100, s);
        s->show();
    });

    pContextMenu = new QMenu(this);

    pContextMenu->addAction(pAddSpeedMeterAction);
    pContextMenu->addAction(pAddLineEditAction);
    pContextMenu->addAction(pAddContainerAction);
    pContextMenu->addSeparator();
    pContextMenu->addAction(pDeleteAction);
}

void Widget::contextMenuEvent( QContextMenuEvent * e ){
    bool deleted = false;
    for(int i = 0; i < rootList.size(); i++){
        Container *c = rootList.at(i);
        if(c->frameGeometry().contains(e->pos())){
            disconnect(pDeleteAction, &QAction::triggered, nullptr, nullptr);
            connect(pDeleteAction, &QAction::triggered ,[=](){
                rootList.removeAt(i);
                deleteWidget(c);
            });
            deleted = true;
            pDeleteAction->setEnabled(true);
            break;
        }
    }
    if(!deleted){
        pDeleteAction->setEnabled(false);
    }
    pContextMenu->exec( e->globalPos() );
}

void Widget::deleteWidget(Container *c){
    for(Container *oc : rootList){
        if(oc != c){
            oc->removeContainer(c);
        }
    }
    if(c == chosen){
        chosen = fakeRoot;
    }
    c->setParent(NULL);
    delete c;
}

QString Widget::saveWidgets(){
    QString json = "{\"widgets\":[";
    for(int i = 0; i < rootList.size(); i++){
        Container *c = rootList.at(i);
        QString x = QString::number(c->x());
        QString y = QString::number(c->y());
        QString w = QString::number(c->width());
        QString h = QString::number(c->height());


        QString children = "";
        if(c->containing){
            children += "[";
            bool hasChildren = false;
            for(int j = 0; j < rootList.size(); j++){
                Container *oc = rootList.at(j);
                if(c->containerList.contains(oc)){
                    children += QString::number(j) + ",";
                    hasChildren = true;
                }
            }
            if(hasChildren){
                children.remove(children.size()-1, 1);
            }
            children += "]";
        }

        QString type = c->containerType;
        json += "{\"x\":" + x + ",\"y\":" + y + ",\"w\":" + w + ",\"h\":" + h + ",\"type\":\"" + type +"\"";
        if(c->containing){
            json += ",\"children\":" + children;
        }
        json+="},\n";
    }
    json.remove(json.size()-2, 2);
    json+="]}";

    return json;
}

void Widget::loadWidgets(QString json){
    //Clear the widgets for now, will add other tabs in the future to save the widgets
    while(rootList.size()>0){
        Container *toDelete = rootList.at(0);
        rootList.removeAt(0);
        deleteWidget(toDelete);
    }
    qDebug() << rootList.size();

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jObject = doc.object();

    //convert the json object to variantmap
    QVariantMap mainMap = jObject.toVariantMap();

    //convert the json object to variantmap
    QVariantList widgetList = mainMap["widgets"].toList();
    for(int i = 0; i < widgetList.size(); i++){
        QVariantMap wid = widgetList[i].toMap();
        int x = wid["x"].toInt();
        int y = wid["y"].toInt();
        int w = wid["w"].toInt();
        int h = wid["h"].toInt();
        QString type = wid["type"].toString();

        //Need to be more extendable: need improvement
        Container *c;
        if(type == "c"){
            c = new Container(this);
        }
        if(type == "mle"){
            c = new MyLineEdit(this);
        }
        if(type == "ssm"){
            c = new SimpleSpeedMeter(this);
        }

        qDebug() << "Widget: x=" + wid["x"].toString() + " y=" + wid["y"].toString() + " type=" + wid["type"].toString() + " children=" + wid["children"].toString();
        this->addContainer(x,y,w,h,c);
        c->show();
    }

    for(int i = 0; i < widgetList.size(); i++){
        QVariantMap wid = widgetList[i].toMap();
        QVariantList childrenList = wid["children"].toList();
        for(int j = 0; j < childrenList.size(); j++){
            int child = childrenList[j].toInt();
            qDebug() << child;
            rootList.at(i)->containerList.append(rootList.at(child));
        }
    }
}

