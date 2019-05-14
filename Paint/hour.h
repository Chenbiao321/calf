#ifndef HOUR_H
#define HOUR_H
#include "mainwindow.h"
#include <QTime>
#include <QTimer>

template<typename T>
class hour : public Hand<T>
{
public:
    hour()
    {

    }
    hour(T w, T h, T x, T y):Hand<T>(w, h, x, y)
    {

    }
    ~hour()
    {

    }

    void drawhand(QPainter &painter)
    {

        QTime time = QTime::currentTime();
        painter.setBrush(Qt::blue);
        painter.setPen(Qt::blue);
        painter.save();
        //painter.translate(200, 200);
        painter.rotate(30*(time.hour() + time.minute()/60));

        painter.drawConvexPolygon(this->pointHand, 4);
        painter.restore();
    }

};

#endif // HOUR_H
