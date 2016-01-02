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
    QWidget::setFixedSize(1200, 900);

    BOARD_HEIGHT = 8; //Squares
    BOARD_WIDTH = 8; //Squares
    size = 105; //px

    //Centers the board depending on the size of each square
    rectPosX = 450 - (size * 4);
    rectPosY = this->height() / 2 - (size * 4);

    qDebug() << "init board";
    initBoard();
    qDebug() << "init complete";

    currentTurn = BLACK;

    pieceSelected = false;
    lastPieceSelected = 0;

    endTurn = new QPushButton("End Turn", this);
    endTurn->move(1000, 500);

    endGame = new QPushButton("End Game", this);
    endGame->move(1000, 550);

    connect(endTurn, SIGNAL(clicked(bool)), this, SLOT(nextTurn()));
    connect(endGame, SIGNAL(clicked(bool)), this, SLOT(close()));

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

        if((*(board.at(i))).isMoveHighlighted())
        {
            painter.fillRect(square, Qt::green);
        }
        else if((*(board.at(i))).isEnemyHighlighted())
        {
            painter.fillRect(square, Qt::red);
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

    QFont* GameInfoFont = new QFont();
    GameInfoFont->setPointSize(24);
    painter.setFont(*GameInfoFont);
    painter.drawText(950, 80, "Turn:");

    if(currentTurn == BLACK)
        painter.drawText(950, 110, "Black");
    else
        painter.drawText(950, 110, "Red");


}//end of paint event

