#ifndef BOARD_H
#define BOARD_H

#include <QPainter>
#include <vector>
using namespace std;

class Board
{
private:
    int rectPosX, rectPosY, BOARD_HEIGHT, BOARD_WIDTH;

public:
    Board();

    void drawBoard(QPainter &paint);
    void initBoard();
    void setPos(int x, int y);

    int getRectX();
    int getRectY();
};

#endif // BOARD_H
