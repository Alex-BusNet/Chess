#include "chessmain.h"
#include "QDebug"
#include "pieces.h"
#include "string"
#include "sstream"

using namespace std;

const char StartingPlacement[2][8] =
{
    {'R','N','B','K','Q','B','N','R'},
    {'P','P','P','P','P','P','P','P'}
};

ChessMain::ChessMain(QWidget *parent) : QWidget(parent)
{
    QWidget::setFixedSize(1200, 900);
    devOpts = false;
    promote = false;

    BOARD_HEIGHT = 8; //Squares
    BOARD_WIDTH = 8; //Squares
    size = 105; //px

    //Centers the board depending on the size of each square
    rectPosX = 450 - (size * 4);
    rectPosY = this->height() / 2 - (size * 4);

    if(devOpts) qDebug() << "init board";
    initBoard();
    if(devOpts) qDebug() << "init complete";

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
        pen.setColor(Qt::black);
        painter.setPen(pen);

        if(devOpts)
        {
            //Board Location numbers:
            painter.drawText((*(board.at(i))).getRectX() + 5, (*(board.at(i))).getRectY() + 90, QString::number(board.at(i)->getNumber()));
        }
    }

    for(int i = 0; i < red.size(); i++)
    {
        if((*(red.at(i))).isActive())
        {
            (*(red.at(i))).drawPiece(painter, devOpts);
        }
    }

    for(int i = 0; i < black.size(); i++)
    {
        if((*(black.at(i))).isActive())
        {
            (*(black.at(i))).drawPiece(painter, devOpts);
        }
    }

    QFont* GameInfoFont = new QFont();
    GameInfoFont->setPointSize(24);
    painter.setFont(*GameInfoFont);
    painter.drawText(950, 80, "Turn:");

    if(currentTurn == BLACK)
        painter.drawText(1030, 80, "Black");
    else
        painter.drawText(1030, 80, "Red");

    if(promote)
    {
        GameInfoFont->setPointSize(26);
        pen.setColor(Qt::red);
        painter.drawText(950, 150, "PROMOTION!");
    }


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

                            showMoves((*(board.at(i))).getNumber());
                            lastPieceSelected = (*(board.at(i))).getNumber();
                        }

                        break;
                    }
                    else if((*(board.at(i))).isEnemyHighlighted())
                    {
                        if(BLACK == currentTurn)
                        {
                            attack(lastPieceSelected, (*(board.at(i))).getNumber(), black, red);
                        }
                        else if(RED == currentTurn)
                        {
                            attack(lastPieceSelected, (*(board.at(i))).getNumber(), red, black);
                        }
                    }
                }
                else if(pieceSelected)
                {
                    if((*(board.at(i))).isMoveHighlighted())
                    {
                        if(BLACK == currentTurn)
                        {
                            makeMove(lastPieceSelected, (*(board.at(i))).getNumber(), black);
                        }
                        else if(RED == currentTurn)
                        {
                            makeMove(lastPieceSelected, (*(board.at(i))).getNumber(), red);
                        }
                    }
                }
            }
        }
    }
}//end of mouseReleaseEvent


