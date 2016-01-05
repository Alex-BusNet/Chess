#include "pieces.h"
#include "qdebug.h"

Pieces::Pieces()
{
    posX = 0;
    posY = 0;
    rangeX = 0;
    rangeY = 1;
    type = 'P';
    width = 80;
    height = 100;
    squareLoc = 0; //Stores which square the piece is currently standing on.
    active = false;
    color = 'b';
    image = new QPixmap("../Chess/Images/Black/BlackPawn.png");
}

Pieces::Pieces(int x, int y, char type, int squareLoc, char pColor)
{
    this->setPos(x, y);
    this->setType(type);
    this->setRange();
    this->setLoc(squareLoc);
    this->setImage(type, pColor);
    this->setSize(width, height);
    this->color = pColor;
}

Pieces::Pieces(int x, int y, char type, int squareLoc, char pColor, int w, int h)
{
    this->setPos(x, y);
    this->setType(type);
    this->setRange();
    this->setLoc(squareLoc);
    this->setImage(type, pColor);
    this->setSize(w, h);
    this->color = pColor;
}

Pieces::~Pieces()
{
    delete image;
}

void Pieces::setPos(int x, int y)
{
    this->posX = x;
    this->posY = y;
}

void Pieces::setRange()
{
    setRangeY(type);
    setRangeX(type);
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

void Pieces::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

void Pieces::setImage(char type, char color)
{
    QString imageLoc = "../Chess/Images/%1/%1%2_OLD.png";
    QString pieceType = "Pawn"; ///Default

    switch(type)
    {
        case PAWN:
            pieceType = "Pawn";
            break;
        case ROOK:
            pieceType = "Rook";
            break;
        case KNIGHT:
            pieceType = "Knight";
            break;
        case BISHOP:
            pieceType = "Bishop";
            break;
        case QUEEN:
            pieceType = "Queen";
            break;
        case KING:
            pieceType = "King";
            break;
    }

    if(color == 'r')
    {
        image = new QPixmap(imageLoc.arg("Red").arg(pieceType));
    }
    else
    {
        image = new QPixmap(imageLoc.arg("Black").arg(pieceType));
    }
}

void Pieces::setImage(char type, char color, int width, int height)
{
    this->setSize(width, height);
    this->setImage(type, color);
}

void Pieces::drawPiece(QPainter &paint, int w, int h)
{
    if(this->isActive())
    {
        paint.drawPixmap(this->posX, this->posY, w, h, *image);
    }
}

void Pieces::drawPiece(QPainter &paint)
{
    if(this->isActive())
    {
        paint.drawPixmap(this->posX, this->posY, width, height, *image);
    }
}

void Pieces::setRangeY(char type)
{
    if(type == PAWN)
        rangeY = 1;//The Pawns unique starting condition is handled in showMoves()
    else if(type == ROOK)
        rangeY = 7;
    else if(type == KNIGHT)
        rangeY = 0;//Due to their unique movement pattern, Knight pieces are given a range of zero, however their range of movement is defined in showMoves()
    else if(type == BISHOP)
        rangeY = 7;
    else if(type == QUEEN)
        rangeY = 7;
    else if(type == KING)
        rangeY = 1;
    else
        rangeY = 1;
}

void Pieces::setRangeX(char type)
{
    if(type == PAWN)
        rangeX = 0; //Since Pawns attack at a diagonal, the x-axis parameter is compensated for in showMoves()
    else if(type == ROOK)
        rangeX = 7;
    else if(type == KNIGHT)
        rangeX = 0; //Due to their unique movement pattern, Knight pieces are given a range of zero, however their range of movement is defined in showMoves()
    else if(type == BISHOP)
        rangeX = 0; //Bishops only move in diagonals thus their range is handled by rangeY
    else if(type == QUEEN)
        rangeX = 7;
    else if(type == KING)
        rangeX = 1;
    else
        rangeX = 0;
}

int Pieces::getPosX()
{
    return this->posX;
}

int Pieces::getPosY()
{
    return this->posY;
}

int Pieces::getRangeX()
{
    return this->rangeX;
}

int Pieces::getRangeY()
{
    return this->rangeY;
}

int Pieces::getLoc()
{
    return this->squareLoc;
}

int Pieces::getImageWidth()
{
    return this->width;
}

int Pieces::getImageHeight()
{
    return this->height;
}

bool Pieces::isActive()
{
    return this->active;
}

char Pieces::getType()
{
    return this->type;
}

char Pieces::getColor()
{
    return this->color;
}
