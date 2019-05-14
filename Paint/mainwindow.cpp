#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hour.h"
#include "minute.h"
#include "second.h"
#include "clock.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.translate(200, 200);
    painter.rotate(270);
    hour<int> h(15,60,0, 0);
    minute<int> j(10, 90, 0, 0);
    second<int> k(5, 120, 0, 0);
    myclock c(0, 0, 150);
    c.drawclock(painter);
    h.drawhand(painter);
    j.drawhand(painter);
    k.drawhand(painter);


}
template<typename T>
Hand<T>::Hand(T w, T h, T x, T y)
{
    this->w = w;
    this->h = h;
    this->x = x;
    this->y = y;
    this->pointHand[0].setX(x);
    this->pointHand[0].setY(y+w/2);
    this->pointHand[1].setX(x+h);
    this->pointHand[1].setY(y);
    this->pointHand[2].setX(x);
    this->pointHand[2].setY(y-w/2);
    this->pointHand[3].setX(x-5);
    this->pointHand[3].setY(y);
}
template<typename T>
T Hand<T>::getH()
{
    return this->h;
}
template<typename T>
T Hand<T>::getW()
{
    return this->w;
}
template<typename T>
T Hand<T>::getX()
{
    return this->x;
}
template<typename T>
T Hand<T>::getY()
{
    return this->y;
}
template<typename T>
void Hand<T>::setHand(T w, T h, T x, T y)
{
    this->pointHand[0].setX(x);
    this->pointHand[0].setY(y+w/2);
    this->pointHand[1].setX(x+h);
    this->pointHand[1].setY(y);
    this->pointHand[2].setX(x);
    this->pointHand[2].setY(y-w/2);
    this->pointHand[3].setX(x-5);
    this->pointHand[3].setY(y);
}
template<typename T>
void Hand<T>::setH(T h)
{
    this->h = h;
    setHand(w, h ,x, y);
}
template<typename T>
void Hand<T>::setW(T w)
{
    this->w = w;
    setHand(w, h ,x, y);
}
template<typename T>
void Hand<T>::setX(T x)
{
    this->x = x;
    setHand(w, h ,x, y);
}
template<typename T>
void Hand<T>::setY(T y)
{
    this->y = y;
    setHand(w, h ,x, y);
}

