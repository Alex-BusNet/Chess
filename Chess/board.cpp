#include "board.h"
#include <QDebug>

Board::Board()
{
    size = 32; //px

    //This is used to set a label for each box on the board so that it can
    //be used to mark the location of a piece that may occupy that square.
    //Default = -1, no value assigned
    number = -1;

    rectPosX = 0;
    rectPosY = 0;
    selected = false;
    highlighted = false;
    containsPiece = false;
}

Board::Board(int x, int y, int size, int number, bool piece)
{
    this->setNumber(number);
    this->rectPosX = x;
    this->rectPosY = y;
    this->setSize(size);
    selected = false;
    highlighted = false;
    containsPiece = piece;
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

void Board::select()
{
    this->selected = !selected;
}

void Board::highlight()
{
    this->highlighted = !highlighted;
}

bool Board::hasPiece()
{
    return containsPiece;
}

void Board::setPiece()
{
    this->containsPiece = !containsPiece;
}

void Board::setPieceColor(bool pColor)
{
    this->pieceColor = pColor;
}

bool Board::intersects(QPointF pt)
{
    if(this->rectPosX < pt.x() && this->rectPosX + size > pt.x())
    {
        if(this->rectPosY < pt.y() && this->rectPosY + size > pt.y())
        {
            return true;
        }
    }

    return false;
}

bool Board::isSelected()
{
    return selected;
}

bool Board::isHighlighted()
{
    return this->highlighted;
}

bool Board::getPieceColor()
{
    return this->pieceColor;
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
