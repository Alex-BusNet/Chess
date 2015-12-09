#ifndef BOARD_H
#define BOARD_H

#include <QPainter>
#include <vector>
using namespace std;

class Board
{
private:
    int rectPosX, rectPosY, size, number;

public:
    Board();
    Board(int x, int y, int size, int number);

//    void drawBoard(QPainter &paint);
    void initBoard();
    void setPos(int x, int y);
    void setSize(int size);
    void setNumber(int number);

    int getRectX();
    int getRectY();
    int getSize();
    int getNumber();
};

#endif // BOARD_H
