#include "pieces.h"
#include "qdebug.h"

Pieces::Pieces()
{
    posX = 0;
    posY = 0;
    range = 1;
    type = 'P';
    width = 80;
    height = 100;
    squareLoc = 0; //Stores which square the piece is currently standing on.
    active = true;
    image = new QPixmap("../Chess/Images/Black/BlackPawn.png");
}

Pieces::Pieces(int x, int y, int range, char type, int squareLoc, bool color)
{
    this->setPos(x, y);
    this->setRange(range);
    this->setType(type);
    this->setLoc(squareLoc);
    this->setImage(type, color);
    this->setSize(width, height);
}

Pieces::Pieces(int x, int y, int range, char type, int squareLoc, bool color, int w, int h)
{
    this->setPos(x, y);
    this->setRange(range);
    this->setType(type);
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

void Pieces::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

void Pieces::setImage(char type, bool color)
{
    QString imageLoc = "../Chess/Images/%1/%1%2_OLD.png";
    QString pieceType = "Pawn"; //Default

    //Color = 0: Black
    //Color = 1: Red

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

    if(color)
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

void Pieces::setImage(char type, bool color, int width, int height)
{
    this->setSize(width, height);
    this->setImage(type, color);
}

void Pieces::drawPiece(QPainter &paint, int w, int h)
{
    paint.drawPixmap(this->posX, this->posY, w, h, *image);
}

void Pieces::drawPiece(QPainter &paint)
{
    paint.drawPixmap(this->posX, this->posY, width, height, *image);
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
