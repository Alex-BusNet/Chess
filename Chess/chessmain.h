#ifndef CHESSMAIN_H
#define CHESSMAIN_H

#include <QWidget>

class ChessMain
{
private:
    int BOARD_WIDTH, BOARDHEIGHT;
    char StartingPlacement[8][2];
public:
    ChessMain();
    ~ChessMain();
};

#endif // CHESSMAIN_H
