#ifndef PIECES_H
#define PIECES_H

#include <QObject>
#include <iostream>
#include <QString>
using namespace std;

class Pieces
{
private:
    int posX, posY, range, squareLoc;
    char type;
    bool active;

public:
    Pieces();
    Pieces(int x, int y, int range, char type, int squareLoc);
    ~Pieces();

    void setPos(int x, int y);
    void setRange(int range);
    void setType(char type);
    void setLoc(int squareLoc);
    void setActive();

    int getRange();
    int getPosX();
    int getPosY();
    int getLoc();
    bool isActive();

    char getType();
};

#endif // PIECES_H
