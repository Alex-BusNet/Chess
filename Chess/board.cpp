#include "board.h"

Board::Board()
{
    size = 32; //px

    //This is used to set a label for each box on the board so that it can
    //be used to mark the location of a piece that may occupy that square.
    //Default = -1, no value assigned
    number = -1;

    rectPosX = 0;
    rectPosY = 0;
}

Board::Board(int x, int y, int size, int number)
{
    this->setNumber(number);
    this->rectPosX = x;
    this->rectPosY = y;
    this->setSize(size);
    QRect* square = new QRect(x, y, size, size);
}

void Board::setSize(int size)
{
    this->size = size;
}

void Board::setNumber(int number)
{
    this->number = number;
}

int Board::getRectX()
{
    return this->rectPosX;
}

int Board::getRectY()
{
    return this->rectPosY;
}

int Board::getSize()
{
    return this->size;
}

int Board::getNumber()
{
    return this->number;
}
