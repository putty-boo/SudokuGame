#include "mysudoku.h"
#include "ui_mysudoku.h"
#include "sudoku_game.h"
#include "won.h"
#include "youlose.h"

int yrov; //объявление глобальной переменной для уровня сложности

MySudoku::MySudoku(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MySudoku)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: rgb(205, 222, 233)"); //цвет фона окна
    centerOnDesktop();
}

MySudoku::~MySudoku()
{
    delete ui;
}

void MySudoku::centerOnDesktop() //ф-ция перемещает окно в центр экрана
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

void MySudoku::on_pushButton_clicked() /*переменная уровня меняет свое значение,
открывается окно игры, а окно меню скрывается*/
{
    yrov=1;
    Sudoku_game * w = new Sudoku_game(this);
    w->show();
    hide();
}

void MySudoku::on_pushButton_2_clicked() //аналогично
{
    yrov=2;
    Sudoku_game * w = new Sudoku_game(this);
    w->show();
    hide();
}

void MySudoku::on_pushButton_3_clicked() //аналогично
{
    yrov=3;
    Sudoku_game * w = new Sudoku_game(this);
    w->show();
    hide();
}
