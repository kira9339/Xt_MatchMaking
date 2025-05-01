#include "GamePage.h"
#include "ui_GamePage.h"

GamePage::GamePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePage)
{
    setWindowFlag(Qt::Window);  //设置为独立窗口
    setWindowTitle("星铁消消乐");
    setFixedSize(1024,768);
    boardwidget = nullptr;
    menubutton = nullptr;
    progressbar = nullptr;
    len = this->width()/100*6;
    progresstimer = new QTimer(this);//
    progresstimer->setInterval(1800);
    ui->setupUi(this);
    lastPropScore = 0;  // 初始化为0
    propButton = nullptr;
    initProp();         // 初始化道具
    init_page();
    initbgm();
    initscene();

    if(settle_ment) {
        settle_ment->raise();
    }
    connect(revivebutton, &QPushButton::clicked, this, &GamePage::revive);
    connect(menubutton, &QPushButton::clicked, [=](){
        settle_ment->raise();
        settle_ment->show();
        progresstimer->stop();
    });
   // ui->setupUi(this);
}
void GamePage::initbgm(){

    bgmGame = new QMediaPlayer(this);
    QAudioOutput *audioOutput = new QAudioOutput(this);
    bgmGame->setAudioOutput(audioOutput);  // 关键步骤！

    QString musicPath = "D:/Xt_Matchmaking/GameMusic/game_music.mp3";
    bgmGame->setProperty("metadataNetworkAccessEnabled", false);
    bgmGame->setSource(QUrl::fromLocalFile(musicPath));


    connect(bgmGame, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia && !is_end)
            bgmGame->play();
    });//循环播放

    matchSoundEffect = new QMediaPlayer(this);
    soundOutput = new QAudioOutput(this);
    matchSoundEffect->setAudioOutput(soundOutput);
    QString soundPath = "D:/Xt_Matchmaking/GameMusic/move.mp3";
    matchSoundEffect->setSource(QUrl::fromLocalFile(soundPath));
    soundOutput->setVolume(0.9f); // 设置音量(0.0-1.0)
}

void GamePage::paintEvent(QPaintEvent *event){
    QPainter painter(this);//不要忘记头文件
    //背景图片
    painter.drawPixmap(0,0,this->width(), this->height(), QPixmap(":/game_background.jpg"));
    //方格
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0,0,0,255));
    painter.setOpacity(0.6);
    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j)
            if((i + j) % 2 == 0)
                painter.drawRect(this->width()/20*7 + len*j, (this->height()- 10*len)/2 + len*i, len, len);
    painter.setOpacity(0.5);
    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j)
            if((i + j) % 2 == 1)
                painter.drawRect(this->width()/20*7 + len*j, (this->height()- 10*len)/2 + len*i, len, len);


    //棋盘上下装饰
    painter.setOpacity(1);
    painter.drawPixmap(this->width()/20*7, (this->height()- 10*len)/2 - len*3/8, 10*len, len*5/8, QPixmap(":/gamepic/bottom.png"));

    //记分器
    painter.drawPixmap(this->width()/20, this->height()/8, this->width()/5, (int)this->width() * 0.1157, QPixmap(":/gamepic/score.png"));


    QWidget::paintEvent(event);
}




