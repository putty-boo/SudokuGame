#include <QtGui/QApplication> //класс для создания графического приложения
#include "mysudoku.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MySudoku w;
    w.show();
    return a.exec();
}
