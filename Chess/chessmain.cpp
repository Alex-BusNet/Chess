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

    turn = false; // True = Black, False = Red
    pieceSelected = false;
    lastPieceSelected = 0;

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

        if((*(board.at(i))).isHighlighted())
        {
            painter.fillRect(square, Qt::green);
        }
        else
        {
            painter.fillRect(square, Qt::gray);
        }

        painter.drawRect(square);
        painter.drawText((*(board.at(i))).getRectX() + 5, (*(board.at(i))).getRectY() + 90, QString::number(board.at(i)->getNumber()));
    }

    for(int i = 0; i < red.size(); i++)
    {
        (*(red.at(i))).drawPiece(painter);
    }

    for(int i = 0; i < black.size(); i++)
    {
        (*(black.at(i))).drawPiece(painter);
    }
}//end of paint event

void ChessMain::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == 1)
    {
        QPointF pt = e->localPos();
        qDebug() << "Clicked at point:" << pt;
        for(int i = 0; i < board.size(); i++)
        {
            if((*(board.at(i))).intersects(pt))
            {
                qDebug() << "Point intersects square:" << (*(board.at(i))).getNumber();

                if((*(board.at(i))).hasPiece())
                {
                    qDebug() << "Square:" << (*(board.at(i))).getNumber() << "Contains piece";

                    if((*(board.at(i))).getPieceColor() == turn)
                    {
                        if(pieceSelected)
                        {
                            for(int j = 0; j < board.size(); j++)
                            {
                                if((*(board.at(j))).isSelected())
                                {
                                    showMoves((*(board.at(j))).getNumber());
                                    (*(board.at(j))).select();
                                    pieceSelected = false; //Reset the piece selected flag
                                    break;
                                }
                            }
                        }

                        if((*(board.at(i))).getNumber() != lastPieceSelected)
                        {
                            (*(board.at(i))).select();
                            pieceSelected = true; //sets the flag indicating there is a piece currently selected

                            qDebug() << "Clicked in square:" << (*(board.at(i))).getNumber() << "Last Piece Selected::" << lastPieceSelected;
                            showMoves((*(board.at(i))).getNumber());
                            lastPieceSelected = (*(board.at(i))).getNumber();
                        }


                        break;
                    }
                }
            }
        }
    }
}//end of mouseReleaseEvent

void ChessMain::showMoves(int location)
{
    int range = 0, index = 0;
    char type = 'P';

    //Gets the Range of the piece inside the selected square
    if((*(board.at(location))).getPieceColor() == 0)
    {
        for(int i = 0; i < black.size(); i++)
        {
            if(location == black.at(i)->getLoc())
            {
                range = black.at(i)->getRange();
                break;
            }
        }
    }
    else if((*(board.at(location))).getPieceColor() == 1)
    {
        for(int i = 0; i < red.size(); i++)
        {
            if(location == red.at(i)->getLoc())
            {
                range =  red.at(i)->getRange();
                break;
            }
        }
    }

    qDebug() << "Piece:" << type << "has range:" << range;
    //Highlights the correct squares corresponding to the range of the selected piece.
    ///Currently only works in the Y-direction. Diagonals and horizontals not supported.
    for(int i = 0; i < range; i++)
    {
        if((*(board.at(location))).getPieceColor() == 0)
        {
            if(!((*(board.at(location - (8 * (i + 1))))).hasPiece()))
            {
                (*(board.at(location - (8 * (i + 1))))).highlight();
            }
            else
            {
                break;
            }
        }
        else if ((*(board.at(location))).getPieceColor() == 1)
        {
            if(!((*(board.at(location + (8 * (i + 1))))).hasPiece()))
            {
                (*(board.at(location + (8 * (i + 1))))).highlight();
            }
            else
            {
                break;
            }
        }
    }
}//end of showMoves

void ChessMain::initBoard()
{
    int count = 0;
    int indexCount = 0;

    qDebug() << "Generating Board";
    for(int i = 0; i < BOARD_HEIGHT; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
            Board* bsqr = new Board(rectPosX, rectPosY, size, count, indexCount, false);

            board.push_back(bsqr);

            rectPosX += size + 1;
            count++;

            qDebug() << "indexCount:" << indexCount;

            if(indexCount < 16)
                indexCount++;
            else
                indexCount = 0;

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
            pc->setRange(pc->getRangeY(type));
            red.push_back(pc);
            (*(board.at(count))).setPiece();
            (*(board.at(count))).setPieceColor(true);
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
            pc->setRange(pc->getRangeY(type));
            black.push_back(pc);
            (*(board.at(count))).setPiece();
            (*(board.at(i))).setPieceColor(false);
            count--;
        }
    }
}//end of initBoard

void ChessMain::updateGame()
{
    this->update();
}//end of updateGame

