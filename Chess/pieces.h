#ifndef PIECES_H
#define PIECES_H

#include <QObject>
#include <iostream>
#include <QString>
#include <QPixmap>
#include <qpainter.h>
using namespace std;

class Pieces
{
private:
    int posX, posY, rangeY, rangeX, squareLoc, width, height;
    char type, color;
    bool active;
    QPixmap *image;

    enum PieceTypes{PAWN = 'P', ROOK = 'R', KNIGHT = 'N', BISHOP = 'B', QUEEN = 'Q', KING = 'K'};

    void setRangeY(char type);
    void setRangeX(char type);

public:
    Pieces();
    Pieces(int x, int y, char type, int squareLoc, char pColor);
    Pieces(int x, int y, char type, int squareLoc, char color, int w, int h);
    Pieces(int x, int y, char type, int squareLoc, char pColor, int w, int h, bool active);
    ~Pieces();

    void setPos(int x, int y);
    void setRange();
    void setType(char type);
    void setLoc(int squareLoc);
    void setActive();
    void setSize(int width, int height);
    void setImage(char type, char color);
    void setImage(char type, char color, int width, int height);

    void drawPiece(QPainter &paint, int width, int h);
    void drawPiece(QPainter &paint, bool devOpts);

    int getPosX();
    int getPosY();
    int getRangeX();
    int getRangeY();
    int getLoc();
    int getImageWidth();
    int getImageHeight();

    bool isActive();

    char getType();
    char getColor();

    QPixmap &getImage();
};

#endif // PIECES_H
