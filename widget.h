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

#define COL 10
#define ROW 10
#define SNAKE_WIDTH 50
#define FOOD_WIDTH 30
#define SCALE 50
#define DELAY_LEVEL 5
#define DEFAULT_DELAY_LEVEL 2




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

    int delayLevel[DELAY_LEVEL];
    int delayLevelIdx;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
