#include "board.h"

Board::Board()
{

}

void Board::drawBoard(QPainter &paint)
{
    paint.drawRect(rectPosX, rectPosY, 8, 8);
}