void GamePage::init_page(){//初始化按钮
    menubutton=new Hoverbutton(this);
    revivebutton=new Hoverbutton(this);
    scorelabel =new QLabel(this);
    decoratelabel=new QLabel(this);
    progressbar = new QProgressBar(this);
    menubutton->setGeometry(60,250,(int)this->width()/5*97.5/288, (int)this->width()/5*97.5/288);
    menubutton->setImage(60, 250,":/gamepic/menu.png");
    revivebutton->setGeometry(170,250,(int)this->width()/5*97.5/288, (int)this->width()/5*97.5/288);
    revivebutton->setImage(170, 250,":/gamepic/revive.png");
    revivebutton->setEnabled(true);
    scorelabel->setGeometry(this->width()/20 + (int)this->width()/5*22.68/280,
                            this->height()/8 + (int)this->width()*0.1157*38.23/162,
                            (int)this->width()/5*235.73/280,
                            (int)this->width()*0.1157*57.43/162);
    scorelabel->setFont(QFont("Microsoft YaHei", 16, QFont::Bold));
    scorelabel->setAlignment(Qt::AlignCenter);
    scorelabel->setStyleSheet("QLabel{color:black; background:transparent;}");
    scorelabel->setText("0");
    decoratelabel->setStyleSheet("QLabel{border-image:url(:/gamepic/bottom.png);}");
    progressbar->setGeometry(this->width()/20*7 + 20, this->height() - len, 10*len - 40, len*7/10 - 15);
    decoratelabel->setGeometry(this->width()/20*7 - 20, this->height() - len - 10, 10*len + 30, len*7/10 + 5);
    progressbar->setRange(0,99);
    progressbar->setValue(0);
    progressbar->setFormat("%p%");
    progressbar->setAlignment(Qt::AlignCenter);
    progressbar->setFont(QFont("Microsoft YaHei", 12, 100));
    progressbar->setStyleSheet("QProgressBar{color:grey;} QProgressBar::chunk{background-color:#24247e}");

    connect(progresstimer, &QTimer::timeout, [=](){
        if(progressbar->value() == 99)
            end();
        else
            progressbar->setValue(progressbar->value()+1);
    });

    settle_ment = new Settlement(this);
    settle_ment->setGeometry(0,0,1024,768);
    settle_ment->hide();
    connect(settle_ment->mask(), &QPushButton::clicked, [=](){
        settle_ment->hide();
        progresstimer->start();
    });
    connect(settle_ment->button(), &Hoverbutton::clicked, this, &GamePage::return2start);

    settlementDialog = new dialogbox(this);
    settlementDialog->hide();
    settlementDialog->setGeometry(0,0,1024,768);
    settlementDialog->descriptionlabel()->setText("您的得分为");
    settlementDialog->lineEdit()->setReadOnly(true);
    connect(settlementDialog->button(), &Hoverbutton::clicked, this, &GamePage::return2start);
}


int GamePage::randomrect(){

    return QRandomGenerator::global()->bounded(0, 5);

}




void GamePage::initscene() {
    boardwidget = new QWidget(this);
    boardwidget->show();
    boardwidget->setGeometry(1024/20*7, (768-10*len)/2, 10*len, 10*len);

    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            int type;
            bool valid = false;

            // 确保初始棋盘没有匹配
            for(int attempt = 0; attempt < 100 && !valid; ++attempt) {
                type = QRandomGenerator::global()->bounded(0, 5);
                valid = true;

                // 检查水平方向
                if(j >= 2 && rectBoard[i][j-1] == type && rectBoard[i][j-2] == type) {
                    valid = false;
                }

                // 检查垂直方向
                if(i >= 2 && rectBoard[i-1][j] == type && rectBoard[i-2][j] == type) {
                    valid = false;
                }
            }

            rectBoard[i][j] = type;
            rects[i][j] = std::make_unique<Rect>(type, len, i, j, 0, boardwidget);
            rects[i][j]->installEventFilter(this);
            rects[i][j]->show();
            connect(rects[i][j].get(), &Rect::mouse_clicked, this, &GamePage::onRectClicked);
        }
    }
}

void GamePage::onRectClicked(Rect* rect) {
    if (!rect || is_acting) return;

    int x = rect->X();
    int y = rect->Y();

    switch(swapState) {
    case SwapState::None:
        firstSelectedPos = QPoint(x, y);
        rects[x][y]->setSelected(true);
        swapState = SwapState::FirstSelected;
        break;

    case SwapState::FirstSelected:
        secondSelectedPos = QPoint(x, y);

        // 检查是否相邻
        if ((qAbs(firstSelectedPos.x() - x) == 1 && firstSelectedPos.y() == y) ||
            (qAbs(firstSelectedPos.y() - y) == 1 && firstSelectedPos.x() == x)) {

            rects[firstSelectedPos.x()][firstSelectedPos.y()]->setSelected(false);
            is_acting = true;
            swapState = SwapState::Swapping;
            performSwap();
        } else {
            // 取消选择
            rects[firstSelectedPos.x()][firstSelectedPos.y()]->setSelected(false);
            firstSelectedPos = QPoint(x, y);
            rects[x][y]->setSelected(true);
        }
        break;

    default:
        break;
    }
}

