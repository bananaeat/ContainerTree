#include "container.h"
#include <QPainter>
#include <QDebug>
#include <QException>

Container::Container(QWidget *parent) : QWidget(parent)
{
    chosen = false;
    widget = NULL;
}

void Container::drawWidget(QPainter &p){
    QPen pen(QColor(0,0,0));
    pen.setWidth(5);
    p.setPen(pen);
    p.drawRect(0, 0, this->width(), this->height());
    if(chosen){
        pen.setColor(QColor(244, 102, 45));
        pen.setWidth(10);
        p.setPen(pen);
        p.drawRect(0, 0, this->width(), this->height());
    }
}

void Container::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    this->drawWidget(painter);
}

void Container::choose(){
    chosen = true;
    this->update();
}

void Container::unChoose(){
    chosen = false;
    this->update();
}

bool Container::noChildAtClicked(QPoint &clicked){
    for(Container* c : containerList){
        if(!(c->noChildAtClicked(clicked))){
            return false;
        }
        if(c->frameGeometry().contains(clicked)){
            return false;
        }
    }
    return true;
}

void Container::addContainer(Container* c){
    if(!this->hasContainer(c)){
        this->containerList.append(c);
    }
}

void Container::removeContainer(Container* c){
    for(int i = 0; i < containerList.size(); i++){
        Container *oc = containerList.value(i);
        oc->removeContainer(c);
        if(oc == c){
            containerList.removeAt(i);
            i--;
        }
    }
}

bool Container::hasContainer(Container* c){
    bool has = false;
    for(int i = 0; i < containerList.size(); i++){
        Container *oc = containerList.value(i);
        if(oc->hasContainer(c)){has=true;}
        if(oc == c){has=true;}
    }
    return has;
}

void Container::resize(int w, int h){
    this->QWidget::resize(w,h);
    if(this->widget != NULL){
        this->widget->resize(w,h);
    }
}

void Container::setGeometry(int x, int y, int w, int h){
    this->QWidget::setGeometry(x,y,w,h);
    if(this->widget != NULL){
        this->widget->setGeometry(x,y,w,h);
    }
}

void Container::move(int x, int y){
    this->QWidget::move(x,y);
    if(this->widget != NULL){
        this->widget->move(x,y);
    }
}

void Container::move(QPoint &p){
    this->QWidget::move(p);
    if(this->widget != NULL){
        this->widget->QWidget::move(p.x(), p.y());
    }
}
