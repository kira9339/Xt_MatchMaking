#ifndef RULEPAGE_H
#define RULEPAGE_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QLabel>
#include <QTimer>

class RulePage : public QWidget
{
    Q_OBJECT

public:
    explicit RulePage(QWidget *parent = nullptr);
    void startLoading();

signals:
    void loadingFinished();

private:
    QLabel *backgroundLabel;
    QLabel *progressLine;
    QLabel *characterIcon;
    QPropertyAnimation *characterAnimation;
    QTimer *loadingTimer;
    void updateLoadingProgress();
    int loadingProgress;
};

#endif // RULEPAGE_H
