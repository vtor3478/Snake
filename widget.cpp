#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    delayLevel[0] = {1000};
    delayLevel[1] = {500};
    delayLevel[2] = {200};
    delayLevel[3] = {150};
    delayLevel[4] = {100};
    delayLevel[5] = {50};
    delayLevel[6] = {20};
    delayLevel[7] = {10};

    Init();

    connect(&runTimer,&QTimer::timeout,[=](){
        GetDir();
        SnakeRun();
        CheckWin();
        CheckFail();
        CreateFood();
    });
    updateTimer.setInterval(10);
    updateTimer.start();
    connect(&updateTimer,&QTimer::timeout,[=](){
        update();
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Init()
{
    grade = 0;
    winFlag = 0;
    failFlag = 0;
    autoFlag = 0;
    invincibleFlag = 0;
    snakeBody[0] = QPoint(5,5);
    snakeBody[1] = snakeBody[0] + QPoint(-1,0);
    snakeBody[2] = snakeBody[1] + QPoint(-1,0);
    snakeLen = 3;
    keyDir = QPoint(0,0);
    snakeDir = QPoint(0,0);
    delayLevelIdx = DEFAULT_DELAY_LEVEL;
    runTimer.setInterval(delayLevel[delayLevelIdx]);
    runTimer.start();
}

void Widget::CheckFail()
{
    // 如果无敌，直接返回
    if (invincibleFlag) {
        return ;
    }
    for (int i = 1;i < snakeLen - 1; i++)
    {
        // 吃到了身体，或者超出了边界，就认定为fail
        if (snakeBody[i] == snakeBody[0]) {
            failFlag = EAT_YOURSELF;
            runTimer.stop();
        } else if (snakeBody[0].x() < 0
                || snakeBody[0].x() >= ROW
                || snakeBody[0].y() < 0
                || snakeBody[0].y() >= COL)
        {
            failFlag = HIT_THE_WALL;
            runTimer.stop();
        }
    }
}

void Widget::CheckWin()
{
    int i,j;
    int emptyExist = 0;
    memset(snakeOccupy,0,sizeof(snakeOccupy));
    for ( i = 0;i < snakeLen; i++) {
        snakeOccupy[snakeBody[i].x()][snakeBody[i].y()] = 1;

    }
    for (i = 0;(0 == emptyExist) && i < COL; i++) {
        for (j = 0;(0 == emptyExist) &&  j < ROW; j ++) {
            if (0 == snakeOccupy[i][j])
            {
                // 存在空格，可以启用随机数放置食物
                emptyExist = 1;
            }
        }
    }
    // 不存在空格子用于存放食物，那么认为此局已经赢了
    if (0 == emptyExist) {
        winFlag = 1;
        runTimer.stop();
    }
}

void Widget::GetDir()
{
    // 是否启用自动吃东西，仅用于debug
    if (autoFlag) {
        // 此处设置，不会自动退出自动模式
        QPoint autoDir = food - snakeBody[0];
        int max = qMax(qAbs(autoDir.x()),qAbs(autoDir.y()));
        autoDir /= max;
        if (autoDir.x() != 0) {
            autoDir.ry() = 0;
        }
        snakeDir = autoDir;
    }
    // 如果按下了方向键，切换蛇头方向，需要注意跨界时的反头
    else if (keyDir != snakeBody[1] + QPoint(0,0) - snakeBody[0]
            && keyDir != snakeBody[1] + QPoint(ROW,0) - snakeBody[0]
            && keyDir != snakeBody[1] + QPoint(-ROW,0) - snakeBody[0]
            && keyDir != snakeBody[1] + QPoint(0,COL) - snakeBody[0]
            && keyDir != snakeBody[1] + QPoint(0,-COL) - snakeBody[0])
    {
        snakeDir = keyDir;
        // 如果按下了方向键，那么回到手动模式
        autoFlag = 0;
    }
}

void Widget::SnakeRun()
{
    // 蛇头有方向，那么蛇就运动
    if (QPoint(0,0) == snakeDir)
    {
        return;
    }
    // 运动时，先收缩尾巴，再伸出头，所以身子运动在前面
    for (int i = snakeLen - 1;i > 0; i--)
    {
        snakeBody[i] = snakeBody[i-1];
    }
    snakeBody[0] += snakeDir;
    // 无敌模式，穿墙，自动去到另一边
    if (invincibleFlag) {
        snakeBody[0].rx() = (snakeBody[0].x() + ROW) % ROW;
        snakeBody[0].ry() = (snakeBody[0].y() + COL) % COL;
    }
    if(food == snakeBody[0])
    {
        // 将身子最后一节赋值，就不会显示到00了
        snakeBody[snakeLen] = snakeBody[snakeLen - 1];
        ++snakeLen;
        ++grade;
        food = QPoint(-1,-1);
    }
}
void Widget::CreateFood()
{
    int i,j;
    // 如果已经没有空间放下食物，说明游戏胜利
    if (QPoint(-1,-1) != food)
    {
        return;
    }
    while (1) {
        i = qrand() % ROW;
        j = qrand() % COL;
        if (0 == snakeOccupy[i][j]){
            food = QPoint(i,j);
            break;
        }

    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if ("w" == event->text()
            || "s" == event->text()
            || "a" == event->text()
            || "d" == event->text())
    {
        if ("w" == event->text()) {
            keyDir = QPoint(0,-1);
        }
        else if ("s" == event->text()) {
            keyDir = QPoint(0,1);
        }
        else if ("a" == event->text()) {
            keyDir = QPoint(-1,0);
        }
        else if ("d" == event->text()) {
            keyDir = QPoint(1,0);
        }
        // 按下了方向键，就退出自动模式
        autoFlag = 0;
    }
    // 加入z键，进行暂停操作
    else if ("z" == event->text())
    {
        keyDir = QPoint(0,0);
    }
    else if ("x" == event->text())
    {
        autoFlag = 1;
    }
    else if ("c" == event->text())
    {
        // c键确认无敌，且能穿墙
        invincibleFlag = !invincibleFlag;
    }
    else if ("r" == event->text()) {
        Init();
    }
    if ("q" == event->text()) {
        if (delayLevelIdx > 0) {
            -- delayLevelIdx;
            runTimer.setInterval(delayLevel[delayLevelIdx]);
        }
    }
    else if ("e" == event->text()) {
        if (delayLevelIdx < DELAY_LEVEL) {
            ++ delayLevelIdx;
            runTimer.setInterval(delayLevel[delayLevelIdx]);
        }
    }
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (invincibleFlag)
    {
        // 无敌模式，可以穿墙，画笔颜色为红色
        painter.setPen(qRgb(255,0,0));
    }
    QString str = "food:" + QString::number(food.x()) + QString::number(food.y());
    painter.drawText(QPoint(COL,2) * SCALE + QPoint(10,0),str);
    str = "grade="+QString::number(grade);
    painter.drawText(QPoint(COL,3) * SCALE + QPoint(10,0),str);

    painter.drawLine(QPoint(COL,0) * SCALE, QPoint(COL,ROW) * SCALE);
    painter.drawLine(QPoint(0,ROW) * SCALE, QPoint(COL,ROW) * SCALE);

    for(int i = snakeLen-1; i >=0 ; i--)
    {
        painter.drawEllipse(snakeBody[i] * SCALE + QPoint(SCALE / 2,SCALE / 2),
                            SNAKE_WIDTH / 2, SNAKE_WIDTH / 2);
    }
    // 设置画刷，重画蛇头，突出食物位置
    painter.setBrush(QBrush(qRgb(0,255,0)));
    painter.drawEllipse(snakeBody[0] * SCALE + QPoint(SCALE / 2,SCALE / 2),
            SNAKE_WIDTH / 2, SNAKE_WIDTH / 2);
    painter.drawEllipse(food * SCALE + QPoint(SCALE / 2,SCALE / 2) ,
                        FOOD_WIDTH / 2,FOOD_WIDTH / 2);
    
    if (winFlag) {
        painter.drawText(QPoint(COL,4) * SCALE + QPoint(10,20 * 0),"!!!win!!!");
    }
    else if (failFlag == EAT_YOURSELF) {
        painter.drawText(QPoint(COL,4) * SCALE + QPoint(10,20 * 0),"!!!fail!!!");
        painter.drawText(QPoint(COL,4) * SCALE + QPoint(10,20 * 1),"!!!死因：撞到了自己!!!");
    }
    else if (failFlag == HIT_THE_WALL) {
        painter.drawText(QPoint(COL,4) * SCALE + QPoint(10,20 * 0),"!!!fail!!!");
        painter.drawText(QPoint(COL,4) * SCALE + QPoint(10,20 * 1),"!!!死因：撞到了墙壁!!!");
    }
    str = "speed["+QString::number(delayLevelIdx) + "]=" + QString::number(delayLevel[delayLevelIdx]);
    painter.drawText(QPoint(COL,5) * SCALE + QPoint(10, 20 * 0),str);
    str = QString("wsad控制方向,\n");
    painter.drawText(QPoint(COL,5) * SCALE + QPoint(10, 20 * 1),str);
    str = QString("q减速，e减速，共5档，默认2");
    painter.drawText(QPoint(COL,5) * SCALE + QPoint(10, 20 * 2),str);
    str = QString("r复位，\n");
    painter.drawText(QPoint(COL,5) * SCALE + QPoint(10, 20 * 3),str);
    str = QString("z暂停，方向键继续行动,\n");
    painter.drawText(QPoint(COL,5) * SCALE + QPoint(10, 20 * 4),str);
    str = QString("x切换为自动模式，\n");
    painter.drawText(QPoint(COL,5) * SCALE + QPoint(10, 20 * 5),str);
    str = QString("c切换为无敌模式，再按一下退出，无敌时可穿墙，颜色为红色\n");
    painter.drawText(QPoint(COL,5) * SCALE + QPoint(10, 20 * 5),str);
}
