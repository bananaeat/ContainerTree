#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    void drawWidgetBorder(QPainter &p);
public:
    explicit MainWindow(QWidget *parent = nullptr);
    int widgetHeight = 600;
    int widgetWidth = 800;
    void paintEvent(QPaintEvent *event);

signals:

};

#endif // MAINWINDOW_H
