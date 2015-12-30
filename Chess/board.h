#ifndef BOARD_H
#define BOARD_H

#include <QPainter>
#include <vector>
using namespace std;

class Board
{
private:
    int rectPosX, rectPosY, size, number;
    bool selected, highlighted, containsPiece, pieceColor;

public:
    Board();
    Board(int x, int y, int size, int number, bool piece);

//    void drawBoard(QPainter &paint);
    void initBoard();
    void setPos(int x, int y);
    void setSize(int size);
    void setNumber(int number);
    void select();
    void highlight();
    void setPiece();
    void setPieceColor(bool pColor);

    bool hasPiece();
    bool intersects(QPointF pt);
    bool isSelected();
    bool isHighlighted();
    bool getPieceColor();

    int getRectX();
    int getRectY();
    int getSize();
    int getNumber();
};

#endif // BOARD_H
