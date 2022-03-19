#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#define COL 20 // 列，常用于y
#define ROW 20 // 行，常用于x
#define SNAKE_WIDTH 18 // 蛇的半径，一般比比例放大倍数略小
#define FOOD_WIDTH 14 // 食物半径，比比例放大倍数略小（小于蛇半径，更易区分）
#define SCALE 20 // 比例放大倍数
#define DELAY_LEVEL 8 // 速度等级，如果需要更加细分的速度，增大此值，并在cpp文件内细分
#define DEFAULT_DELAY_LEVEL 2 // 默认速度等级
#define EAT_YOURSELF    1 // fail标志位，吃自己
#define HIT_THE_WALL    2 // fail标志位，撞墙




class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void GetDir();
    void SnakeRun();
    void CreateFood();
    void Init();
    void CheckWin();
    void CheckFail();
    char snakeOccupy[COL][ROW];
    QPoint snakeBody[COL * ROW * 10];
    QPoint snakeDir;
    QPoint keyDir;
    QPoint food;
    int snakeLen;
    QTimer runTimer;
    QTimer updateTimer;
    int grade;
    char winFlag;
    char failFlag;
    char autoFlag;
    char invincibleFlag;

    int delayLevel[DELAY_LEVEL];
    int delayLevelIdx;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
