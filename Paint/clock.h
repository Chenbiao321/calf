#ifndef CLOCK_H
#define CLOCK_H
#include "mainwindow.h"

class myclock
{
public:
    myclock() = default;
    myclock(int x ,int y, int r);
    void drawclock(QPainter &painter);
    int getX();
    int getY();
    int getR();
    void setX(int x);
    void setY(int y);
    void setR(int r);
private:
    int x;
    int y;
    int r;
};

#endif // CLOCK_H
