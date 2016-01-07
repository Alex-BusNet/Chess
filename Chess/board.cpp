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
    highlightedMove = false;
    highlightedEnemy = false;
    containsPiece = false;
}

Board::Board(int x, int y, int size, int number, bool piece, char color)
{
    this->setNumber(number);
    this->rectPosX = x;
    this->rectPosY = y;
    this->setSize(size);
    selected = false;
    highlightedMove = false;
    highlightedEnemy = false;
    containsPiece = piece;
    this->setPieceColor(color);
}

void Board::setSize(int size)
{
    this->size = size;
}

void Board::setNumber(int number)
{
    this->number = number;
}

void Board::setIndex(int index)
{
    this->indexValue = index;
}

void Board::select()
{
    this->selected = !selected;
}

void Board::highlightMove()
{
    this->highlightedMove = !highlightedMove;
}

void Board::highlightEnemy()
{
    this->highlightedEnemy = !highlightedEnemy;
}

bool Board::hasPiece()
{
    return containsPiece;
}

void Board::setPiece()
{
    this->containsPiece = !containsPiece;
}

void Board::setPieceColor(char pColor)
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
    return this->selected;
}

bool Board::isEnemyHighlighted()
{
    return this->highlightedEnemy;
}

bool Board::isMoveHighlighted()
{
    return this->highlightedMove;
}

char Board::getPieceColor()
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

int Board::getIndex()
{
    return this->indexValue;
}
