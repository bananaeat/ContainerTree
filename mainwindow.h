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
    QTabWidget* tabWidget;
    int currentWidget = -1;
    explicit MainWindow(QWidget *parent = nullptr);
    int widgetHeight = 600;
    int widgetWidth = 800;
    void paintEvent(QPaintEvent *event);
    void initializeMenuBar();
    void contextualizeMenuBar();
    void initializeToolBar();
    void contextualizeToolBar();
    void mousePressEvent(QMouseEvent *event);
    QToolBar* toolbar;
    QMenu* menu;
    QString addingType;

signals:

};

#endif // MAINWINDOW_H
