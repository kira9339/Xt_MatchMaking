#include "Settlement.h"
#include "ui_Settlement.h"
#include <QDebug>
Settlement::Settlement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settlement)
{
    ui->setupUi(this);
    this->hide();
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    ui->mask->setStyleSheet("QPushButton{border-image:url(:/gamepic/mask.png);}");
    ui->button->setImage(ui->button->width(), ui->button->height(),":/gamepic/button_1.png");
}
QPushButton* Settlement::mask(){
    return ui->mask;
}

Hoverbutton* Settlement::button(){
    return ui->button;
}
Settlement::~Settlement()
{
    delete ui;
}

