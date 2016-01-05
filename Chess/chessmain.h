#ifndef CHESSMAIN_H
#define CHESSMAIN_H

#include <QPainter>
#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>
#include "board.h"
#include "pieces.h"

using namespace std;

class ChessMain : public QWidget
{
    Q_OBJECT

private:
    int BOARD_WIDTH, BOARD_HEIGHT, rectPosX, rectPosY, size, lastPieceSelected;
    bool pieceSelected, opponentHighlighted;
    enum turn {RED = 'r', BLACK = 'b', EMPTY = 'e'};
    char currentTurn;

    QTimer *timer;
    QPushButton *endTurn, *endGame;

public:
    explicit ChessMain(QWidget* parent = 0);
    ~ChessMain();

    QVector<Pieces*> red;
    QVector<Pieces*> black;
    QVector<Board*> board;

    void paintEvent(QPaintEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void showMoves(int location);
    void makeMove(int oldLoc, int newLoc);
    void attack(int attackingLoc, int targetLoc);
    void gameOver();

    void initBoard();

private slots:
    void updateGame();
    void nextTurn();
};

#endif // CHESSMAIN_H
