#ifndef YOULOSE_H
#define YOULOSE_H

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>

namespace Ui {
class YouLose;
}

class YouLose : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit YouLose(QWidget *parent = 0);
    ~YouLose();
    
private slots:
    void centerOnDesktop();

    void on_pushButton_2_clicked(); //меню

private:
    Ui::YouLose *ui;
};

#endif // YOULOSE_H
