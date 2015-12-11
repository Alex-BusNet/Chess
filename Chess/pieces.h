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
    int posX, posY, range, squareLoc, width, height;
    char type;
    bool active;
    QPixmap *image;

    enum PieceTypes{PAWN = 'P', ROOK = 'R', KNIGHT = 'N', BISHOP = 'B', QUEEN = 'Q', KING = 'K'};

public:
    Pieces();
    Pieces(int x, int y, int range, char type, int squareLoc, bool color);
    Pieces(int x, int y, int range, char type, int squareLoc, bool color, int w, int h);
    ~Pieces();

    void setPos(int x, int y);
    void setRange(int range);
    void setType(char type);
    void setLoc(int squareLoc);
    void setActive();
    void setSize(int width, int height);
    void setImage(char type, bool color);
    void setImage(char type, bool color, int width, int height);

    void drawPiece(QPainter &paint, int width, int h);
    void drawPiece(QPainter &paint);

    int getRange();
    int getPosX();
    int getPosY();
    int getLoc();
    int getImageWidth();
    int getImageHeight();
    bool isActive();

    char getType();

    QPixmap &getImage();
};

#endif // PIECES_H
