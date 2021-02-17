#include "simplespeedmeter.h"
#include <QPainter>
#include <QtMath>
#include <QPainterPath>
#include <QLineEdit>

SimpleSpeedMeter::SimpleSpeedMeter(QWidget *parent) : Container(parent)
{
    theta = 135;
    QLineEdit le(this);
    size = 200;
}

SimpleSpeedMeter::~SimpleSpeedMeter(){
}

void SimpleSpeedMeter::drawGauge(QPainter &p){
    int w = this->Container::width();
    int h = this->Container::height();
    size = (w > h ? h : w);
    p.fillRect(0, 0, size, size*1.1, QColor(81, 81, 81));
    p.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor(0,0,0));
    pen.setWidth(size/40);
    p.setPen(pen);
    p.drawEllipse(QPoint(size/2, size/2), size*7/20, size*7/20);

    p.setBrush(QBrush(QColor(162,121,197)));
    pen.setWidth(0);
    pen.setColor(QColor(162,121,197));
    p.setPen(pen);
    p.drawEllipse(QPoint(size/2, size/2), size*7/100, size*7/100);

    QPainterPath path;
    path.addRoundedRect(QRectF(0, -3, size*3/10, size*3/100), size/40, size/40);
    pen.setWidth(size/40);
    p.setPen(pen);
    p.translate(size/2, size/2);
    p.rotate(theta);
    p.drawPath(path);
    p.resetTransform();

    for(int i = 0; i <= 20; i++){
        p.setBrush(QColor(0,0,0));
        pen.setColor(QColor(0,0,0));
        if(theta < 135 + i * 13.5){
            p.setBrush(QColor(0,0,0));
            pen.setColor(QColor(0,0,0));
        } else {
            p.setBrush(QColor(162,121,197));
            pen.setColor(QColor(162,121,197));
        }

        p.resetTransform();
        p.translate(size/2, size/2);
        p.setPen(pen);
        path.clear();
        path.addRoundedRect(QRect(size*2/5, -2, size*3/50, 1), size/200, size/200);
        p.rotate(135 + i * 13.5);
        p.drawPath(path);
    }
    p.resetTransform();

    p.translate(size/2,size);
    QFont font = p.font();
    font.setPixelSize(size*3/25);
    p.setFont(font);
    p.setPen(QColor(255,255,255));
    p.drawText(QRectF(-size/2, -size/20, size, size/10), Qt::AlignCenter, QString::number((theta - 135) / 2.7, 'f', 2));
}

void SimpleSpeedMeter::paintEvent(QPaintEvent *event){
    this->Container::paintEvent(event);
    QPainter painter(this);
    this->drawGauge(painter);
}

void SimpleSpeedMeter::setAngle(double a){
    theta = a;
    update();
}

void SimpleSpeedMeter::setNumber(double x){
    setAngle(135 + x * 2.7);
}

void SimpleSpeedMeter::resize(int w, int h){
    this->QWidget::resize(w,h);
    this->size = (w >= h ? h : w);
}

void SimpleSpeedMeter::setGeometry(int x, int y, int w, int h){
    this->QWidget::setGeometry(x,y,w,h);
    this->size = (w >= h ? h : w);
}
