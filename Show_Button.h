#ifndef SHOW_BUTTON_H
#define SHOW_BUTTON_H

#include <QMainWindow>
#include<QLabel>
#include<QPushButton>
#include<QString>
#include<QTimer>
#include"Hoverbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Show_Button;
}
QT_END_NAMESPACE

class Show_Button : public QLabel
{
    Q_OBJECT

public:
    explicit Show_Button(int x, int y, int width, int height, QString path,  QWidget *parent = nullptr);
    void show();
    void setVisible(bool);
    void setImage(int x,int y,QString pathin);

    ~Show_Button();

private:
    Ui::Show_Button *ui;
    Hoverbutton*startbutton =new Hoverbutton;
    QLabel *QPushbutton=new QLabel;

signals:
    void clicked();
};
#endif // Show_Button
