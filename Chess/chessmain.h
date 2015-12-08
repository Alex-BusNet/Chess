#ifndef CHESSMAIN_H
#define CHESSMAIN_H

#include <QPainter>
#include <QWidget>
#include <QVector>
#include <QTimer>
#include "pieces.h"

using namespace std;

class ChessMain : public QWidget
{
    Q_OBJECT

private:
    int BOARD_WIDTH, BOARD_HEIGHT, rectPosX, rectPosY, size;
    QTimer *timer;

public:
    explicit ChessMain(QWidget* parent = 0);
    ~ChessMain();

    QVector<Pieces*> red;
    QVector<Pieces*> black;
    QVector<QRect*> board;

    void paintEvent(QPaintEvent *e);

    void initBoard();
    void setPos(int x, int y);
    int getRectX();
    int getRectY();

private slots:
    void updateGame();
};

#endif // CHESSMAIN_H
