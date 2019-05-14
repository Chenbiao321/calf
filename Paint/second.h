#ifndef SECOND_H
#define SECOND_H
#include "mainwindow.h"
#include <QTime>
#include <QTimer>

template<typename T>
class second : public Hand<T>
{
public:
    second()
    {

    }
    second(T w, T h, T x, T y):Hand<T>(w, h, x, y)
    {

    }
    ~second()
    {

    }

    void drawhand(QPainter &painter)
    {

        QTime time = QTime::currentTime();
        painter.setBrush(Qt::black);
        painter.setPen(Qt::black);
        painter.save();
        //painter.translate(200, 200);
        painter.rotate(6.0*(time.second()));

        painter.drawConvexPolygon(this->pointHand, 4);
        painter.restore();
    }

};

#endif // SECOND_H
