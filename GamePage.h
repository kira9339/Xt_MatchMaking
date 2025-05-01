#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QMainWindow>
#include"Settlement.h"
#include<QMediaPlayer>
#include"Hoverbutton.h"
#include"Rect.h"
#include"QProgressBar"
#include<QSoundEffect>
#include<QFileInfo>
#include<QAudioOutput>
#include<QPainter>
#include<QTimer>
#include"dialogbox.h"
#include<QRandomGenerator>
#include<QPropertyAnimation>
#include<QPushButton>
#include<memory.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class GamePage;
}
QT_END_NAMESPACE

// 添加交换状态枚举
enum class SwapState {
    None,
    FirstSelected,
    Swapping,
    Matching,
    Falling
};
struct rect_bomb{
    int cnt;
    bool is_straight;
    int num_straight;
};
class GamePage : public QMainWindow
{
    Q_OBJECT

public:
    GamePage(QWidget *parent = nullptr);

    struct NameScorePair{
        QString name;
        int score;
    };
    const int pre_rect_score= 5;        //每颗宝石分数
    const int pre_straight_score = 5;
    const int straight_score = 5;  //有直边基础加分
    const int rect_4 = 5;//田字形加分
    enum Direction{Center, Left, Up, Down,  Right};
    Settlement *settle_ment;
    dialogbox* settlementDialog;
    QMediaPlayer* bgmGame;
    bool is_end= false;             //背景音乐循环是否停止
    void start();
    QPainter *painter;
    Hoverbutton* menubutton, *revivebutton;
    void onRectClicked(Rect* rect);
    ~GamePage();
protected:
   // bool eventFilter(QObject *watched, QEvent *event) override;//事件过滤器
private:
    void playMatchSound();
    QMediaPlayer* matchSoundEffect; // 消除音效
    QAudioOutput* soundOutput;     // 音效输出
    bool trySwap(int x1, int y1, int x2, int y2);
    bool checkMatch(int x, int y);
    void removeMatches();
    void performSwap();
    SwapState swapState = SwapState::None;
    QPoint firstSelectedPos;
    QPoint secondSelectedPos;
    int rect_type = 5;          //宝石种类数
    QLabel* scorelabel, *decoratelabel;
    QProgressBar* progressbar;
    QWidget* boardwidget;
    QTimer *progresstimer;
    std::unique_ptr<Rect> rects[10][10];
    int len;
    int score=0;
    unsigned int rectBoard[10][10];          //第一个值为x，第二个值为y
    int fallBoard[10][10];
    bool is_acting=false;
    std::vector<Rect*> _2Bomb;//储存方块

    void initbgm();
    void paintEvent(QPaintEvent *event) override;//画游戏界面,因为是虚基类函数所以必须大写
    void init_page();
    void initscene();

    bool isfail();//判断游戏是否失败
    bool isout(Rect* rect);//判断宝石是否可以消除
    void act(Rect* rect);
    rect_bomb Rectbomb(Rect* rect, int character, Direction dir);
    void fallanimation(Rect* target, int h);
    void fillboard();//填充棋盘上的宝石
    int randomrect();
    void badmove(Rect* rect);//错误的处理方式
    void rectshack(Rect* rect);
    void showmenu();
    void sort();
    void return2start();
    void revive();//重置
    void end();
    void fall();//方块下降的逻辑
    bool hasInitialMatches(); // 新增初始化检查函数
    void regenerateUntilValid(); // 新增重新生成函数
    bool eventFilter(QObject *watched, QEvent *event);
    void fillboardImmediate();
    // 添加清理函数声明
    void cleanupAnimations();
    void cleanupGameBoard();
    Hoverbutton* propButton;  // 道具按钮
    int lastPropScore;        // 上次生成道具时的分数
    void initProp();          // 初始化道具
    void checkProp();         // 检查是否需要生成道具
    Hoverbutton* propLoading;
     bool isPropActive;            // 标记道具是否处于激活状态
    void useProp();           // 使用道具
     void showEvent(QShowEvent *event) override;
     Ui::GamePage *ui;
signals:
    void return2menu();
};
#endif // GAMEPAGE_H
