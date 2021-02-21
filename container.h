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
    bool containing;
    virtual void paintEvent(QPaintEvent *event);
    QList<Container*> containerList;
    void choose();
    void unChoose();
    void addContainer(Container* c);
    void removeContainer(Container* c);
    bool hasContainer(Container* c);
    QPoint  savePos;
    bool noChildAtClicked(QPoint &clicked);
    QString containerType;

signals:

};

#endif // CONTAINER_H
