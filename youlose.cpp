#include "youlose.h"
#include "ui_youlose.h"
#include "sudoku_game.h"
#include "mysudoku.h"

YouLose::YouLose(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YouLose)
{
    ui->setupUi(this);
    centerOnDesktop();
}

YouLose::~YouLose()
{
    delete ui;
}

void YouLose::on_pushButton_2_clicked() //открывает меню 
{
    MySudoku * w = new MySudoku(this);
    w->show();
    hide();
}

void YouLose::centerOnDesktop()
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
