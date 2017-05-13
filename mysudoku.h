#ifndef MYSUDOKU_H
#define MYSUDOKU_H

#include <QMainWindow>
#include <QApplication> //классы для ф-ции отцентровки окон
#include <QDesktopWidget>

namespace Ui {
class MySudoku;
}

class MySudoku : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MySudoku(QWidget *parent = 0);
    ~MySudoku();


private slots:
    void centerOnDesktop(); //отцентровка окон

    void on_pushButton_clicked(); //лёгкий уровень

    void on_pushButton_2_clicked(); //средний уровень

    void on_pushButton_3_clicked(); //сложный уровень

private:
    Ui::MySudoku *ui;
};

#endif // MYSUDOKU_H