void GamePage::performSwap() {
    int x1 = firstSelectedPos.x();
    int y1 = firstSelectedPos.y();
    int x2 = secondSelectedPos.x();
    int y2 = secondSelectedPos.y();

    // 交换动画
    QPropertyAnimation* anim1 = new QPropertyAnimation(rects[x1][y1].get(), "pos");
    QPropertyAnimation* anim2 = new QPropertyAnimation(rects[x2][y2].get(), "pos");

    anim1->setDuration(300);
    anim2->setDuration(300);

    anim1->setStartValue(rects[x1][y1]->pos());
    anim1->setEndValue(rects[x2][y2]->pos());

    anim2->setStartValue(rects[x2][y2]->pos());
    anim2->setEndValue(rects[x1][y1]->pos());

    // 实际交换数据
    QObject::connect(anim1, &QPropertyAnimation::finished, [=]() {
        std::swap(rectBoard[x1][y1], rectBoard[x2][y2]);
        std::swap(rects[x1][y1], rects[x2][y2]);

        rects[x1][y1]->setX(x1);
        rects[x1][y1]->setY(y1);
        rects[x2][y2]->setX(x2);
        rects[x2][y2]->setY(y2);

        anim1->deleteLater();
        anim2->deleteLater();

        // 检查匹配
        if (checkMatch(x1, y1) || checkMatch(x2, y2)) {
            removeMatches();
        } else {
            // 没有匹配，交换回来
            QPropertyAnimation* anim3 = new QPropertyAnimation(rects[x1][y1].get(), "pos");
            QPropertyAnimation* anim4 = new QPropertyAnimation(rects[x2][y2].get(), "pos");

            anim3->setDuration(300);
            anim4->setDuration(300);

            anim3->setStartValue(rects[x1][y1]->pos());
            anim3->setEndValue(rects[x2][y2]->pos());

            anim4->setStartValue(rects[x2][y2]->pos());
            anim4->setEndValue(rects[x1][y1]->pos());

            QObject::connect(anim3, &QPropertyAnimation::finished, [=]() {
                std::swap(rectBoard[x1][y1], rectBoard[x2][y2]);
                std::swap(rects[x1][y1], rects[x2][y2]);

                rects[x1][y1]->setX(x1);
                rects[x1][y1]->setY(y1);
                rects[x2][y2]->setX(x2);
                rects[x2][y2]->setY(y2);

                anim3->deleteLater();
                anim4->deleteLater();

                swapState = SwapState::None;
                is_acting = false;
            });

            anim3->start();
            anim4->start();
        }
    });

    anim1->start();
    anim2->start();
}

bool GamePage::checkMatch(int x, int y) {
    int type = rectBoard[x][y];
    if (type == 100) return false; // 空白格子

    // 水平方向检查
    int left = x;
    while (left > 0 && rectBoard[left-1][y] == type) left--;

    int right = x;
    while (right < 9 && rectBoard[right+1][y] == type) right++;

    // 垂直方向检查
    int top = y;
    while (top > 0 && rectBoard[x][top-1] == type) top--;

    int bottom = y;
    while (bottom < 9 && rectBoard[x][bottom+1] == type) bottom++;

    // 返回是否有至少三个匹配
    return (right - left >= 2) || (bottom - top >= 2);
}

void GamePage::playMatchSound() {
    if(matchSoundEffect) {
        matchSoundEffect->setPosition(0); // 重置播放位置
        matchSoundEffect->play();
    }
}

void GamePage::removeMatches() {
    QVector<QPoint> toRemove;

    // 标记要消除的方块
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (rectBoard[i][j] == 100) continue;

            // 水平方向检查
            int left = i;
            while (left > 0 && rectBoard[left-1][j] == rectBoard[i][j]) left--;
            int right = i;
            while (right < 9 && rectBoard[right+1][j] == rectBoard[i][j]) right++;

            if (right - left >= 2) {
                for (int x = left; x <= right; ++x) {
                    toRemove.append(QPoint(x, j));
                }
            }

            // 垂直方向检查
            int top = j;
            while (top > 0 && rectBoard[i][top-1] == rectBoard[i][j]) top--;
            int bottom = j;
            while (bottom < 9 && rectBoard[i][bottom+1] == rectBoard[i][j]) bottom++;

            if (bottom - top >= 2) {
                for (int y = top; y <= bottom; ++y) {
                    toRemove.append(QPoint(i, y));
                }
            }
        }
    }

    // 去重
    QSet<QPoint> uniqueToRemove(toRemove.begin(), toRemove.end());
    toRemove = uniqueToRemove.values();

    if (toRemove.isEmpty()) {
        swapState = SwapState::None;
        is_acting = false;
        return;
    }
    // 播放消除音效(MP3格式)
 playMatchSound();
    score += toRemove.size() * pre_rect_score;
    scorelabel->setText(QString::number(score));
    checkProp();
    // 消除方块
    for (const QPoint& pos : toRemove) {
        if (rectBoard[pos.x()][pos.y()] != 100) {
            rectBoard[pos.x()][pos.y()] = 100;
            if (rects[pos.x()][pos.y()]) {
                rects[pos.x()][pos.y()]->hide();
                rects[pos.x()][pos.y()].reset();
            }
        }
    }

    // 执行下落和填补
    fall();
    fillboard();

    // 检查是否有新的匹配
    bool hasNewMatches = false;
    for (int i = 0; i < 10 && !hasNewMatches; ++i) {
        for (int j = 0; j < 10 && !hasNewMatches; ++j) {
            if (rectBoard[i][j] != 100) {
                hasNewMatches = checkMatch(i, j);
            }
        }
    }

    if (hasNewMatches) {
        QTimer::singleShot(300, this, &GamePage::removeMatches); // 增加延迟确保动画完成
    } else {
        swapState = SwapState::None;
        is_acting = false;

        if (isfail()) {
            if (revivebutton->isEnabled()) {
                revive();
            } else {
                end();
            }
        }
    }
}

