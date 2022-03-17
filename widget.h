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




class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void SnakeRun();
    void Feed();
    QPoint snakeBody[COL * ROW];
    QPoint dir;
    QPoint food;
    int snakeLen;
    QTimer runTimer;
    QTimer updateTimer;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
