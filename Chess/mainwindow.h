#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chessmain.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_play_clicked();

    void on_exit_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    bool devOptions;
    ChessMain* cm;
};

#endif // MAINWINDOW_H
