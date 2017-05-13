#include "won.h"
#include "ui_won.h"
#include "sudoku_game.h"
#include "mysudoku.h"

won::won(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::won)
{
    ui->setupUi(this);
    centerOnDesktop();
}

won::~won()
{
    delete ui;
}

void won::on_pushButton_clicked() //открывает меню 
{
    MySudoku * w = new MySudoku(this);
    w->show();
    hide();
}

void won::centerOnDesktop()
{
    quint16 screenWidth, width, screenHeight, height, x, y;
    QSize windowSize;

    screenWidth = QApplication::desktop()->width();
    screenHeight = QApplication::desktop()->height();
    windowSize = size();
    width = windowSize.width();
    height = windowSize.height();
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    y -= 100;
    move(x, y);
}