void GamePage::start(){
    settle_ment->hide();
    settle_ment->raise();
    settlementDialog->hide();
    settlementDialog->raise();

    revivebutton->setImage(170,250,":/gamepic/revive.png");
    revivebutton->setEnabled(true);
    progressbar->setValue(0);

    score = 0;
    scorelabel->setText("0");
    progressbar->setValue(0);

    lastPropScore = 0;
    propButton->hide();
    propButton->setEnabled(false);
    propLoading->show(); // 游戏开始时显示加载状态
    if (progresstimer) {
        progresstimer->start();
    }
    progresstimer->setInterval(1800);//
    qDebug() << "GamePage object address:" << this;
    qDebug() << "progresstimer address:" << progresstimer;
    if (!progresstimer) {
        qDebug() << "progresstimer is nullptr!";
        return;
    }
    progresstimer->start();
}

void GamePage::fall() {
    // 按列处理
    for (int col = 0; col < 10; ++col) {
        // 从底部向上查找空缺
        int emptyRow = -1;
        for (int row = 9; row >= 0; --row) {
            if (rectBoard[col][row] == 100 && emptyRow == -1) {
                emptyRow = row; // 找到第一个空缺位置
            }
            else if (rectBoard[col][row] != 100 && emptyRow != -1) {
                // 找到可以下落的方块
                rectBoard[col][emptyRow] = rectBoard[col][row];
                rectBoard[col][row] = 100;

                // 移动方块对象
                rects[col][emptyRow] = std::move(rects[col][row]);
                rects[col][emptyRow]->setY(emptyRow);

                // 执行下落动画
                QPropertyAnimation* animation = new QPropertyAnimation(rects[col][emptyRow].get(), "pos");
                animation->setDuration(300);
                animation->setStartValue(QPoint(len * col, len * row));
                animation->setEndValue(QPoint(len * col, len * emptyRow));
                animation->start(QAbstractAnimation::DeleteWhenStopped);

                // 更新空缺位置
                row = emptyRow; // 继续检查当前位置
                emptyRow = -1;
            }
        }
    }
}


void GamePage::fillboard() {
    // 从顶部填补空缺
    for (int col = 0; col < 10; ++col) {
        for (int row = 0; row < 10; ++row) {
            if (rectBoard[col][row] == 100) {
                int type = randomrect();
                rectBoard[col][row] = type;

                // 创建新方块并设置初始位置在顶部上方
                rects[col][row] = std::make_unique<Rect>(type, len, col, -1, 0, boardwidget);
                rects[col][row]->setY(row);
                rects[col][row]->installEventFilter(this);
                connect(rects[col][row].get(), &Rect::mouse_clicked, this, &GamePage::onRectClicked);

                // 执行下落动画
                QPropertyAnimation* animation = new QPropertyAnimation(rects[col][row].get(), "pos");
                animation->setDuration(300);
                animation->setStartValue(QPoint(len * col, -len));
                animation->setEndValue(QPoint(len * col, len * row));
                animation->start(QAbstractAnimation::DeleteWhenStopped);

                rects[col][row]->show();
            }
        }
    }
}




