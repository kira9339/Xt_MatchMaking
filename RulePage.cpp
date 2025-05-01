#include "RulePage.h"
#include <QPixmap>
#include <QPainter>
#include <QLinearGradient>
#include <QApplication>  // 确保包含核心头文件
RulePage::RulePage(QWidget *parent) : QWidget(parent), loadingProgress(0)
{

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_ShowWithoutActivating);  // 防止抢焦点

    // 强制设置固定大小（避免布局问题）
    setFixedSize(1024, 768);
    // 背景图片（居中填充）
    backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/gamepic/lie_che.png").scaled(
        this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    backgroundLabel->setGeometry(0, 0, width(), height());

    // 进度条直线
    const int progressBarWidth = 724; // 进度条固定宽度
    const int progressBarHeight = 10;
    const int progressBarX = (width() - progressBarWidth) / 2;
    const int progressBarY = height() - 80;
    progressLine = new QLabel(this);
    progressLine->setGeometry(progressBarX, progressBarY, progressBarWidth, progressBarHeight);

    // 人物素材
    const int characterSize = 100;
    characterIcon = new QLabel(this);
    characterIcon->setPixmap(QPixmap(":/gamepic/jy.png"));
    characterIcon->setGeometry(
        progressBarX - characterSize/2,            // 居中到进度条起点
        progressBarY - characterSize -55,         // 悬浮在进度条上方
        characterSize,
        characterSize
        );

    // 动画设置（修正后的坐标）
    characterAnimation = new QPropertyAnimation(characterIcon, "geometry", this);
    characterAnimation->setDuration(7000);
    characterAnimation->setStartValue(characterIcon->geometry());
    characterAnimation->setEndValue(QRect(
        progressBarX + progressBarWidth - characterSize/2, // 终点：进度条末端
        characterIcon->y(),
        characterSize,
        characterSize
        ));
    loadingTimer = new QTimer(this);
    connect(loadingTimer, &QTimer::timeout, this, &RulePage::updateLoadingProgress);
    show();
    repaint();
    QApplication::processEvents();
}


void RulePage::startLoading() {
    // 重置状态
    loadingProgress = 0;
    characterIcon->move(140, 620);  // 重置起始位置

    // 启动动画（确保绑定到当前窗口）
    characterAnimation->setTargetObject(characterIcon);
    characterAnimation->start();

    // 启动进度条计时器（总时长7秒）
    loadingTimer->start(70);  }
void RulePage::updateLoadingProgress()
{
    // 处理加载进度逻辑
    loadingProgress++;
    if (loadingProgress >= 100) {
        loadingTimer->stop();
        emit loadingFinished();
    }
}
