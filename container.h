#ifndef CONTAINER_H
#define CONTAINER_H

#include <QWidget>

class Container : public QWidget
{
    Q_OBJECT
private:
    bool chosen;
    void drawWidget(QPainter &p);
public:
    explicit Container(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    QList<Container*> containerList;
    void choose();
    void unChoose();
    void addContainer(Container* c);
    void removeContainer(Container* c);
    bool hasContainer(Container* c);
    QPoint  savePos;
    bool noChildAtClicked(QPoint &clicked);
    void setGeometry(int x, int y, int w, int h);
    void resize(int w, int h);
    void move(int x, int y);
    void move(QPoint &p);

signals:

};

#endif // CONTAINER_H