void GamePage::fallanimation(Rect* rect, int h) {
    if (!rect) return;

    QPropertyAnimation* animation = new QPropertyAnimation(rect, "geometry");
    animation->setDuration(300);
    animation->setStartValue(rect->geometry());
    animation->setEndValue(QRect(rect->X(), rect->Y() + len * h, rect->width(), rect->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);

    // 自动删除动画对象
    animation->connect(animation, &QPropertyAnimation::finished, [animation]() {
        animation->deleteLater();
    });

    animation->start();
}

void GamePage::rectshack(Rect *rect){
    QPropertyAnimation* animation = new QPropertyAnimation(rect, "geometry", this);
    animation->setDuration(300);
    animation->setStartValue(rect->geometry());
    animation->setKeyValueAt(0.25, QRect(rect->geometry().x() + 10, rect->geometry().y(), rect->width(), rect->height()));
    animation->setKeyValueAt(0.75, QRect(rect->geometry().x() - 10, rect->geometry().y(), rect->width(), rect->height()));
    animation->setEndValue(rect->geometry());
    animation->setEasingCurve(QEasingCurve::InOutCubic);
    animation->start();
    QTimer::singleShot(1000, this, [=](){
        delete animation;
    });
}


bool GamePage::eventFilter(QObject *watched, QEvent *event){              //动画进行中禁用点击事件

    if(watched->metaObject()->className() == QStringLiteral("Rect") &&
        is_acting &&
        (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick)) {
        return true;
    }
    return QMainWindow::eventFilter(watched, event);
}

void GamePage::initProp() {
    // 道具按钮
    propButton = new Hoverbutton(this);
    propButton->setGeometry(100, 500, (int)this->width()*187.11/5/300, (int)this->width()*187.11/5/300);
    propButton->setImage(100, 500, ":/gamepic/yuzhoudachaofan.png");
    propButton->hide();
    propButton->setEnabled(false);
    connect(propButton, &QPushButton::clicked, this, &GamePage::useProp);

    // 加载状态图片
    propLoading = new Hoverbutton(this);
    propLoading->setGeometry(100, 400, (int)this->width()*187.11/5/300, (int)this->width()*187.11/5/300);
    propLoading->setImage(100, 400, ":/gamepic/danheng.png");
    propLoading->setEnabled(false);
    propLoading->show();

    isPropActive = false;
    lastPropScore = 0;
}

void GamePage::checkProp() {
    if (score - lastPropScore >= 200 && !isPropActive) {
        // 切换为道具状态
        propLoading->hide();
        propButton->show();
        propButton->setEnabled(true);
        isPropActive = true;
        lastPropScore = score;

        // 道具出现动画
        QPropertyAnimation* anim = new QPropertyAnimation(propButton, "geometry");
        anim->setDuration(500);
        anim->setStartValue(QRect(100, 300, propButton->width(), propButton->height()));
        anim->setEndValue(QRect(100, 350, propButton->width(), propButton->height()));
        anim->setEasingCurve(QEasingCurve::OutBounce);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void GamePage::useProp() {
    if (is_acting) return;

    is_acting = true;

    // 道具消失动画
    QPropertyAnimation* anim = new QPropertyAnimation(propButton, "geometry");
    anim->setDuration(300);
    anim->setStartValue(propButton->geometry());
    anim->setEndValue(QRect(100, 500, propButton->width(), propButton->height()));
    anim->setEasingCurve(QEasingCurve::InBack);

    connect(anim, &QPropertyAnimation::finished, [=]() {
        propButton->hide();
        propButton->setEnabled(false);
        propLoading->show(); // 切换回加载状态
        isPropActive = false;

        // 执行道具效果
        int row = QRandomGenerator::global()->bounded(0, 10);
        QVector<QPoint> toRemove;
        for (int col = 0; col < 10; ++col) {
            if (rectBoard[col][row] != 100) {
                toRemove.append(QPoint(col, row));
            }
        }

        if (!toRemove.isEmpty()) {
            score += toRemove.size() * pre_rect_score;
            scorelabel->setText(QString::number(score));

            for (const QPoint& pos : toRemove) {
                if (rectBoard[pos.x()][pos.y()] != 100) {
                    rectBoard[pos.x()][pos.y()] = 100;
                    if (rects[pos.x()][pos.y()]) {
                        rects[pos.x()][pos.y()]->bomb();
                        rects[pos.x()][pos.y()].reset();
                    }
                }
            }

            fall();
            fillboard();
        }

        is_acting = false;
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void GamePage::revive() {
    // 清除当前所有方块
    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            if (rects[i][j]) {
                rects[i][j]->bomb();
                rects[i][j] = nullptr;
            }
        }
    }

    // 删除旧的 boardwidget
    delete boardwidget;

    // 重新初始化场景
    initscene();

    // 为所有方块添加震动动画
    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            rectshack(rects[i][j].get());
        }
    }

    // 重置游戏状态
    progressbar->setValue(0);
    // score = 0;
    // scorelabel->setNum(score);
    score = 0;
    scorelabel->setText("0");
    // 重新启用 revive 按钮
    revivebutton->setEnabled(true);

    // 重新开始计时器
    progresstimer->start();
}
void GamePage::return2start(){
    settlementDialog->hide();
    settle_ment->hide();
    emit return2menu();
    QTimer::singleShot(500, this, [=](){
        for(int i = 0; i < 10; ++i)
            for(int j = 0; j < 10; ++j)
                rects[i][j]->bomb();
        delete boardwidget;
        initscene();
        this->hide();
    });
}


bool GamePage::isfail() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            // 检查右侧交换
            if (i < 9) {
                std::swap(rectBoard[i][j], rectBoard[i+1][j]);
                if (checkMatch(i, j) || checkMatch(i+1, j)) {
                    std::swap(rectBoard[i][j], rectBoard[i+1][j]);
                    return false;
                }
                std::swap(rectBoard[i][j], rectBoard[i+1][j]);
            }

            // 检查下方交换
            if (j < 9) {
                std::swap(rectBoard[i][j], rectBoard[i][j+1]);
                if (checkMatch(i, j) || checkMatch(i, j+1)) {
                    std::swap(rectBoard[i][j], rectBoard[i][j+1]);
                    return false;
                }
                std::swap(rectBoard[i][j], rectBoard[i][j+1]);
            }
        }
    }
    return true; // 没有可能的移动
}

