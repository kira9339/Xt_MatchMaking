#ifndef SETTLEMENT_H
#define SETTLEMENT_H

#include <QWidget>
#include<QPushButton>
#include"Hoverbutton.h"
#include <QTableWidget>
#include <QTextEdit>
namespace Ui {
class Settlement;
}

class Settlement : public QWidget
{
    Q_OBJECT

public:
    explicit Settlement(QWidget *parent = nullptr);
    ~Settlement();
    QPushButton* mask();
    Hoverbutton* button();
\
private:
    Ui::Settlement *ui;
};

#endif // SETTLEMENT_H
