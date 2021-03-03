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
    int widgetHeight = 600;
    int widgetWidth = 800;
    void initializeMenuBar();
    void contextualizeMenuBar();
    void initializeToolBar();
    void contextualizeToolBar();
    void initializeTabWidget();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QToolBar* toolbar;
    QMenu* menu;
    QString addingType;
    int untitled = 0;
    QTabWidget* tabWidget;
    int currentWidget = -1;
public:
    explicit MainWindow(QWidget *parent = nullptr);


signals:

};

#endif // MAINWINDOW_H
