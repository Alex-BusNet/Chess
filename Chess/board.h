#ifndef BOARD_H
#define BOARD_H

#include <QPainter>
#include <vector>
using namespace std;

class Board
{
private:
    int rectPosX, rectPosY, size, number, indexValue;
    enum PieceTypes{PAWN = 'P', ROOK = 'R', KNIGHT = 'N', BISHOP = 'B', QUEEN = 'Q', KING = 'K'};
    bool selected, highlighted, containsPiece, pieceColor;

public:
    Board();
    Board(int x, int y, int size, int number, int indexValue, bool piece);

//    void drawBoard(QPainter &paint);
    void initBoard();
    void setPos(int x, int y);
    void setSize(int size);
    void setNumber(int number);
    void setIndex(int index);
    void select();
    void highlight();
    void setPiece();
    void drawMoves(char type);
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
    int getIndex();
};

#endif // BOARD_H
