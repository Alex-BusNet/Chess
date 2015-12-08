#include "board.h"

Board::Board()
{
    BOARD_HEIGHT = 8;
    BOARD_WIDTH = 8;

    rectPosX = this->width() / 4;
    rectPosY = this->height() / 4;
}

void Board::drawBoard(QPainter &paint)
{
    paint.drawRect(rectPosX, rectPosY, 8, 8);
}

void Board::setPos(int x, int y)
{
    this->rectPosX = x;
    this->rectPosY = y;
}

int Board::getRectX()
{
    return rectPosX;
}

int Board::getRectY()
{
    return rectPosY;
}

void Board::initBoard()
{
    for(int i = 0; i < BOARD_WIDTH; i++)
    {
        for(int j = 0; j < BOARD_HEIGHT; i++)
        {
            QRect* square = new QRect(rectPosX, rectPosY, 8, 8);

            board.push_back(square);

            rectPosY += 9;
        }

        rectPosX += 9;
    }
}//end of initBoard
