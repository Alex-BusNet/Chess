#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cm = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cm;
}

void MainWindow::on_play_clicked()
{
    if(cm == NULL)
    {
        cm = new ChessMain();
    }
    else
    {
        delete cm;
        cm = new ChessMain();
    }

    cm->show();
}

void MainWindow::on_exit_clicked()
{
    QApplication::exit();
}
