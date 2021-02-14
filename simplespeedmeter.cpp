#include "simplespeedmeter.h"
#include <QPainter>
#include <QtMath>
#include <QPainterPath>
#include <QLineEdit>

SimpleSpeedMeter::SimpleSpeedMeter(QWidget *parent) : QWidget(parent)
{
    theta = 135;
    QLineEdit le(this);
}

SimpleSpeedMeter::~SimpleSpeedMeter(){
}

void SimpleSpeedMeter::drawGauge(QPainter &p){
    p.fillRect(0, 0, 200, 220, QColor(81, 81, 81));
    p.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor(0,0,0));
    pen.setWidth(5);
    p.setPen(pen);
    p.drawEllipse(QPoint(100, 100), 70, 70);

    p.setBrush(QBrush(QColor(162,121,197)));
    pen.setWidth(0);
    pen.setColor(QColor(162,121,197));
    p.setPen(pen);
    p.drawEllipse(QPoint(100, 100), 14, 14);

    QPainterPath path;
    path.addRoundedRect(QRectF(0, -3, 60, 6), 5, 5);
    pen.setWidth(5);
    p.setPen(pen);
    p.translate(100, 100);
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
        p.translate(100, 100);
        p.setPen(pen);
        path.clear();
        path.addRoundedRect(QRect(80, -2, 12, 1), 1, 1);
        p.rotate(135 + i * 13.5);
        p.drawPath(path);
    }
    p.resetTransform();

    p.translate(100,200);
    QFont font = p.font();
    font.setPixelSize(24);
    p.setFont(font);
    p.setPen(QColor(255,255,255));
    p.drawText(QRectF(-100, -10, 200, 20), Qt::AlignCenter, QString::number((theta - 135) / 2.7, 'f', 2));
}

void SimpleSpeedMeter::paintEvent(QPaintEvent *event){
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
