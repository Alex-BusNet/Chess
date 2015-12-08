#ifndef PIECES_H
#define PIECES_H

#include <QObject>
#include <iostream>

class Pieces
{
private:
    int posX, posY, rangeAttack, rangeMove;

public:
    Pieces();
    ~Pieces();

    void setPos(int x, int y);
    void setRange(int attack, int move);
    int getATK();
    int getMove();
    int getPosX();
    int getPosY();
};

#endif // PIECES_H
