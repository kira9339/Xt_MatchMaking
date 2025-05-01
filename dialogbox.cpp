#include "dialogbox.h"
#include "ui_dialogbox.h"

dialogbox::dialogbox(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::dialogbox)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setFixedSize(1024,768);
    ui->mask->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->mask->setFixedSize(1024, 768);
    ui->mask->setStyleSheet("QPushButton{border-image:url(:/gamepic/mask.png);}");
    ui->titleLabel->setAlignment(Qt::AlignCenter);
    ui->titleLabel->setStyleSheet("color:white;");
    ui->titleLabel->setFont(QFont("Microsoft YaHei", 8, 600));
    ui->descriptionlabel->setFont(QFont("Microsoft YaHei", 8, 600));
    ui->pushbutton->setImage(509, 250,":/gamepic/button_1.png");
    ui->pushbutton->setLabel("确定");
}


QPushButton* dialogbox::mask(){
    return ui->mask;
}
QLabel* dialogbox::title(){
    return ui->titleLabel;
}
Hoverbutton* dialogbox::button(){
    return ui->pushbutton;
}
QLabel* dialogbox::descriptionlabel(){
    return ui->descriptionlabel;
}
QLineEdit* dialogbox::lineEdit(){
    return ui->lineEdit;
}
dialogbox::~dialogbox()
{
    delete ui;
}
