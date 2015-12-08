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
    QWidget::setFixedSize(this->width(), this->height());
    BOARD_HEIGHT = 8; //Squares
    BOARD_WIDTH = 8; //Squares
    size = 32; //px
    rectPosX = this->width() / 2 - (size * 4);
    rectPosY = this->height() / 2 - (size * 4);

    initBoard();

    timer = new QTimer();
    timer->setInterval(50);
    timer->start();

    connect(timer, SIGNAL(timeout()), this, SLOT(updateGame()));

}//end of c'tor

ChessMain::~ChessMain()
{
    delete timer;

    for(int i = 0; i < board.size(); i++)
    {
        delete board.at(i);
    }


    for(int i = 0; i < red.size(); i++)
    {
        delete red.at(i);
    }

    for(int i = 0; i < black.size(); i++)
    {
        delete black.at(i);
    }

}//end of dec'tor

void ChessMain::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen br;
    br.setColor(Qt::red);
    painter.setPen(br);

    QRect square;

    for(int i = 0; i < board.size(); i++)
    {
        square = QRect((*(board.at(i))).x(), (*(board.at(i))).y(), size, size);
        painter.drawRect(square);
    }


}//end of paint event

void ChessMain::initBoard()
{
    for(int i = 0; i < BOARD_WIDTH; i++)
    {
        for(int j = 0; j < BOARD_HEIGHT; j++)
        {
            QRect* square = new QRect(rectPosX, rectPosY, size, size);

            board.push_back(square);

            rectPosY += size + 1;
        }

        rectPosX += size + 1;
        rectPosY = this->height() / 2 - (size * 4);
    }

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; i++)
        {

        }
    }
}//end of initBoard

void ChessMain::setPos(int x, int y)
{
    this->rectPosX = x;
    this->rectPosY = y;
}//end of setPos

int ChessMain::getRectX()
{
    return rectPosX;
}//end of getRectX

int ChessMain::getRectY()
{
    return rectPosY;
}//end of getRectY

void ChessMain::updateGame()
{
    this->update();
}//end of updateGame

