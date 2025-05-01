#ifndef RECT_H
#define RECT_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QPixmap>
#include <QMovie>
#include <QLabel>
#include<QPainter>

class Rect: public QPushButton
{

    Q_OBJECT
public:
    Rect(int _character, int len, int x, int y,int m=0, QWidget *parent = nullptr);
    int X(){return this->x;}
    int Y(){return this->y;}
    void setY(int n){this->y=n;}
    void setX(int n){this->x=n;}
    int character(){return _character;}
    int m = 0;
    void bomb();
    void setSelected(bool selected) {
        isSelected = selected;
        update();
    }
private:
    bool isSelected = false;

    int x;
    int y;
    int _character;
    void initrectpath();
    QMap<int, QString> Rectpath;
    QMovie* gif;
    QLabel* gifLabel;
signals:
    void mouse_clicked(Rect*);
};

#endif // RECT_H
