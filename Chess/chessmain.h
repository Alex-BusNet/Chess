#ifndef CHESSMAIN_H
#define CHESSMAIN_H

#include <QPainter>
#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QMouseEvent>
#include <QPoint>
#include "board.h"
#include "pieces.h"

using namespace std;

class ChessMain : public QWidget
{
    Q_OBJECT

private:
    int BOARD_WIDTH, BOARD_HEIGHT, rectPosX, rectPosY, size, lastPieceSelected;
    bool turn, pieceSelected, opponentHighlighted;
    QTimer *timer;

public:
    explicit ChessMain(QWidget* parent = 0);
    ~ChessMain();

    QVector<Pieces*> red;
    QVector<Pieces*> black;
    QVector<Board*> board;

    void paintEvent(QPaintEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void showMoves(int location);
    void move();

    void initBoard();

private slots:
    void updateGame();
};

#endif // CHESSMAIN_H
