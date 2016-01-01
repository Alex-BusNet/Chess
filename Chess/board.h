#ifndef BOARD_H
#define BOARD_H

#include <QPainter>
#include <vector>
using namespace std;

class Board
{
private:
    int rectPosX, rectPosY, size, number;
    bool selected, highlighted, containsPiece;
    enum PieceTypes{PAWN = 'P', ROOK = 'R', KNIGHT = 'N', BISHOP = 'B', QUEEN = 'Q', KING = 'K'};
public:
    Board();
    Board(int x, int y, int size, int number, bool piece);

//    void drawBoard(QPainter &paint);
    void initBoard();
    void setPos(int x, int y);
    void setSize(int size);
    void setNumber(int number);
    void select();
    void setPiece();
    void drawMoves(char type);
    void setHighlight();

    bool hasPiece();
    bool intersects(QPointF pt);
    bool isSelected();
    bool isHighlighted();

    int getRectX();
    int getRectY();
    int getSize();
    int getNumber();
};

#endif // BOARD_H