bool GamePage::isout(Rect *rect){       //只判断是否有三个相邻
    int cnt=1;
    if(rect->X()>0 && rects[rect->X()-1][rect->Y()]->character() == rect->character())
        cnt++;
    if(rect->X()<9 && rects[rect->X()+1][rect->Y()]->character() == rect->character())
        cnt++;
    if(rect->Y()>0 && rects[rect->X()][rect->Y()-1]->character() == rect->character())
        cnt++;
    if(rect->Y()<9 && rects[rect->X()][rect->Y()+1]->character() == rect->character())
        cnt++;
    return cnt>=3;
}

void GamePage::sort(){

}



void GamePage::end(){
    progresstimer->stop();

    // 停止背景音乐
    if(bgmGame) {
        bgmGame->stop();
    }

    if(progressbar->value() == 99){
        settlementDialog->title()->setText("时间到");
    }
    else{
        settlementDialog->title()->setText("无法继续开拓了");
    }

    settlementDialog->lineEdit()->setText(QString::number(score));
    settlementDialog->raise();
    settlementDialog->show();
    revivebutton->setEnabled(true);


}

void GamePage::cleanupAnimations()
{
    // 查找并停止所有动画
    foreach(QPropertyAnimation* anim, findChildren<QPropertyAnimation*>()) {
        anim->stop();
        anim->deleteLater();
    }
}


void GamePage::cleanupGameBoard()
{
    // 清理游戏板
    if(boardwidget) {
        // 先移除所有子控件
        QList<Rect*> rects = boardwidget->findChildren<Rect*>();
        foreach(Rect* rect, rects) {
            rect->hide();
            rect->deleteLater();
        }

        delete boardwidget;
        boardwidget = nullptr;
    }

    // 清理数组中的智能指针
    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            if(rects[i][j]) {
                rects[i][j].reset();
            }
        }
    }
}
void GamePage::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);  // 先调用父类处理


    if (bgmGame && bgmGame->playbackState() != QMediaPlayer::PlayingState) {
        bgmGame->play();
    }
}
GamePage::~GamePage()
{
    // 停止所有动画
    cleanupAnimations();

    // 停止音乐
    if(bgmGame) {
        bgmGame->stop();
        bgmGame->deleteLater();
        bgmGame = nullptr;
    }
    if(matchSoundEffect) {
        matchSoundEffect->stop();
        matchSoundEffect->deleteLater();
        matchSoundEffect = nullptr;
    }
    // 停止计时器
    if(progresstimer) {
        progresstimer->stop();
        delete progresstimer;
        progresstimer = nullptr;
    }

    // 清理游戏板
    cleanupGameBoard();


    delete ui;
}
