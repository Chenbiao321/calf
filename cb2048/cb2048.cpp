#include "cb2048.h"
#include "ui_cb2048.h"
#include <QPaintEvent>

cb2048::cb2048(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cb2048)
{
    ui->setupUi(this);
    this->setWindowIcon(QPixmap(":/logo/res/logo.png"));
    this->setFixedSize(550, 380);
    this->setWindowIconText(QString("cb2048"));
    ui->textBrowser->setFontPointSize(16);
    ui->textBrowser->setStyleSheet("background-color: rgb(0, 255, 0)");

    CanGetKeyToMove = false;
    this->m_socre = 0;
    this->InitEachGrid();
}

cb2048::~cb2048()
{
    delete ui;
}

void cb2048::InitEachGrid()
{
    for(int row = 0;row < Coorx;row++)
        for(int col = 0;col < Coory;col++)
        {
            this->m_block[row][col] = 0;
        }
}

void cb2048::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.fillRect(QRect(10, 10, Mainwindow_Width, Mainwindow_Height),
                           QColor(187, 173, 160));
    for(int row = 0;row < Coorx;row++)
        for(int col = 0;col < Coory;col++)
        {
            if(this->m_block[row][col] == 0)
            {
                painter.fillRect(QRect(20+(Logo_Width + 10)*col,
                                       20+(Logo_Height + 10)*row,
                                       Logo_Width, Logo_Height),
                                       QColor(100, 193, 179));
            }
            else
            {
                painter.drawPixmap(QRect(20+(Logo_Width + 10)*col,
                                         20+(Logo_Height + 10)*row,
                                         Logo_Width, Logo_Height),
                                         QPixmap(GetBlockColor(m_block[row][col])));
            }
        }
}

QString cb2048::GetBlockColor(int number)
{
    switch (number)
    {
    case 2:
    case 4:
    case 8:
    case 16:
    case 32:
    case 64:
    case 128:
    case 256:
    case 612:
    case 1024:
    case 2048:
        return QString(":/logo/res/") + QString::number(number)
                + QString(".png");
    default:
        return ":/logo/res/4096.png";
    }
}

void cb2048::on_pushButton_clicked()
{
    CanGetKeyToMove = true;
    this->m_socre = 0;
    this->InitEachGrid();

    RandBlockNumber();

    ui->label_2->setText(QString("0"));
    ui->pushButton->setText(QString("restart"));
    ui->textBrowser->clear();

    update();
}

void cb2048::RandBlockNumber()
{
    int number = rand()%(Coorx*Coory);
    m_block[number/4][number%4] = (rand()%2+1)*2;
    int number1 = rand()%(Coorx*Coory);
    while(number1 == number)
    {
        number1 = rand()%(Coorx*Coory);
    }
    m_block[number1/4][number1%4] = (rand()%2+1)*2;

}


void cb2048::CalcBlocks(int dir)
{
    if(dir == DR_DOWN)
    {
        for(int col = 0;col < Coory;col++)
        {
            for(int i = Coorx-1;i > 0;i--)
                for(int row = Coory-1;row > Coory-1-i;row--)
                {
                        CalculateGrid(row, col, -1, 0);
                }
        }
    }
    else if(dir == DR_RIGHT)
    {
        for(int row = 0;row < Coorx;row++)
        {
            for(int i = Coory-1;i > 0;i--)
                for(int col = Coory-1;col > Coory-1-i;col--)
                {
                        CalculateGrid(row, col, 0, -1);
                }
        }
    }
    else if(dir == DR_UP)
    {
        for(int col = 0;col < Coory;col++)
        {
            for(int i = Coorx-1;i > 0;i--)
                for(int row = 0;row < i;row++)
                {
                   CalculateGrid(row, col, 1, 0);
                }
        }
    }
    else if(dir == DR_LEFT)
    {
        for(int row = 0;row < Coorx;row++)
        {
            for(int i = Coory-1;i > 0;i--)
                for(int col = 0;col < i;col++)
                {
                   CalculateGrid(row, col, 0, 1);
                }
        }
    }

}

void cb2048::CalculateGrid(int j, int k, int St1, int St2)
{
    if((m_block[j][k] == 0)&&(m_block[j+St1][k+St2] != 0))
    {
        int s = m_block[j][k];
        m_block[j][k] = m_block[j+St1][k+St2];
        m_block[j+St1][k+St2] = s;
    }
    else if((m_block[j][k] != 0)&&(m_block[j][k] == m_block[j+St1][k+St2]))
    {
        m_block[j][k]*=2;
        m_block[j+St1][k+St2] = 0;
    }
}

void cb2048::keyPressEvent(QKeyEvent *event)
{
    if(CanGetKeyToMove == false)
        return;
    switch(event->key())
    {
    case Qt::Key_W:
    {
        CalcBlocks(DR_UP);
    }
        break;
    case Qt::Key_S:
    {
        CalcBlocks(DR_DOWN);
    }
        break;
    case Qt::Key_A:
    {
        CalcBlocks(DR_LEFT);
    }
        break;
    case Qt::Key_D:
    {
        CalcBlocks(DR_RIGHT);
    }
        break;
    }

    InsertBlockNumber();

    update();
}

void cb2048::InsertBlockNumber()
{
    int score;
    QVector<int *> Gridvector;
    for(int row = 0;row < Coorx;row++)
        for(int col = 0;col < Coory;col++)
        {
            if(m_block[row][col] == 0)
                Gridvector.push_back(m_block[row] + col);
            else
            {
                score+=m_block[row][col]/2;
            }
        }
    ui->label_2->setText(QString::number(score));
    int leng = Gridvector.size();
    if(CheckBlockNumber(leng) == CS_LOSE)
    {
         ui->textBrowser->setText(QString("you are lose, please restart"));
         return;
    }
    int number = rand()%leng;
    *Gridvector[number] = (rand()%2+1)*2;

}

