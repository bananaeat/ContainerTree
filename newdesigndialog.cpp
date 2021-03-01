#include "newdesigndialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QDoubleValidator>
#include <QDialogButtonBox>
#include <QPushButton>

NewDesignDialog::NewDesignDialog(QWidget *parent, QString defaultName) : QDialog(parent)
{
    this->setWindowTitle("New Widget Design");
    this->resize(400,300);
    this->setContentsMargins(50,20,50,20);
    this->setModal(true);

    QGridLayout *newDesignLayout = new QGridLayout;

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    buttons->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttons->button(QDialogButtonBox::Cancel)->setDefault(true);
    buttons->button(QDialogButtonBox::Ok)->setDefault(false);

    QLabel* labelName = new QLabel("Name:", this);
    nameEdit = new QLineEdit(this);
    nameEdit->setText(defaultName);
    labelName->setBuddy(nameEdit);

    QDoubleValidator* widthValidator = new QDoubleValidator(200, 999, 2, this);
    widthValidator->setNotation(QDoubleValidator::StandardNotation);
    QDoubleValidator* heightValidator = new QDoubleValidator(200, 739, 2, this);
    heightValidator->setNotation(QDoubleValidator::StandardNotation);

    QLabel* labelWidth = new QLabel("Width:", this);
    widthEdit = new QLineEdit(this);
    widthEdit->setText("999");
    widthEdit->setValidator(widthValidator);
    connect(widthEdit, &QLineEdit::textChanged, [=](){
        if(!widthEdit->hasAcceptableInput()){
            buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
        } else {
            buttons->button(QDialogButtonBox::Ok)->setEnabled(true);
        }
    });
    labelWidth->setBuddy(widthEdit);

    QLabel* labelHeight = new QLabel("Height:", this);
    heightEdit = new QLineEdit(this);
    heightEdit->setText("739");
    heightEdit->setValidator(heightValidator);
    connect(heightEdit, &QLineEdit::textChanged, [=](){
        if(!heightEdit->hasAcceptableInput()){
            buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
        } else {
            buttons->button(QDialogButtonBox::Ok)->setEnabled(true);
        }
    });
    labelHeight->setBuddy(heightEdit);

    newDesignLayout->addWidget(labelName, 0, 0);
    newDesignLayout->addWidget(nameEdit, 0, 1, 1, 5);
    newDesignLayout->addWidget(labelWidth, 1, 0);
    newDesignLayout->addWidget(widthEdit, 1, 1);
    newDesignLayout->addWidget(labelHeight, 1, 3);
    newDesignLayout->addWidget(heightEdit, 1, 4);
    newDesignLayout->addWidget(buttons, 2, 0, 1, 6);
    this->setLayout(newDesignLayout);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString NewDesignDialog::getName(){
    return nameEdit->text();
}

double NewDesignDialog::getHeight(){
    return heightEdit->text().toDouble();
}

double NewDesignDialog::getWidth(){
    return widthEdit->text().toDouble();
}
