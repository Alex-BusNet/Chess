#include "pieces.h"
#include "qdebug.h"

Pieces::Pieces()
{
    posX = 0;
    posY = 0;
    rangeX = 1;
    rangeY = 1;
    type = 'P';
    width = 80;
    height = 100;
    squareLoc = 0; //Stores which square the piece is currently standing on.
    active = true;
    color = false; //False = Black, True = Red
    image = new QPixmap("../Chess/Images/Black/BlackPawn.png");
}

Pieces::Pieces(int x, int y, char type, int squareLoc, char color)
{
    this->setPos(x, y);
    this->setRange();
    this->setType(type);
    this->setLoc(squareLoc);
    this->setImage(type, color);
    this->setSize(width, height);
    this->color = color;
}

Pieces::Pieces(int x, int y, char type, int squareLoc, char color, int w, int h)
{
    this->setPos(x, y);
    this->setType(type);
    this->setRange();
    this->setLoc(squareLoc);
    this->setImage(type, color);
    this->setSize(w, h);
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
//        qDebug() << "imageLoc:" << imageLoc.arg("Red").arg(pieceType);
        image = new QPixmap(imageLoc.arg("Red").arg(pieceType));
    }
    else
    {
//        qDebug() << "imageLoc:" << imageLoc.arg("Black").arg(pieceType);
        image = new QPixmap(imageLoc.arg("Black").arg(pieceType));
    }
}

void Pieces::setImage(char type, char color, int width, int height)
{
    this->setSize(width, height);
    this->setImage(type, color);
}

void Pieces::showMoves()
{

}

void Pieces::drawPiece(QPainter &paint, int w, int h)
{
    paint.drawPixmap(this->posX, this->posY, w, h, *image);
}

void Pieces::drawPiece(QPainter &paint)
{
    paint.drawPixmap(this->posX, this->posY, width, height, *image);
}

void Pieces::setRangeY(char type)
{
    if(type == PAWN)
        rangeY = 1;
    else if(type == ROOK)
        rangeY = 7;
    else if(type == KNIGHT)
        rangeY = 3;
    else if(type == BISHOP)
        rangeY = 1;
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
        rangeX = 1;
    else if(type == ROOK)
        rangeX = 7;
    else if(type == KNIGHT)
        rangeX = 3;
    else if(type == BISHOP)
        rangeX = 1;
    else if(type == QUEEN)
        rangeX = 7;
    else if(type == KING)
        rangeX = 1;
    else
        rangeX = 1;
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
    return active;
}

char Pieces::getType()
{
    return this->type;
}

char Pieces::getColor()
{
    return this->color;
}
