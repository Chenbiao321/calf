#include "clock.h"
#include <QPen>
#include <QFont>

myclock::myclock(int x ,int y, int r):x(x), y(y), r(r)
{
}
void myclock::drawclock(QPainter &painter)
{
    QPen h;
    h.setWidth(4.0);
    QPen m;
    m.setWidth(2.0);
    for(int i = 0;i < 60;i++)
    {
        if(i % 5 == 0)
        {
            painter.setPen(h);
            painter.drawLine(QPoint(x, y-r), QPoint(x, y-r+8));
            painter.rotate(6.0);
        }
        else
        {
            painter.setPen(m);
            painter.drawLine(QPoint(x, y-r), QPoint(x, y-r+4));
            painter.rotate(6.0);
        }
    }
}
int myclock::getX()
{
    return x;
}
int myclock::getY()
{
    return y;
}
int myclock::getR()
{
    return r;
}
void myclock::setX(int x)
{
    this->x = x;
}
void myclock::setY(int y)
{
    this->y = y;
}
void myclock::setR(int r)
{
    this->r = r;
}
