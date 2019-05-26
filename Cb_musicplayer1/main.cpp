#include "musicplayer.h"
#include "creatdatabase.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Createdatabase();
    musicplayer w;
    w.show();

    return a.exec();
}
