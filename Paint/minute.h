#ifndef MINUTE_H
#define MINUTE_H
#include "mainwindow.h"
#include <QTime>
#include <QTimer>

template<typename T>
class minute : public Hand<T>
{
public:
    minute()
    {

    }
    minute(T w, T h, T x, T y):Hand<T>(w, h, x, y)
    {

    }
    ~minute()
    {

    }

    void drawhand(QPainter &painter)
    {

        QTime time = QTime::currentTime();
        painter.setBrush(Qt::red);
        painter.setPen(Qt::red);
        painter.save();
        //painter.translate(200, 200);
        painter.rotate(6.0*(time.minute() + time.second()/60));

        painter.drawConvexPolygon(this->pointHand, 4);
        painter.restore();
    }


};




#endif // MINUTE_H
