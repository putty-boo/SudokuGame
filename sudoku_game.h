#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QTableWidgetItem> //классы для работы с таблицей
#include <QAbstractItemView>

namespace Ui {
class Sudoku_game;
}

class Sudoku_game : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Sudoku_game(QWidget *parent = 0);
    ~Sudoku_game();

private slots:
    void centerOnDesktop();

    void on_pushButton_clicked(); //меню

    void on_pushButton_2_clicked(); //запуск ф-ции подсказки

    void count(int a[9][9]); //проверка на заполненность судоку

    void changed(QTableWidgetItem *item); //проверка ввода данных

    void help(int a[9][9]); //ф-ия подсказки: выделяет красным неправильно поставленные цифры

private:
    Ui::Sudoku_game *ui;
};

#endif // SUDOKU_GAME_H
