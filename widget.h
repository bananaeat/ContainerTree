#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenu>
#include "container.h"

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
    void initializeContextMenu();
    QMenu* pContextMenu;
    QAction *pDeleteAction;
public:
    int mouseX; int mouseY;
    Widget(QWidget *parent = nullptr);
    void resize(int w, int h);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseClickEvent(QMouseEvent *event);
    void addContainer(int x, int y, int w, int h, Container *c);
    void contextMenuEvent( QContextMenuEvent * e );
    void deleteWidget(Container *c);
    QString saveWidgets();
    void loadWidgets(QString json);
    QCursor cursor;
    QString name;
    bool saved;
    bool saveAsed = false;
    int height; int width;
};
#endif // WIDGET_H
