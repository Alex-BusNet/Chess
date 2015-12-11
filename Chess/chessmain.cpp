#include "chessmain.h"
#include "QDebug"
#include "pieces.h"

using namespace std;

const char StartingPlacement[2][8] =
{
    {'R','N','B','K','Q','B','N','R'},
    {'P','P','P','P','P','P','P','P'}
};

ChessMain::ChessMain(QWidget *parent) : QWidget(parent)
{
    QWidget::setFixedSize(900, 900);

    BOARD_HEIGHT = 8; //Squares
    BOARD_WIDTH = 8; //Squares
    size = 105; //px

    //Centers the board depending on the size of each square
    rectPosX = this->width() / 2 - (size * 4);
    rectPosY = this->height() / 2 - (size * 4);

    qDebug() << "init board";
    initBoard();
    qDebug() << "init complete";
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

    QRect square;
    QPen pen;

    for(int i = 0; i < board.size(); i++)
    {
        square = QRect((*(board.at(i))).getRectX(), (*(board.at(i))).getRectY(), size, size);

        if((*(board.at(i))).isSelected())
        {
            pen.setColor(Qt::blue);
            pen.setWidth(5);
        }
        else
        {
            pen.setColor(Qt::black);
            pen.setWidth(1);
        }

        painter.setPen(pen);
        painter.drawRect(square);
        painter.drawText((*(board.at(i))).getRectX() + 5, (*(board.at(i))).getRectY() + 90, QString::number(board.at(i)->getNumber()));
    }

    for(int i = 0; i < red.size(); i++)
    {
        (*(red.at(i))).drawPiece(painter/*, (*(red.at(i))).getImageWidth(), (*(red.at(i))).getImageHeight()*/);
    }

    for(int i = 0; i < black.size(); i++)
    {
        (*(black.at(i))).drawPiece(painter/*, (*(black.at(i))).getImageWidth(), (*(black.at(i))).getImageHeight()*/);
    }
}//end of paint event

void ChessMain::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug() << "MouseReleaseEvent" << e->button();
    if(e->button() == 1)
    {
        QPointF pt = e->localPos();
        qDebug() << e->localPos();

        for(int i = 0; i < board.size(); i++)
        {
            if((*(board.at(i))).intersects(pt))
            {
                qDebug() << "Square" <<(*(board.at(i))).getNumber() << "selected.";
                (*(board.at(i))).select();
                break;
            }
        }
    }
}//end of mouseReleaseEvent

void ChessMain::initBoard()
{
    int count = 0;

    qDebug() << "Generating Board";
    for(int i = 0; i < BOARD_HEIGHT; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
            Board* bsqr = new Board(rectPosX, rectPosY, size, count);

            board.push_back(bsqr);

            rectPosX += size + 1;
            count++;
        }

        rectPosY += size + 1;
        rectPosX = this->height() / 2 - (size * 4);
    }

    count = 0;

    qDebug() << "Generating Red Pieces";

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < BOARD_HEIGHT; j++)
        {
            char type = StartingPlacement[i][j];
            Pieces* pc = new Pieces((*(board.at(count))).getRectX() + 15, (*(board.at(count))).getRectY() + 5, 1, type, (*(board.at(count))).getNumber(), 1, 80, 100);
            red.push_back(pc);
            count++;
        }

        qDebug() << count;
    }

    count = 63;

    qDebug() << "Generating Black Pieces";

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
            char type = StartingPlacement[i][j];
            Pieces* pc = new Pieces(board.at(count)->getRectX() + 15, board.at(count)->getRectY() + 5, 1, type, board.at(count)->getNumber(), 0, 80, 100);
            black.push_back(pc);
            count--;
        }
    }


}//end of initBoard

void ChessMain::updateGame()
{
    this->update();
}//end of updateGame

