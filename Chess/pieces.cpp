#include "pieces.h"

Pieces::Pieces()
{
    posX = 0;
    posY = 0;
    range = 1;
    type = 'D';
    squareLoc = 0; //Stores which square the piece is currently standing on.
    active = true;
}

Pieces::Pieces(int x, int y, int range, char type, int squareLoc)
{
    this->setPos(x, y);
    this->setRange(range);
    this->setType(type);
    this->setLoc(squareLoc);
}

Pieces::~Pieces()
{

}

void Pieces::setPos(int x, int y)
{
    this->posX = x;
    this->posY = y;
}

void Pieces::setRange(int range)
{
    this->range = range;
}

void Pieces::setType(char type)
{
    this->type = type;
}

void Pieces::setLoc(int squareLoc)
{
    this->squareLoc = squareLoc;
}

void Pieces::setActive()
{
    this->active = !active;
}

int Pieces::getRange()
{
    return this->range;
}

int Pieces::getPosX()
{
    return this->posX;
}

int Pieces::getPosY()
{
    return this->posY;
}

int Pieces::getLoc()
{
    return this->squareLoc;
}

bool Pieces::isActive()
{
    return active;
}

char Pieces::getType()
{
    return this->type;
}

