#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPainter>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

template<typename T>
class Hand
{
public:
    Hand() = default;
    Hand(T w, T h, T x, T y);
    ~Hand()
    {

    }
    T getW();
    T getH();
    T getX();
    T getY();
    virtual void drawhand(QPainter &painter) = 0;

private:
    T w;
    T h;
    T x;
    T y;

protected:
    void setW(T w);
    void setH(T h);
    void setX(T x);
    void setY(T y);
    void setHand(T w, T h, T x, T y);
    QPoint pointHand[4];

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;


protected:
    void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOW_H
