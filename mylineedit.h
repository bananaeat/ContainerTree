#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include "container.h"

class MyLineEdit : public Container
{
public:
    QLineEdit *le;
    MyLineEdit(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event);
};

#endif // MYLINEEDIT_H
