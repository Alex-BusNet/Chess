#include "chessmain.h"
#include "QDebug"
#include "pieces.h"

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
    size = 50; //px

    //Centers the board depending on the size of each square
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
    br.setColor(Qt::blue);
    painter.setPen(br);

    QRect square;

    for(int i = 0; i < board.size(); i++)
    {
        square = QRect((*(board.at(i))).getRectX(), (*(board.at(i))).getRectY(), size, size);
        painter.drawRect(square);
        painter.drawText((*(board.at(i))).getRectX() + 5, (*(board.at(i))).getRectY() + 45, QString::number(board.at(i)->getNumber()));
    }

    for(int i = 0; i < red.size(); i++)
    {
        br.setColor(Qt::red);
        painter.setPen(br);
        painter.drawText(red.at(i)->getPosX(), red.at(i)->getPosY(), QString(red.at(i)->getType()));
    }

    for(int i = 0; i < black.size(); i++)
    {
        br.setColor(Qt::black);
        painter.setPen(br);
        painter.drawText(black.at(i)->getPosX(), black.at(i)->getPosY(), QString(black.at(i)->getType()));
    }
}//end of paint event

void ChessMain::initBoard()
{
    int count = 0;

    for(int i = 0; i < BOARD_WIDTH; i++)
    {
        for(int j = 0; j < BOARD_HEIGHT; j++)
        {
            Board* bsqr = new Board(rectPosX, rectPosY, size, count);

            board.push_back(bsqr);

            rectPosY += size + 1;
            count += 8;
        }

        rectPosX += size + 1;
        rectPosY = this->height() / 2 - (size * 4);
        count -= 63;
    }

    count = 0;

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
            char type = StartingPlacement[i][j];
            Pieces* pc = new Pieces(board.at(count)->getRectX() + 22, board.at(count)->getRectY() + 22, 1, type, board.at(count)->getNumber());
            red.push_back(pc);
            count += 8;
        }

        count -= 63;
    }

    count = 63;

    for(int i = 0; i < BOARD_WIDTH; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
            char type = StartingPlacement[i][j];
            Pieces* pc = new Pieces(board.at(count)->getRectX() + 22, board.at(count)->getRectY() + 22, 1, type, board.at(count)->getNumber());
            black.push_back(pc);
            count -= 8;
        }

        count += 63;
    }


}//end of initBoard

void ChessMain::updateGame()
{
    this->update();
}//end of updateGame

