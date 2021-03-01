#ifndef NEWDESIGNDIALOG_H
#define NEWDESIGNDIALOG_H

#include <QDialog>
#include <QLineEdit>

class NewDesignDialog : public QDialog
{
private:
    QLineEdit* nameEdit;
    QLineEdit* heightEdit;
    QLineEdit* widthEdit;
public:
    NewDesignDialog(QWidget *parent = nullptr, QString defaultName = "");
    QString getName();
    double getHeight();
    double getWidth();
};

#endif // NEWDESIGNDIALOG_H
