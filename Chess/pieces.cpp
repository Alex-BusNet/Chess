#include "pieces.h"

Pieces::Pieces()
{
    posX = 0;
    posY = 0;
    rangeAttack = 1;
    rangeMove = 1;
}

Pieces::~Pieces()
{

}

void Pieces::setPos(int x, int y)
{
    this->posX = x;
    this->posY = y;
}

int Pieces::getATK()
{
    return this->rangeAttack;
}

int Pieces::getMove()
{
    return this->rangeMove;
}

int Pieces::getPosX()
{
    return this->posX;
}

int Pieces::getPosY()
{
   return this->posY;
}

