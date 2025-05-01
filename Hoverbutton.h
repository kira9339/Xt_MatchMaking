#undef label  // 防止其他库定义了 `label` 宏
#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QObject>
#include <QWidget>
#include<QPushButton>
#include<QLabel>
#include<QDebug>
#include<QFileInfo>
class Hoverbutton:public QPushButton
{
    Q_OBJECT
public:
    explicit Hoverbutton(QWidget *parent = nullptr);
    void setImage(int _width,int _height,QString pathin);
    void setLabel(QString text);
    ~Hoverbutton() {
        qDebug() << "Hoverbutton destroyed:" << this;
        if (label) {
            qDebug() << "Deleting label:" << label;
            delete label;  // 确保析构时清理
            label = nullptr;
        }
    }
private:
    QLabel *label ;

};


#endif // HOVERBUTTON_H
