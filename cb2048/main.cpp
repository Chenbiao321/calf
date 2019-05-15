#include "cb2048.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(NULL));
    cb2048 w;
    w.show();

    return a.exec();
}
