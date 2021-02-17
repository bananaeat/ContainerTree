#ifndef SIMPLESPEEDMETER_H
#define SIMPLESPEEDMETER_H

#include "container.h"
#include <QWidget>
#include <QPainter>

class SimpleSpeedMeter : public Container
{
    Q_OBJECT
public:
    explicit SimpleSpeedMeter(QWidget *parent = nullptr);
    void setNumber(double x);
    ~SimpleSpeedMeter();
    int size;
    void resize(int w, int h);
    void setGeometry(int x, int y, int w, int h);

private:
    double theta;
    void drawGauge(QPainter &p);
    void paintEvent(QPaintEvent *event);
    void setAngle(double angle);

signals:

};

#endif // SIMPLESPEEDMETER_H
