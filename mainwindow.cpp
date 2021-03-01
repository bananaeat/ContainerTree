#include "mainwindow.h"
#include "simplespeedmeter.h"
#include "mylineedit.h"
#include "ui_widget.h"
#include "widget.h"
#include "newdesigndialog.h"
#include <QDebug>
#include <QToolBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->resize(1000,800);
    this->setMouseTracking(true);

    tabWidget = new QTabWidget(this);
    tabWidget->resize(1000,740);
    tabWidget->move(0,60);

    initializeMenuBar();
    initializeToolBar();

    connect(tabWidget, &QTabWidget::currentChanged, [=](){
        currentWidget = tabWidget->currentIndex();
        Widget* w = (Widget*)tabWidget->widget(currentWidget);
        tabWidget->resize(w->width, w->height);
        tabWidget->move((this->width() - w->width)/2, (this->height() - w->height)/2 + 28);
        contextualizeMenuBar();
        contextualizeToolBar();
    });
}

void MainWindow::initializeToolBar(){
    toolbar = new QToolBar(this);
    this->addToolBar(toolbar);
    this->setContextMenuPolicy(Qt::NoContextMenu);

    QAction* qAddSimpleSpeedMeter = new QAction(QIcon(":/toolBar/images/speed.png"), "Add Simple Speed Meter", this);
    QAction* qAddmyLineEdit = new QAction(QIcon(":/toolBar/images/text-box.png"), "Add Line Edit", this);
    QAction* qAddContainer = new QAction(QIcon(":/toolBar/images/box.png"), "Add Container", this);

    toolbar->addAction(qAddSimpleSpeedMeter);
    toolbar->addAction(qAddmyLineEdit);
    toolbar->addAction(qAddContainer);
}

void MainWindow::contextualizeToolBar(){
    QAction* qAddSimpleSpeedMeter = toolbar->actions().at(0);
    QAction* qAddmyLineEdit = toolbar->actions().at(1);
    QAction* qAddContainer = toolbar->actions().at(2);

    Widget* w = (Widget*)tabWidget->widget(currentWidget);
    qAddSimpleSpeedMeter->disconnect();
    qAddmyLineEdit->disconnect();
    qAddContainer->disconnect();

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
    if(tabWidget->count()==0){
        return;
    }
    Widget* w = (Widget*)tabWidget->widget(currentWidget);

    QString shortenedName = w->name;
    shortenedName.truncate(12);
    shortenedName.append("...");

    if(event->button() == Qt::RightButton){
        this->setCursor(Qt::ArrowCursor);
        w->cursor = Qt::ArrowCursor;
        this->addingType = "";
    }
    if(event->button() == Qt::LeftButton){
        Container* wid = NULL;
        int height = 0;
        int width = 0;
        qDebug() << addingType;

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

            tabWidget->setTabText(currentWidget, shortenedName + (w->saved?"":"*"));
            return;
        }

        QPoint pos = event->pos() - tabWidget->pos() + QPoint(0, -20);
        w->addContainer(pos.x(), pos.y(), width, height, wid);
        qDebug() << wid->containerType;
        wid->show();
    }

    tabWidget->setTabText(currentWidget, shortenedName + (w->saved?"":"*"));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(tabWidget->count()==0){
        return;
    }
    Widget* w = (Widget*)tabWidget->widget(currentWidget);
    QString shortenedName = w->name;
    shortenedName.truncate(12);
    shortenedName.append("...");
    tabWidget->setTabText(currentWidget, shortenedName + (w->saved?"":"*"));
}

void MainWindow::initializeMenuBar(){
    QMenuBar* menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);

    menu = new QMenu("File", this);
    menuBar->addMenu(menu);


    QAction* qSaveAction = new QAction("Save Current Widget Design...", this);
    QAction* qSaveAsAction = new QAction("Save Current Widget Design As...", this);
    QAction* qOpenAction = new QAction("Open Widget Design From File...", this);
    QAction* qNewAction = new QAction("New Widget Design...", this);

    qSaveAction->setShortcut(QKeySequence::Save);
    qSaveAsAction->setShortcut(QKeySequence::SaveAs);
    qOpenAction->setShortcut(QKeySequence::Open);
    qNewAction->setShortcut(QKeySequence::New);

    menu->addAction(qNewAction);
    menu->addAction(qSaveAction);
    menu->addAction(qSaveAsAction);
    menu->addAction(qOpenAction);

    connect(qNewAction, &QAction::triggered, [=](){
        Widget* w = new Widget(this);
        NewDesignDialog* dial = new NewDesignDialog(this, "Untitled-" + QString::number(untitled));
        if(dial->exec() == QDialog::Accepted){
            if(dial->getName() == "Untitled-" + QString::number(untitled)){
                untitled++;
            }
            w->name = dial->getName();
            w->resize(dial->getWidth(), dial->getHeight());
            currentWidget++;
            QString shortenedName = w->name;
            shortenedName.truncate(12);
            shortenedName.append("...");
            tabWidget->addTab(w, shortenedName + (w->saved?"":"*"));
            tabWidget->setCurrentIndex(currentWidget);
        }
    });

    connect(qOpenAction, &QAction::triggered, [=](){
        QString fileName = QFileDialog::getOpenFileName(this,tr("Open Widget Design"), "", tr("JSON (*.json);;All Files (*)"));
        for(int i = 0; i < tabWidget->count(); i++){
            QString name = ((Widget*)tabWidget->widget(i))->name;
            if(name == fileName){
                tabWidget->setCurrentIndex(i);
                return;
            }
        }
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
            Widget* w = new Widget(this);
            w->saved = true;
            w->name = fileName;
            w->loadWidgets(loadedw);
            currentWidget++;

            QString shortenedName = w->name;
            shortenedName.truncate(12);
            shortenedName.append("...");

            tabWidget->addTab(w, shortenedName);
            tabWidget->setCurrentIndex(currentWidget);
            w->saveAsed = true;
        }
    });
}

void MainWindow::contextualizeMenuBar(){
    QAction* qSaveAction = menu->actions().at(1);
    QAction* qSaveAsAction = menu->actions().at(2);

    Widget* w = (Widget*)tabWidget->widget(currentWidget);

    qSaveAsAction->disconnect();
    qSaveAction->disconnect();

    connect(qSaveAction, &QAction::triggered, [=](){
        if(!w->saveAsed){
            qSaveAsAction->trigger();
            return;
        }

        QString fileName = w->name;
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
            w->saved = true;
            w->name = fileName;

            QString shortenedName = w->name;
            shortenedName.truncate(12);
            shortenedName.append("...");

            tabWidget->setTabText(currentWidget, shortenedName);
        }
    });

    connect(qSaveAsAction, &QAction::triggered, [=](){
        QString fileName = QFileDialog::getSaveFileName(this,tr("Save Widget Design"), "", tr("JSON (*.json);;All Files (*)"));
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
            w->saved = true;
            w->name = fileName;

            QString shortenedName = w->name;
            shortenedName.truncate(12);
            shortenedName.append("...");

            tabWidget->setTabText(currentWidget, shortenedName);
            w->saveAsed = true;
        }
    });
}
