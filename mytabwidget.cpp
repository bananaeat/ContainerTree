#include "mytabwidget.h"

myTabWidget::myTabWidget(QWidget *parent) : QTabWidget(parent), Container(parent)
{

}

void myTabWidget::resize(int w, int h){
    this->QTabWidget::resize(w,h);
    this->Container::resize(w,h);
}

void myTabWidget::move(int x, int y){
    this->QTabWidget::move(x,y);
    this->Container::move(x,y);
}
