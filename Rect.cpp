#include "Rect.h"

Rect::Rect(int character, int len, int x, int y,int m, QWidget *parent):QPushButton(parent)
{
    this->x=x;
    this->y=y;
    this->_character=character;
    initrectpath();
    setGeometry(len*x, len*(y+m), len, len);
    setVisible(true);
    setStyleSheet(QString("QPushButton{border-image:url(%1);}").arg(Rectpath[character]));

    connect(this, &Rect::clicked, [=](bool){
        emit this->mouse_clicked(this);
    });
}
void Rect::initrectpath()
{
    Rectpath[0]=":/jingyuan.png";
    Rectpath[1]=":/kafuka.png";
    Rectpath[2]=":/ren.png";
    Rectpath[3]=":/sanyueqi.png";
    Rectpath[4]=":/lajitong.png";
}
void Rect::bomb()
{

    hide(); // 隐藏方块
}
