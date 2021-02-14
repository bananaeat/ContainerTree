#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include "container.h"
#include <QTabWidget>

class myTabWidget : public Container, public QTabWidget
{
public:
    myTabWidget(QWidget *parent);
    void resize(int w, int h);
    void move(int x, int y);
};

#endif // MYTABWIDGET_H
