#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include "Hoverbutton.h"

namespace Ui {
class dialogbox;
}

class dialogbox : public QWidget
{
    Q_OBJECT

public:
    explicit dialogbox(QWidget *parent = nullptr);
    QLabel *title();
    Hoverbutton *button();
    QLineEdit *lineEdit();
    QLabel *descriptionlabel();
    QPushButton *mask();
    ~dialogbox();

private:
    Ui::dialogbox *ui;
};

#endif // DIALOGBOX_H
