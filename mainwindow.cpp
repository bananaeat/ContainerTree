#include "mainwindow.h"
#include "ui_widget.h"
#include "widget.h"
#include <QPainter>
#include <QDebug>
#include <QToolBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->resize(1000,800);

    Widget* w = new Widget(this);
    w->setMouseTracking(true);
    w->resize(800,600);
    w->move(100,100);
    w->show();

    QMenuBar* menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);

    QMenu* menu = new QMenu("File", this);
    menuBar->addMenu(menu);

    QAction* qSaveAction = new QAction("Save", this);
    menu->addAction(qSaveAction);

    connect(qSaveAction, &QAction::triggered, [=](){
        QString fileName = QFileDialog::getSaveFileName(this,tr("Save Widget Design"), "", tr("JSON (*.json);;All Files (*)"));
        qDebug() << fileName;
        if (fileName.isEmpty()){
            return;
        } else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }

            file.write(w->saveWidgets().toUtf8());
        }
    });

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
