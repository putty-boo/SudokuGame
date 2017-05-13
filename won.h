#ifndef WON_H
#define WON_H

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>

namespace Ui {
class won;
}

class won : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit won(QWidget *parent = 0);
    ~won();
    
private slots:
    void centerOnDesktop();

    void on_pushButton_clicked(); //меню

private:
    Ui::won *ui;
};

#endif // WON_H
