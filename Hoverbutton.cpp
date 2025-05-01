#include "Hoverbutton.h"

Hoverbutton::Hoverbutton(QWidget *parent): QPushButton(parent)
{
    setStyleSheet("QPushButton{border:0px;}");
    label = nullptr;  // 在构造函数中初始化
    qDebug() << "Hoverbutton constructed, label =" << label;  // 检查是否执行
}
void Hoverbutton::setImage(int x,int y,QString pathin)
{
    //w = x;
    //h = y;
    //setGeometry(x, y, width(), height());
    setStyleSheet(QString("QPushButton{border-image:url(%1);}").arg(pathin));
    QFileInfo file(pathin);
    if (!file.exists()) {
        qWarning() << "Image not found:" << pathin;
        return;
    }
}
void Hoverbutton::setLabel(QString text){
    if (label) {
        delete label;
        label = nullptr;
    }
    label = new QLabel(text, this);
    label->setAttribute(Qt::WA_TransparentForMouseEvents);
    label->setGeometry(0,0, width(), height());
    label->setAlignment(Qt::AlignCenter);
    label->setFont(QFont("Microsoft YaHei", 8, 600));
    label->setStyleSheet("QLabel{color:white;}");
    label->setVisible(true);
}

