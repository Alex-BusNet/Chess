#include "chessmain.h"
#include "QDebug"
using namespace std;

char StartingPlacement[2][8] =
{
    {'R','N','B','K','Q','B','N','R'},
    {'P','P','P','P','P','P','P','P'}
};

ChessMain::ChessMain(QWidget *parent) : QWidget(parent)
{
    initBoard();

}//end of c'tor

ChessMain::~ChessMain()
{

}//end of dec'tor

void ChessMain::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    for(unsigned int i = 0; i < board.size(); i++)
    {
        (*(board.at(i))).drawBoard(painter);
    }

}//end of paint event



