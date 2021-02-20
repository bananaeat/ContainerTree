#include "mainwindow.h"
#include "ui_widget.h"
#include "widget.h"
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->resize(1000,800);
    Widget* w = new Widget(this);
    w->setMouseTracking(true);
    w->resize(800,600);
    w->move(100,100);
    w->show();
}

void MainWindow::drawWidgetBorder(QPainter &p){
    QBrush brush(QColor(245,245,245));
    p.setBrush(brush);
    QPen pen(QColor(0,0,0));
    pen.setWidth(0);
    p.setPen(pen);
    p.drawRect(100,100,widgetWidth,widgetHeight);
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    drawWidgetBorder(painter);
}
