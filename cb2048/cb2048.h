#ifndef CB2048_H
#define CB2048_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QList>


namespace Ui {
class cb2048;
}

#define Mainwindow_Height 350
#define Mainwindow_Width 350
#define Logo_Height 75
#define Logo_Width 75
#define Victiry 2048
#define Coorx 4
#define Coory 4

class cb2048 : public QMainWindow
{
    Q_OBJECT

public:
    explicit cb2048(QWidget *parent = 0);
    ~cb2048();
protected:
    enum CheckStatus
    {
        CS_WIN,
        CS_LOSE,
        CS_NULL
    };
    enum Direction
    {
        DR_UP,
        DR_DOWN,
        DR_LEFT,
        DR_RIGHT
    };

    virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    Ui::cb2048 *ui;

    int m_block[Coorx][Coory];
    bool CanGetKeyToMove;
    int m_socre;

    void InitEachGrid();
    int GetEmptyGridNumber();
    void InsertBlockNumber();
    void RandBlockNumber();
    void CalculateGrid(int j, int k, int St1, int St2);
    QString GetBlockColor(int number);
    CheckStatus CheckBlockNumber(int EmptyGrid)
    {
        if(EmptyGrid == 0)
        {
            CanGetKeyToMove = false;
            return CS_LOSE;
        }
        return CS_NULL;
    }
    void CalcBlocks(int dir);
};

#endif // CB2048_H
