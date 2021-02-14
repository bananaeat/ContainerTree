#ifndef SIMPLESPEEDMETER_H
#define SIMPLESPEEDMETER_H

#include <QWidget>
#include <QPainter>

class SimpleSpeedMeter : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleSpeedMeter(QWidget *parent = nullptr);
    void setNumber(double x);
    ~SimpleSpeedMeter();

private:
    double theta;
    void drawGauge(QPainter &p);
    void paintEvent(QPaintEvent *event);
    void setAngle(double angle);

signals:

};

#endif // SIMPLESPEEDMETER_H
