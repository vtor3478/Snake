#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    snakeBody[0] = QPoint(5,5);
    snakeBody[1] = snakeBody[0] + QPoint(-1,0);
    snakeBody[2] = snakeBody[1] + QPoint(-1,0);
    snakeLen = 3;
    dir = QPoint(1,0);

    runTimer.setInterval(200);
    runTimer.start();
    connect(&runTimer,&QTimer::timeout,[=](){
        SnakeRun();
    });
    updateTimer.setInterval(10);
    updateTimer.start();
    connect(&updateTimer,&QTimer::timeout,[=](){
        update();
    });
    Feed();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::SnakeRun()
{
    for (int i = snakeLen - 1;i > 0; i--)
    {
        snakeBody[i] = snakeBody[i-1];
    }
    snakeBody[0] += dir;
    snakeBody[0].rx() = (snakeBody[0].x() + ROW) % ROW;
    snakeBody[0].ry() = (snakeBody[0].y() + COL) % COL;

    if(food == snakeBody[0])
    {
        ++snakeLen;
        Feed();
    }
}
void Widget::Feed()
{
    food.setX(qrand() % 10);
    food.setY(qrand() % 10);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if("w" == event->text())
    {
        dir = QPoint(0,-1);
    }
    else if("s" == event->text())
    {
        dir = QPoint(0,1);
    }
    else if("a" == event->text())
    {
        dir = QPoint(-1,0);
    }
    else if("d" == event->text())
    {
        dir = QPoint(1,0);
    }
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawLine(100,100,200,100);
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
}