void ChessMain::showMoves(int location)
{
    int rangeY = 0, rangeX = 0;
    char type = 'P';

    if(promote)
    {
        promote = false;
    }

    //Gets the Range of the piece inside the selected square
    if((*(board.at(location))).getPieceColor() == BLACK)
    {
        for(int i = 0; i < black.size(); i++)
        {
            if(location == (*(black.at(i))).getLoc())
            {
                rangeY = (*(black.at(i))).getRangeY();
                rangeX = (*(black.at(i))).getRangeX();
                type = (*(black.at(i))).getType();
                break;
            }
        }
    }
    else if((*(board.at(location))).getPieceColor() == RED)
    {
        for(int i = 0; i < red.size(); i++)
        {
            if(location == (*(red.at(i))).getLoc())
            {
                rangeY = (*(red.at(i))).getRangeY();
                rangeX = (*(red.at(i))).getRangeX();
                type = (*(red.at(i))).getType();
                break;
            }
        }
    }

    if(type == 'P')
    {
        if(devOpts) qDebug() << "   Pawn Location:" << location;

        if(currentTurn == BLACK)
        {
            if(location >= 48 && location <= 55)
            {
                rangeY++;
            }
        }
        else if(currentTurn == RED)
        {
            if(location >= 8 && location <= 15)
            {
                rangeY++;
            }
        }
    }


    //Highlights the correct squares corresponding to the range of the selected piece.
    ///Currently only works for pieces moving in the Y-direction, Knight, and Bishop pieces. Horizontals, Queen and Kind not fully supported yet.

    if(devOpts) qDebug() << "RangeY =" << rangeY << "RangeX =" << rangeX <<"Type =" << type;

    if((*(board.at(location))).getPieceColor() == currentTurn)
    {
        if(type == 'N')
        {
            if(location - 6 >= 0) //Towards Red team
            {
                if(location % 8 < 2)
                {
                    if(!(*(board.at(location - 6))).hasPiece())
                    {
                        (*(board.at(location - 6))).highlightMove();
                    }
                    else if((*(board.at(location - 6))).getPieceColor() != currentTurn && ((*(board.at(location - 6))).getPieceColor() != EMPTY))
                    {
                        (*(board.at(location - 6))).highlightEnemy();
                    }
                }

                if(location % 8  < 7)
                {
                    if(location - 15 >= 0)
                    {
                        if(!(*(board.at(location - 15))).hasPiece())
                        {
                            (*(board.at(location - 15))).highlightMove();
                        }
                        else if((*(board.at(location - 15))).getPieceColor() != currentTurn && ((*(board.at(location - 15))).getPieceColor() != EMPTY))
                        {
                            (*(board.at(location - 15))).highlightEnemy();
                        }
                    }
                }

                if(location % 8 > 0)
                {
                    if(location - 17 >= 0)
                    {
                        if(!(*(board.at(location - 17))).hasPiece())
                        {
                            (*(board.at(location - 17))).highlightMove();
                        }
                        else if((*(board.at(location - 17))).getPieceColor() != currentTurn && ((*(board.at(location - 17))).getPieceColor() != EMPTY))
                        {
                            (*(board.at(location - 17))).highlightEnemy();
                        }
                    }
                }
            }

            if(location + 6 <= 63) //Towards Black team
            {
                if(location % 8 > 1)
                {
                    if(!(*(board.at(location + 6))).hasPiece())
                    {
                        (*(board.at(location + 6))).highlightMove();
                    }
                    else if((*(board.at(location + 6))).getPieceColor() != currentTurn && ((*(board.at(location + 6))).getPieceColor() != EMPTY))
                    {
                        (*(board.at(location + 6))).highlightEnemy();
                    }
                }

                if(location % 8 < 6)
                {
                    if(location + 10 <= 63)
                    {
                        if(!(*(board.at(location + 10))).hasPiece())
                        {
                            (*(board.at(location + 10))).highlightMove();
                        }
                        else if((*(board.at(location + 10))).getPieceColor() != currentTurn && ((*(board.at(location + 10))).getPieceColor() != EMPTY))
                        {
                            (*(board.at(location + 10))).highlightEnemy();
                        }
                    }
                }

                if(location % 8 > 0)
                {
                    if(location + 15 <= 63)
                    {
                        if(!(*(board.at(location + 15))).hasPiece())
                        {
                            (*(board.at(location + 15))).highlightMove();
                        }
                        else if((*(board.at(location + 15))).getPieceColor() != currentTurn && ((*(board.at(location - 15))).getPieceColor() != EMPTY))
                        {
                            (*(board.at(location + 15))).highlightEnemy();
                        }
                    }
                }

                if(location % 8 < 7)
                {
                    if(location + 17 <= 63)
                    {
                        if(!(*(board.at(location + 17))).hasPiece())
                        {
                            (*(board.at(location + 17))).highlightMove();
                        }
                        else if((*(board.at(location + 17))).getPieceColor() != currentTurn && ((*(board.at(location - 17))).getPieceColor() != EMPTY))
                        {
                            (*(board.at(location + 17))).highlightEnemy();
                        }
                    }
                }
            }

//                break; //Ends the loop after selecting the appropriate squares for the Knight.
        }

        else if(type == 'B' || type == 'Q' || type == 'K')
        {
            if(location % 8 > 0)
            {
                if(devOpts) qDebug() << "Upper Left";

                for(int i = 0; i < (location % 8); i++) //Towards Upper Left Corner
                {
                    if(location - (9 * (i + 1)) >= 0)
                    {
                        if(((location - (9 * (i + 1))) % 8) == ((location - (i + 1)) % 8))
                        {
                            if(!(*(board.at(location - (9 * (i + 1))))).hasPiece())
                            {
                                if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location - (9 * (i + 1))))).getNumber();
                                (*(board.at(location - (9 * (i + 1))))).highlightMove();
                            }
                            else if((*(board.at(location - (9 * (i + 1))))).getPieceColor() != currentTurn && ((*(board.at(location - (9 * (i + 1))))).getPieceColor() != EMPTY))
                            {
                                if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location - (9 * (i + 1))))).getNumber();
                                (*(board.at(location - (9 * (i + 1))))).highlightEnemy();
                                break;
                            }
                            else if((*(board.at(location - (9 * (i + 1))))).getPieceColor() == currentTurn)
                            {
                                if(devOpts) qDebug() << "Friendly at:" << (*(board.at(location - (9 * (i + 1))))).getNumber();
                                break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }

                    if(type == 'K')
                    {
                        break;
                    }
                }

                if(devOpts) qDebug() << "Lower Left";

                for(int i = 0; i < (location % 8); i++) //Towards Lower Left Corner
                {
                    if(location + (7 * (i + 1)) <= 63)
                    {
                        if(((location + (7 * (i + 1))) % 8) == ((location - (i + 1)) % 8))
                        {
                            if(!(*(board.at(location + (7 * (i + 1))))).hasPiece())
                            {
                                if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location + (7 * (i + 1))))).getNumber();
                                (*(board.at(location + (7 * (i + 1))))).highlightMove();
                            }
                            else if((*(board.at(location + (7 * (i + 1))))).getPieceColor() != currentTurn && ((*(board.at(location + (7 * (i + 1))))).getPieceColor() != EMPTY))
                            {
                                if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location + (7 * (i + 1))))).getNumber();
                                (*(board.at(location + (7 * (i + 1))))).highlightEnemy();
                                break;
                            }
                            else if((*(board.at(location + (7 * (i + 1))))).getPieceColor() == currentTurn)
                            {
                                if(devOpts) qDebug() << "Friendly at" << (*(board.at(location + (7 * (i + 1))))).getNumber();
                                break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }

                    if(type == 'K')
                    {
                        break;
                    }
                }
            }

            if(location % 8 < 7)
            {
                if(devOpts) qDebug() << "Upper Right";

                for(int i = 0; i < (7 - (location % 8)); i++) //Towards Upper Right Corner
                {
                    if(location - (7 * (i + 1)) >= 0)
                    {
                        if(((location - (7 * (i + 1))) % 8) == ((location + (i + 1)) % 8))
                        {
                            if(!(*(board.at(location - (7 * (i + 1))))).hasPiece())
                            {
                                if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location - (7 * (i + 1))))).getNumber();
                                (*(board.at(location - (7 * (i + 1))))).highlightMove();
                            }
                            else if((*(board.at(location - (7 * (i + 1))))).getPieceColor() != currentTurn && ((*(board.at(location - (7 * (i + 1))))).getPieceColor() != EMPTY))
                            {
                                if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location - (7 * (i + 1))))).getNumber();
                                (*(board.at(location - (7 * (i + 1))))).highlightEnemy();
                                break;
                            }
                            else if((*(board.at(location - (7 * (i + 1))))).getPieceColor() == currentTurn)
                            {
                                if(devOpts) qDebug() << "Friendly at" << (*(board.at(location - (7 * (i + 1))))).getNumber();
                                break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }

                    if(type == 'K')
                    {
                        break;
                    }
                }

                if(devOpts) qDebug() << "Lower Right";

                for(int i = 0; i < (7 - (location % 8)); i++) //Towards Lower Right Corner
                {
                    if(location + (9 * (i + 1)) <= 63)
                    {
                        if(((location + (9 * (i + 1))) % 8) == ((location + (i + 1)) % 8))
                        {
                            if(!(*(board.at(location + (9 * (i + 1))))).hasPiece())
                            {
                                if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location + (9 * (i + 1))))).getNumber();
                                (*(board.at(location + (9 * (i + 1))))).highlightMove();
                            }
                            else if((*(board.at(location + (9 * (i + 1))))).getPieceColor() != currentTurn && ((*(board.at(location + (9 * (i + 1))))).getPieceColor() != EMPTY))
                            {
                                if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location + (9 * (i + 1))))).getNumber();
                                (*(board.at(location + (9 * (i + 1))))).highlightEnemy();
                                break;
                            }
                            else if((*(board.at(location + (9 * (i + 1))))).getPieceColor() == currentTurn)
                            {
                                if(devOpts) qDebug() << "Friendly at:" << (*(board.at(location + (9 * (i + 1))))).getNumber();
                                break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }

                    if(type == 'K')
                    {
                        break;
                    }
                }
            }

            if(type == 'Q' || type == 'K')
            {
                if((*(board.at(location))).getPieceColor() == BLACK)
                    goto axisB;
                else if((*(board.at(location))).getPieceColor() == RED)
                    goto axisR;
            }
        }

        else if((*(board.at(location))).getPieceColor() == BLACK)
        {
axisB:
            if(devOpts) qDebug() << " Black Front";

            for(int i = 0; i < rangeY; i++)
            {
                if(location - (8 * (i + 1)) >= 0)
                {
                    if(!((*(board.at(location - (8 * (i + 1))))).hasPiece()))
                    {
                        if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location - (8 * (i + 1))))).getNumber();
                        (*(board.at(location - (8 * (i + 1))))).highlightMove();
                    }
                    else
                    {
                        if((*(board.at(location - (8 * (i + 1))))).getPieceColor() != currentTurn && (*(board.at(location - (8 * (i + 1))))).getPieceColor() != EMPTY)
                        {
                            if(type != 'P')
                            {
                                if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location - (8 * (i + 1))))).getNumber();
                                (*(board.at(location - (8 * (i + 1))))).highlightEnemy();
                                break; //End loop if Enemy is found
                            }
                        }
                        else
                        {
                            if(devOpts) qDebug() << "Friendly at:" << (*(board.at(location - (8 * (i + 1))))).getNumber();
                            break;
                        }
                    }

                    if(type == 'P')
                    {
                        if(location % 8 > 0)
                        {
                            if((*(board.at(location - 9))).getPieceColor() != currentTurn && ((*(board.at(location - 9))).getPieceColor() != EMPTY))
                            {
                                (*(board.at(location - 9))).highlightEnemy();
                            }
                        }

                        if(location % 8 < 7)
                        {
                            if((*(board.at(location - 7))).getPieceColor() != currentTurn && ((*(board.at(location - 7))).getPieceColor() != EMPTY))
                            {
                                (*(board.at(location - 7))).highlightEnemy();
                                break;
                            }
                        }
                    }
                }
                else
                {
                    break;
                }
            }

            if(type != 'P') //Highlights squares behind selected piece
            {
                if(devOpts) qDebug() << " Black Back";

                for (int i = 0; i < rangeY; i++)
                {
                    if(location + (8 * (i + 1)) <= 63)
                    {
                        if(!((*(board.at(location + (8 * (i + 1))))).hasPiece()))
                        {
                            if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location + (8 * (i + 1))))).getNumber();
                            (*(board.at(location + (8 * (i + 1))))).highlightMove();
                        }
                        else
                        {
                            if((*(board.at(location + (8 * (i + 1))))).getPieceColor() != currentTurn && (*(board.at(location + (8 * (i + 1))))).getPieceColor() != EMPTY)
                            {
                                if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location + (8 * (i + 1))))).getNumber();
                                (*(board.at(location + (8 * (i + 1))))).highlightEnemy();
                                break; //End loop if Enemy is found
                            }
                            else
                            {
                                if(devOpts) qDebug() << "Friendly At:" << (*(board.at(location + (8 * (i + 1))))).getNumber();
                                break; //End loop if Friendly is found
                            }
                        }
                    }
                }
            }

            if(rangeX > 0)
            {
                if(devOpts) qDebug() << " Right";

                for(int i = 0; i < rangeX; i++)
                {
                    if(location + (i + 1) <= 63 && (location % 8) != 7)
                    {
                        if((location + (i + 1)) % 8 != 0)
                        {
                            //Look to the Right
                            if(!(*(board.at(location + (i + 1)))).hasPiece())
                            {
                                if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location + (i + 1)))).getNumber();
                                (*(board.at(location + (i +1)))).highlightMove();
                            }
                            else
                            {
                                if((*(board.at(location + (i + 1)))).getPieceColor() != currentTurn && (*(board.at(location + (i + 1)))).getPieceColor() != EMPTY)
                                {
                                    if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location + (i + 1)))).getNumber();
                                    (*(board.at(location + (i + 1)))).highlightEnemy();
                                    break; //End loop if Enemy is found
                                }
                                else
                                {
                                    if(devOpts) qDebug() << "Friendly at:" << (*(board.at(location + (i + 1)))).getNumber();
                                    break; //End loop if Friendly is found
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                if(devOpts) qDebug() << " Left";
                for(int i = 0; i < rangeX; i++)
                {
                    if(location - (i + 1) >= 0 && (location % 8) != 0)
                    {
                        if((location - (i + 1)) % 8 != 7)
                        {
                            //Look to the Left
                            if(!(*(board.at(location - (i + 1)))).hasPiece())
                            {
                                if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location - (i + 1)))).getNumber();

                                (*(board.at(location - (i + 1)))).highlightMove();
                            }
                            else
                            {
                                if((*(board.at(location - (i + 1)))).getPieceColor() != currentTurn && (*(board.at(location - (i + 1)))).getPieceColor() != EMPTY)
                                {
                                    if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location - (i + 1)))).getNumber();

                                    (*(board.at(location - (i + 1)))).highlightEnemy();
                                    break; //End loop if Enemy is found
                                }
                                else
                                {
                                    if(devOpts) qDebug() << "Friendly at:" << (*(board.at(location - (i + 1)))).getNumber();
                                    break; //End loop if Friendly is found
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        else if ((*(board.at(location))).getPieceColor() == RED)
        {
axisR:
            if(devOpts) qDebug() << " Red Front";

            for(int i = 0; i < rangeY; i++)
            {
                if(location + (8 * (i + 1)) <= 63)
                {
                    if(!((*(board.at(location + (8 * (i + 1))))).hasPiece()))
                    {
                        if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location + (8 * (i + 1))))).getNumber();
                        (*(board.at(location + (8 * (i + 1))))).highlightMove();
                    }
                    else
                    {
                        if((*(board.at(location + (8 * (i + 1))))).getPieceColor() != currentTurn && (*(board.at(location + (8 * (i + 1))))).getPieceColor() != EMPTY)
                        {
                            if(type != 'P')//Pawns cannot attack directly infront of them, only diagonally moving forward.
                            {
                                if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location + (8 * (i + 1))))).getNumber();
                                (*(board.at(location + (8 * (i + 1))))).highlightEnemy();
                                break; //End loop if Enemy is found
                            }
                        }
                        else
                        {
                            if(devOpts) qDebug() << "Friendly:" << (*(board.at(location + (8 * (i + 1))))).getNumber();
                            break; //End loop if Friendly is found
                        }
                    }

                    if(type == 'P')
                    {
                        if(location % 8 < 7)
                        {
                            if((*(board.at(location + 9))).getPieceColor() != currentTurn && ((*(board.at(location + 9))).getPieceColor() != EMPTY))
                            {
                                (*(board.at(location + 9))).highlightEnemy();
                            }
                        }

                        if(location % 8 > 0)
                        {
                            if((*(board.at(location + 7))).getPieceColor() != currentTurn && ((*(board.at(location + 7))).getPieceColor() != EMPTY))
                            {
                                (*(board.at(location + 7))).highlightEnemy();
                                break;
                            }
                        }
                    }
                }
            }

            if(type != 'P') //Highlight spaces behind selected piece
            {
                if(devOpts) qDebug() << " Red Back";

                for(int i = 0; i < rangeY; i++)
                {
                    if(location - (8 * (i + 1)) >= 0)
                    {
                        if(!((*(board.at(location - (8 * (i + 1))))).hasPiece()))
                        {
                            if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location - (8 * (i + 1))))).getNumber();
                            (*(board.at(location - (8 * (i + 1))))).highlightMove();
                        }
                        else
                        {
                            if((*(board.at(location - (8 * (i + 1))))).getPieceColor() != currentTurn && (*(board.at(location - (8 * (i + 1))))).getPieceColor() != EMPTY)
                            {
                                if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location - (8 * (i + 1))))).getNumber();
                                (*(board.at(location - (8 * (i + 1))))).highlightEnemy();
                                break; //End loop if Enemy is found
                            }
                            else
                            {
                                if(devOpts) qDebug() << "Friendly at:" << (*(board.at(location - (8 * (i + 1))))).getNumber();
                                break; //End loop if Friendly is found
                            }
                        }
                    }
                }
            }

            if(rangeX > 0)
            {
                if(devOpts) qDebug() << " Right";
                for(int i = 0; i < rangeX; i++)
                {
                    if(location + (i + 1) <= 63 && (location % 8) != 7)
                    {
                        if((location + (i + 1)) % 8 != 0)
                        {
                            //Look to the Right
                            if(!(*(board.at(location + (i + 1)))).hasPiece())
                            {
                                if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location + (i + 1)))).getNumber();
                                (*(board.at(location + (i + 1)))).highlightMove();
                            }
                            else
                            {
                                if((*(board.at(location + (i + 1)))).getPieceColor() != currentTurn && (*(board.at(location + (i + 1)))).getPieceColor() != EMPTY)
                                {
                                    if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location + (i + 1)))).getNumber();
                                    (*(board.at(location + (i + 1)))).highlightEnemy();
                                    break; //End loop if Enemy is found
                                }
                                else
                                {
                                    if(devOpts) qDebug() << "Friendly at:" << (*(board.at(location + (i + 1)))).getNumber();
                                    break; //End loop if Friendly is found
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                if(devOpts) qDebug() << " Left";
                for(int i = 0; i < rangeX; i++)
                {
                    if(location - (i + 1) >= 0 && (location % 8) != 0)
                    {
                        if((location - (i + 1)) % 8 != 7)
                        {
                            //Look to the Left
                            if(!(*(board.at(location - (i + 1)))).hasPiece())
                            {
                                if(devOpts) qDebug() << "Highlighting:" << (*(board.at(location - (i +1)))).getNumber();
                                (*(board.at(location - (i + 1)))).highlightMove();
                            }
                            else
                            {
                                if((*(board.at(location - (i + 1)))).getPieceColor() != currentTurn && (*(board.at(location - (i + 1)))).getPieceColor() != EMPTY)
                                {
                                    if(devOpts) qDebug() << "Enemy at:" << (*(board.at(location - (i + 1)))).getNumber();
                                    (*(board.at(location - (i + 1)))).highlightEnemy();
                                    break; //End loop if Enemy is found
                                }
                                else
                                {
                                    if(devOpts) qDebug() << "Friendly at:" << (*(board.at(location - (i + 1)))).getNumber();
                                    break; //End loop if Friendly is found
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

}//end of showMoves


//=====================================================================================
//
//  oldLoc: The number of the square the selected piece is currently occupying.
//  newLoc: The number of the square the selected piece is moving to.
//
//  index is the value (0-15) where the selected piece resides within the stored vector
//
//======================================================================================

void ChessMain::makeMove(int oldLoc, int newLoc, const QVector<Pieces*> &vec)
{
    int index = 0;

    for(int i = 0; i < vec.size(); i++)
    {
        if(vec.at(i)->getLoc() == oldLoc)
        {
            index = i;
            break;
        }
    }

    showMoves(oldLoc);
    (*(board.at(oldLoc))).select();
    pieceSelected = false; //Reset the piece selected flag

    vec.at(index)->setLoc(newLoc);
    vec.at(index)->setPos((*(board.at(newLoc))).getRectX() + 15, (*(board.at(newLoc))).getRectY() + 5);

    (*(board.at(oldLoc))).setPiece();
    (*(board.at(oldLoc))).setPieceColor(EMPTY);
    (*(board.at(newLoc))).setPiece();
    (*(board.at(newLoc))).setPieceColor(currentTurn);

    if(vec.at(index)->getType() == 'P')
    {
        if(newLoc + 8 > 63)
        {
            promotion(newLoc, vec);
        }
        else if(newLoc - 8 < 0)
        {
            promotion(newLoc, vec);
        }
    }

    endTurn->click();

}//end of makeMove

void ChessMain::attack(int atkLoc, int trgLoc, const QVector<Pieces*> &atkVector, const QVector<Pieces*> &trgVector)
{
    int atkIndex = 0, trgIndex = 0;

    for(int i = 0; i < atkVector.size(); i++)
    {
        if(atkVector.at(i)->getLoc() == atkLoc)
        {
            atkIndex = i;
            break;
        }
    }

    for(int i = 0; i < trgVector.size(); i++)
    {
        if(trgVector.at(i)->getLoc() == trgLoc)
        {
            trgIndex = i;
            break;
        }
    }

    showMoves((*(board.at(atkLoc))).getNumber());
    (*(board.at(atkLoc))).select();
    pieceSelected = false; //Reset the piece selected flag

    trgVector.at(trgIndex)->setActive();
    trgVector.at(trgIndex)->setLoc(-1);
    (*(atkVector.at(atkIndex))).setLoc(trgLoc);
    (*(atkVector.at(atkIndex))).setPos((*(trgVector.at(trgIndex))).getPosX(), (*(trgVector.at(trgIndex))).getPosY());

    (*(board.at(atkLoc))).setPiece();
    (*(board.at(atkLoc))).setPieceColor(EMPTY);
    (*(board.at(trgLoc))).setPieceColor(currentTurn);

    ///INSERT END GAME CHECK HERE

    if(atkVector.at(atkIndex)->getType() == 'P')
    {
        if(trgLoc + 8 > 63)
        {
            promotion(trgLoc, atkVector);
        }
        else if(trgLoc - 8 < 0)
        {
            promotion(trgLoc, atkVector);
        }
    }

    endTurn->click();

}//end of attack

void ChessMain::gameOver()
{

}//end of gameOver

void ChessMain::promotion(int loc, const QVector<Pieces *> &vec)
{
    int index = 0;

    for(int i = 0; i < vec.size(); i++)
    {
        if(vec.at(i)->getLoc() == loc)
        {
            index = i;
            break;
        }
    }

    for(int i = 0; i < vec.size(); i++)
    {
        if(vec.at(i)->getType() == 'Q' && !(vec.at(i)->isActive()))
        {
            promote = true;

            vec.at(i)->setActive();
            vec.at(i)->setLoc(loc);
            vec.at(i)->setPos(vec.at(index)->getPosX(), vec.at(index)->getPosY());

            vec.at(index)->setActive();
            vec.at(index)->setLoc(-1);

            break;
        }
    }

}//end of Promotion


void ChessMain::initBoard()
{
    int count = 0;
    int indexCount = 0;

    if(devOpts) qDebug() << "Generating Board";
    for(int i = 0; i < BOARD_HEIGHT; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
            Board* bsqr = new Board(rectPosX, rectPosY, size, count, indexCount, false, EMPTY);

            board.push_back(bsqr);

            rectPosX += size + 1;
            count++;

            if(devOpts) qDebug() << "indexCount:" << indexCount;

            if(indexCount < 15)
                indexCount++;
            else
                indexCount = 0;

        }

        rectPosY += size + 1;
        rectPosX = 450 - (size * 4);
    }

    count = 0;

    if(devOpts) qDebug() << "Generating Red Pieces";

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < BOARD_HEIGHT; j++)
        {
            char type = StartingPlacement[i][j];
            Pieces* pc = new Pieces((*(board.at(count))).getRectX() + 15, (*(board.at(count))).getRectY() + 5, type, (*(board.at(count))).getNumber(), RED, 80, 100, true);
            red.push_back(pc);
            (*(board.at(count))).setPiece();
            (*(board.at(count))).setPieceColor(RED);
            count++;
        }
    }

    count = 63;

    if(devOpts) qDebug() << "Generating Black Pieces";

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
            char type = StartingPlacement[i][j];
            Pieces* pc = new Pieces(board.at(count)->getRectX() + 15, board.at(count)->getRectY() + 5, type, board.at(count)->getNumber(), BLACK, 80, 100, true);
            black.push_front(pc);
            (*(board.at(count))).setPiece();
            (*(board.at(count))).setPieceColor(BLACK);
            count--;
        }
    }
}//end of initBoard

void ChessMain::setDevOpts(bool show)
{
    this->devOpts = show;
}


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

