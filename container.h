#ifndef CONTAINER_H
#define CONTAINER_H

#include <QWidget>

class Container : public QWidget
{
    Q_OBJECT
private:
    bool chosen;
    void drawWidget(QPainter &p);
    void paintEvent(QPaintEvent *event);
public:
    explicit Container(QWidget *parent = nullptr);
    QList<Container*> containerList;
    void choose();
    void unChoose();
    void addContainer(Container* c);
    void removeContainer(Container* c);
    bool hasContainer(Container* c);
    QWidget* widget = NULL;
    QPoint  savePos;
    bool noChildAtClicked(QPoint &clicked);
    void setGeometry(int x, int y, int w, int h);
    void resize(int w, int h);
    void move(int x, int y);
    void move(QPoint &p);

signals:

};

#endif // CONTAINER_H
