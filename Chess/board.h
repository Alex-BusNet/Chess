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
    bool selected, highlightedMove, highlightedEnemy, containsPiece;
    char pieceColor;

public:
    Board();
    Board(int x, int y, int size, int number, int indexValue, bool piece, char color);

//    void drawBoard(QPainter &paint);
    void initBoard();
    void setPos(int x, int y);
    void setSize(int size);
    void setNumber(int number);
    void setIndex(int index);
    void select();
    void highlightMove();
    void highlightEnemy();
    void setPiece();
    void setPieceColor(char pColor);

    bool hasPiece();
    bool intersects(QPointF pt);
    bool isSelected();
    bool isMoveHighlighted();
    bool isEnemyHighlighted();

    char getPieceColor();

    int getRectX();
    int getRectY();
    int getSize();
    int getNumber();
    int getIndex();
};

#endif // BOARD_H
