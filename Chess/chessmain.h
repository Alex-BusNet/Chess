#ifndef CHESSMAIN_H
#define CHESSMAIN_H

#include <QPainter>
#include <QWidget>
#include <QVector>
#include "board.h"
#include "pieces.h"

using namespace std;

class ChessMain : public QWidget
{
    Q_OBJECT

private:


public:
    explicit ChessMain(QWidget* parent = 0);
    ~ChessMain();

    QVector<Pieces*> red;
    QVector<Pieces*> black;
    QVector<Board*> board;

    void paintEvent(QPaintEvent *e);


    void initBoard();

};

#endif // CHESSMAIN_H
