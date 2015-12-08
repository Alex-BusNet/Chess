#ifndef PIECES_H
#define PIECES_H

#include <QObject>
#include <iostream>

class Pieces
{
private:
    int posX, posY;

public:
    Pieces();
    ~Pieces();

    void setPos(int x, int y);
    int getPosX();
    int getPosY();
};

#endif // PIECES_H
