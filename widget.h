#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "container.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#define DRAG_TO_LEFT   1
#define DRAG_TO_RIGHT  2
#define DRAG_TO_TOP    4
#define DRAG_TO_BOTTOM 8

class Widget : public QWidget
{
    Q_OBJECT
private:
    Container* fakeRoot;
    Container* chosen;
    QList<Container*> rootList;
    bool bPressed;
    QWidget *pEdit;
    int     nDrag;
    Container* c1;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void paintEvent(QPaintEvent *event);
    void mouseClickEvent(QMouseEvent *event);
    void addContainer(int x, int y, int w, int h, QWidget *qw);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
