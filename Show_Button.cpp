#include "Show_Button.h"
#include<QLabel>
#include<QTimer>
#include<QString>
#include<QObject>
Show_Button::Show_Button(int x, int y, int width, int height, QString path,  QWidget *parent ): QLabel(parent)

{
    startbutton = new Hoverbutton(this);
    //分别为左上角坐标，宽度和高度
    startbutton->setGeometry(0, 0,width,height);
    startbutton->setImage(width,height,path);
    connect(startbutton, &QPushButton::clicked, [=](){emit clicked(); });
    //菱形继承会导致编译器不知道调用哪个connect
    this->setGeometry(x, y, width, height);
    this->setStyleSheet("background: transparent;");      // 透明背景
}
void Show_Button::show(){
    startbutton->setVisible(true);

}

void Show_Button::setVisible(bool a){
    startbutton->setVisible(a);
    QLabel::setVisible(a);
}
Show_Button::~Show_Button()
{

}