void ChessMain::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == 1)
    {
        QPointF pt = e->localPos();

        for(int i = 0; i < board.size(); i++)
        {
            if((*(board.at(i))).intersects(pt))
            {
                if((*(board.at(i))).hasPiece())
                {
                    if((*(board.at(i))).getPieceColor() == currentTurn)
                    {
                        if(pieceSelected)
                        {
                            for(int j = 0; j < board.size(); j++)
                            {
                                if((*(board.at(j))).isSelected())
                                {
                                    showMoves((*(board.at(j))).getNumber());
                                    qDebug() <<"Deselecting Piece";
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
    int range = 0;
    char type = 'p';

    //Gets the Range of the piece inside the selected square
    if((*(board.at(location))).getPieceColor() == BLACK)
    {
        for(int i = 0; i < black.size(); i++)
        {
            if(location == black.at(i)->getLoc())
            {
                range = black.at(i)->getRangeY();
                type = black.at(i)->getType();
                break;
            }
        }
    }
    else if((*(board.at(location))).getPieceColor() == RED)
    {
        for(int i = 0; i < red.size(); i++)
        {
            if(location == red.at(i)->getLoc())
            {
                range =  red.at(i)->getRangeY();
                type = red.at(i)->getType();
                break;
            }
        }
    }

    //Highlights the correct squares corresponding to the range of the selected piece.
    ///Currently only works in the Y-direction. Diagonals and horizontals not supported.
    for(int i = 0; i < range; i++)
    {
        if((*(board.at(location))).getPieceColor() == currentTurn)
        {
            if(type == 'N')
            {
                if(location - 5 >= 0) //Towards Red team
                {
                    qDebug() << "location % 8 =" << location % 8;

                    if(location % 8 < 3) //Move selection if Knight is near left edge
                    {
                        if(!(*(board.at(location - 5))).hasPiece())
                        {
                            qDebug() <<"Selecting Empty Square";
                            (*(board.at(location - 5))).highlightMove();
                        }
                        else if((*(board.at(location - 5))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                        {
                            qDebug() <<"Enemy Found";
                            (*(board.at(location - 5))).highlightEnemy();
                        }

                        if(location - 23 >= 0)
                        {
                            if(!(*(board.at(location - 23))).hasPiece())
                            {
                                qDebug() <<"Selecting Empty Square";
                                (*(board.at(location - 23))).highlightMove();
                            }
                            else if((*(board.at(location - 23))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                            {
                                qDebug() <<"Enemy Found";
                                (*(board.at(location - 23))).highlightEnemy();
                            }

                            if(location % 8 > 0)
                            {
                                if(location - 25 >= 0)
                            {
                                if(!(*(board.at(location - 25))).hasPiece())
                                {
                                    qDebug() <<"Selecting Empty Square";
                                    (*(board.at(location - 25))).highlightMove();
                                }
                                else if((*(board.at(location - 25))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                                {
                                    qDebug() <<"Enemy Found";
                                    (*(board.at(location - 25))).highlightEnemy();
                                }
                            }
                            }
                        }
                    }
                    else //Procede with normal move selection
                    {
                        if(location % 8 < 4) //Move selection if Knight is not near right edge
                        {
                            if(!(*(board.at(location - 5))).hasPiece())
                            {
                                qDebug() <<"Selecting Empty Square";
                                (*(board.at(location - 5))).highlightMove();
                            }
                            else if((*(board.at(location - 5))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                            {
                                qDebug() <<"Enemy Found";
                                (*(board.at(location - 5))).highlightEnemy();
                            }
                        }

                        if(location - 11 >= 0)
                        {
                            if(!(*(board.at(location - 11))).hasPiece())
                            {
                                qDebug() <<"Selecting Empty Square";
                                (*(board.at(location - 11))).highlightMove();
                            }
                            else if((*(board.at(location - 11))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                            {
                                qDebug() <<"Enemy Found";
                                (*(board.at(location - 11))).highlightEnemy();
                            }

                            if(location - 25 >= 0)
                            {
                                if(!(*(board.at(location - 25))).hasPiece())
                                {
                                    qDebug() <<"Selecting Empty Square";
                                    (*(board.at(location - 25))).highlightMove();
                                }
                                else if((*(board.at(location - 25))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                                {
                                    qDebug() <<"Enemy Found";
                                    (*(board.at(location - 25))).highlightEnemy();
                                }

                                if(location % 8 < 7)
                                {
                                    if(location - 23 >= 0)
                                    {
                                        if(!(*(board.at(location - 23))).hasPiece())
                                        {
                                            qDebug() <<"Selecting Empty Square";
                                            (*(board.at(location - 23))).highlightMove();
                                        }
                                        else if((*(board.at(location - 23))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                                        {
                                            qDebug() <<"Enemy Found";
                                            (*(board.at(location - 23))).highlightEnemy();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if(location + 5 <= 63) //Towards Black team
                {
                    if(location % 8 > 2)
                    {
                        if(!(*(board.at(location + 5))).hasPiece())
                        {
                            qDebug() <<"Selecting Empty Square";
                            (*(board.at(location + 5))).highlightMove();
                        }
                        else if((*(board.at(location + 5))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                        {
                            qDebug() <<"Enemy Found";
                            (*(board.at(location + 5))).highlightEnemy();
                        }
                    }

                    if(location % 8 < 5)
                    {
                        if(location + 11 <= 63)
                        {
                            if(!(*(board.at(location + 11))).hasPiece())
                            {
                                qDebug() <<"Selecting Empty Square";
                                (*(board.at(location + 11))).highlightMove();
                            }
                            else if((*(board.at(location + 11))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                            {
                                qDebug() <<"Enemy Found";
                                (*(board.at(location + 11))).highlightEnemy();
                            }
                        }
                    }

                    if(location % 8 > 0)
                    {
                        if(location + 23 <= 63)
                        {
                            if(!(*(board.at(location + 23))).hasPiece())
                            {
                                qDebug() <<"Selecting Empty Square";
                                (*(board.at(location + 23))).highlightMove();
                            }
                            else if((*(board.at(location + 23))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                            {
                                qDebug() <<"Enemy Found";
                                (*(board.at(location + 23))).highlightEnemy();
                            }
                        }
                    }

                    if(location % 8 < 7)
                    {
                        if(location + 25 <= 63)
                        {
                            if(!(*(board.at(location + 25))).hasPiece())
                            {
                                qDebug() <<"Selecting Empty Square";
                                (*(board.at(location + 25))).highlightMove();
                            }
                            else if((*(board.at(location + 25))).getPieceColor() != currentTurn && ((*(board.at(location - 5))).getPieceColor() != EMPTY))
                            {
                                qDebug() <<"Enemy Found";
                                (*(board.at(location + 25))).highlightEnemy();
                            }
                        }
                    }
                }

                break; //Ends the loop after selecting the appropriate squares for the Knight.
            }

            qDebug() << "Non-Knight Piece";

            if(!((*(board.at(location - (8 * (i + 1))))).hasPiece()))
            {
                qDebug() <<"Selecting Empty Square";
                (*(board.at(location - (8 * (i + 1))))).highlightMove();
            }
            else
            {
                if((*(board.at(location - (8 * (i + 1))))).getPieceColor() != currentTurn && (*(board.at(location - 5))).getPieceColor() != EMPTY)
                {
                    qDebug() <<"Enemy Piece";
                    (*(board.at(location - (8 * (i + 1))))).highlightEnemy();
                    break; //End loop if Enemy is found
                }
                else
                {
                    qDebug() <<"Friendly Piece";
                    break; //End loop if Friendly is found
                }
            }
        }
        else if ((*(board.at(location))).getPieceColor() == RED)
        {
            if(!((*(board.at(location + (8 * (i + 1))))).hasPiece()))
            {
                qDebug() <<"Selecting Empty Square";
                (*(board.at(location + (8 * (i + 1))))).highlightMove();
            }
            else
            {
                if((*(board.at(location + (8 * (i + 1))))).getPieceColor() != currentTurn && (*(board.at(location - 5))).getPieceColor() != EMPTY)
                {
                    qDebug() <<"Enemy Piece";
                    (*(board.at(location + (8 * (i + 1))))).highlightEnemy();
                    break; //End loop if Enemy is found
                }
                else
                {
                    qDebug() <<"Friendly Piece";
                    break; //End loop if Friendly is found
                }
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
            Board* bsqr = new Board(rectPosX, rectPosY, size, count, indexCount, false, EMPTY);

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
        rectPosX = 450 - (size * 4);
    }

    count = 0;

    qDebug() << "Generating Red Pieces";

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < BOARD_HEIGHT; j++)
        {
            char type = StartingPlacement[i][j];
            Pieces* pc = new Pieces((*(board.at(count))).getRectX() + 15, (*(board.at(count))).getRectY() + 5, type, (*(board.at(count))).getNumber(), RED, 80, 100);
            red.push_back(pc);
            (*(board.at(count))).setPiece();
            (*(board.at(count))).setPieceColor(RED);
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
            Pieces* pc = new Pieces(board.at(count)->getRectX() + 15, board.at(count)->getRectY() + 5, type, board.at(count)->getNumber(), BLACK, 80, 100);
            black.push_back(pc);
            (*(board.at(count))).setPiece();
            (*(board.at(count))).setPieceColor(BLACK);
            count--;
        }
    }
}//end of initBoard

void ChessMain::updateGame()
{
    this->update();
}

void ChessMain::nextTurn()
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

    if(currentTurn == BLACK)
    {
        currentTurn = RED;
    }
    else
    {
        currentTurn = BLACK;
    }


}//end of updateGame

