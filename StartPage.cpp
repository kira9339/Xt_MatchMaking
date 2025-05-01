#include "StartPage.h"
#include "ui_StartPage.h"
#include <QMainWindow>
#include <QTimer>
#include<QMediaPlayer>
#include <QFileInfo>
#include<Show_Button.h>
#include<QPixmap>
#include<QLabel>
#include<RulePage.h>
#include<QPushButton>
StartPage::StartPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartPage)
{
    ui->setupUi(this);
    setFixedSize(1024, 576);//
    setWindowTitle("星铁消消乐");
    gamepage = new GamePage(parent);


    bgmMainmenu= new QMediaPlayer(this);
    QAudioOutput *audioOutput = new QAudioOutput(this);
    bgmMainmenu->setAudioOutput(audioOutput);  // 关键步骤！
    QString musicPath = "D:/Xt_Matchmaking/GameMusic/start_music.mp3";
    bgmMainmenu->setProperty("metadataNetworkAccessEnabled", false);
    bgmMainmenu->setSource(QUrl::fromLocalFile(musicPath));
    QObject::connect(bgmMainmenu, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            // 当音乐播放结束时，将播放位置设置为开始
            bgmMainmenu->setPosition(0);
            // 重新开始播放音乐
            bgmMainmenu->play();
        }
    });
    QObject::connect(bgmMainmenu, &QMediaPlayer::errorOccurred, [=](QMediaPlayer::Error error) {
        qDebug() << "音乐播放出错: " << bgmMainmenu->errorString();
    });
    bgmMainmenu->play();//播放音乐
    background();//调出背景
    show_button();
}

StartPage::~StartPage()
{

    delete ui;
}
void StartPage::background() {
    // 背景图片相关操作代码
    QPixmap pix;
    QLabel *pic = new QLabel(this); // QLabel用于显示QPixmap类型的图像
    if (!pix.load(":/gamepic/backpic.png")) {
        qDebug() << "导入背景图片错误，文件可能不存在或格式不正确。";
        QFileInfo fileInfo(":/gamepic/backpic.png");
        if (fileInfo.exists()) {
            qDebug() << "文件存在，但加载失败，可能是格式问题。";
        } else {
            qDebug() << "文件不存在，请检查资源文件配置。";
        }
    } else {
        qDebug() << "背景图片加载成功。";
        pic->setPixmap(pix);
        pic->setGeometry(0, 0, pix.width(), pix.height());
        pic->lower(); // 将背景图片置于最底层
    }
}

void StartPage::show_button() {
    startButton = new Show_Button(340, 370, 321, 41,
                                  ":/gamepic/start_button_plus.png",this);
    startButton->raise();

    connect(startButton, &Show_Button::clicked, this, [=]() {
        startButton->setEnabled(false);

        // 创建规则页（必须无父对象！）
        RulePage *rulePage = new RulePage();  // 无parent！
        rulePage->setAttribute(Qt::WA_DeleteOnClose);
        bgmMainmenu->stop();
        // 连接信号（使用Qt::DirectConnection确保实时响应）
        connect(rulePage, &RulePage::loadingFinished, this, [=]() {
            gamepage->show();
            this->hide();
            gamepage->start();
        }, Qt::DirectConnection);

        // 显示后立即开始动画
        rulePage->show();
        rulePage->startLoading();  // 内部包含7秒动画逻辑

        // 7秒后自动触发结束
        QTimer::singleShot(7000, rulePage, [=]() {
            emit rulePage->loadingFinished();
        });
    });

}
void StartPage::restore(){
    setWindowOpacity(1);
    startButton->setVisible(true);
}
