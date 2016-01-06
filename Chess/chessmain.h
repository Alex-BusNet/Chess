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
    bool pieceSelected, opponentHighlighted, devOpts, promote;
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
    void makeMove(int oldLoc, int newLoc, const QVector<Pieces *> &vec);
    void attack(int atkLoc, int trgLoc, const QVector<Pieces *> &atkVector, const QVector<Pieces *> &trgVector);
    void gameOver();
    void promotion(int loc, const QVector<Pieces*> &vec);

    void initBoard();
    void setDevOpts(bool show);


private slots:
    void updateGame();
    void nextTurn();
};

#endif // CHESSMAIN_H
