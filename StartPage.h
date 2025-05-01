#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QMainWindow>
#include <QMediaPlayer>
#include "GamePage.h"
#include "Show_Button.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class StartPage;
}
QT_END_NAMESPACE

class StartPage : public QMainWindow
{
    Q_OBJECT

public:
    StartPage(
        QWidget *parent
        = nullptr); //初始化界面：设置窗口大小、标题、图标等；初始化游戏界面：创建GameWidget对象；初始化音乐播放器：加载背景音乐文件；连接信号与槽：处理音乐播放结束、按钮点击等事件；显示背景和按钮：通过定时器延迟显示按钮。
    ~StartPage();
    //下一层界面

    //按钮
    Show_Button *startButton;
    GamePage *gamepage;
    QMediaPlayer *bgmGaming, *bgmMainmenu;
    QAudioOutput *audioOutput ;
    bool is_end = false;

private:
    Ui::StartPage *ui;
    void background();     //背景图片
    void backmusic();      //背景音乐
    void show_button();    //显示开始，退出按钮，退出时应该有提示“确认退出？”；按钮需要连接槽函数
    void gamepage_close(); //关闭游戏界面回到主界面时的切换
    void restore();        //恢复主界面时的操作

};
#endif // STARTPAGE_H
