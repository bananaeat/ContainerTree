#include "mainwindow.h"
#include "simplespeedmeter.h"
#include "mylineedit.h"
#include "ui_widget.h"
#include "widget.h"
#include <QPainter>
#include <QDebug>
#include <QToolBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->resize(1000,800);

    w = new Widget(this);
    w->setMouseTracking(true);
    w->resize(800,600);
    w->move(100,100);
    w->show();

    contextualizeMenuBar();
    initializeToolBar();
}

void MainWindow::initializeToolBar(){
    QToolBar* toolbar = new QToolBar(this);
    this->addToolBar(toolbar);
    this->setContextMenuPolicy(Qt::NoContextMenu);

    QAction* qAddSimpleSpeedMeter = new QAction(QIcon(":/toolBar/images/speed.png"), "Add Simple Speed Meter", this);
    QAction* qAddmyLineEdit = new QAction(QIcon(":/toolBar/images/text-box.png"), "Add Line Edit", this);
    QAction* qAddContainer = new QAction(QIcon(":/toolBar/images/box.png"), "Add Container", this);

    toolbar->addAction(qAddSimpleSpeedMeter);
    toolbar->addAction(qAddmyLineEdit);
    toolbar->addAction(qAddContainer);

    connect(qAddSimpleSpeedMeter, &QAction::triggered, [=](){
        QPixmap cursorSpeed(":/toolBar/images/speedAdd.png");
        cursorSpeed = cursorSpeed.scaledToHeight(64, Qt::SmoothTransformation);
        setCursor(QCursor(cursorSpeed));
        w->cursor = QCursor(cursorSpeed);
        addingType = "ssm";
    });

    connect(qAddmyLineEdit, &QAction::triggered, [=](){
        QPixmap cursorEdit(":/toolBar/images/text-boxAdd.png");
        cursorEdit = cursorEdit.scaledToHeight(64, Qt::SmoothTransformation);
        setCursor(QCursor(cursorEdit));
        w->cursor = QCursor(cursorEdit);
        addingType = "mle";
    });

    connect(qAddContainer, &QAction::triggered, [=](){
        QPixmap cursorContainer(":/toolBar/images/boxAdd.png");
        cursorContainer = cursorContainer.scaledToHeight(64, Qt::SmoothTransformation);
        setCursor(QCursor(cursorContainer));
        w->cursor = QCursor(cursorContainer);
        addingType = "c";
    });
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::RightButton){
        this->setCursor(Qt::ArrowCursor);
        w->cursor = Qt::ArrowCursor;
        this->addingType = "";
    }
    if(event->button() == Qt::LeftButton){
        Container* wid = NULL;
        int height = 0;
        int width = 0;

        if(addingType == "ssm"){
            wid = new SimpleSpeedMeter(w);
            height = 220; width = 200;
        }

        if(addingType == "mle"){
            wid = new MyLineEdit(w);
            height = 40; width = 200;
        }

        if(addingType == "c"){
            wid = new Container(w);
            height = 200; width = 200;
        }

        if(addingType == ""){
            return;
        }

        QPoint pos = event->pos() - w->pos();
        w->addContainer(pos.x(), pos.y(), width, height, wid);
        wid->show();
    }
}

void MainWindow::contextualizeMenuBar(){
    QMenuBar* menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);

    QMenu* menu = new QMenu("File", this);
    menuBar->addMenu(menu);

    QAction* qSaveAction = new QAction("Save", this);
    QAction* qLoadAction = new QAction("Load", this);
    menu->addAction(qSaveAction);
    menu->addAction(qLoadAction);

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
            file.close();
        }
    });

    connect(qLoadAction, &QAction::triggered, [=](){
        QString fileName = QFileDialog::getOpenFileName(this,tr("Save Widget Design"), "", tr("JSON (*.json);;All Files (*)"));
        qDebug() << fileName;
        if (fileName.isEmpty()){
            return;
        } else {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }

            QString loadedw = file.readAll();
            file.close();
            qDebug() << loadedw;
            w->loadWidgets(loadedw);
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
