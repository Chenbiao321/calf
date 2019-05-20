#include "musicplayer.h"
#include "ui_musicplayer.h"

#include <QFileDialog>
#include <QPainter>

musicplayer::musicplayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::musicplayer)
{
    ui->setupUi(this);
    this->setMaximumSize(820, 500);
    this->setMinimumSize(820, 500);

    player = new QMediaPlayer;
    playerlist = new QMediaPlaylist;
    QString str("Music");
    Download_database(str);
    player->setPlaylist(playerlist);
    iteming = new QListWidgetItem;
    QTimer timer;
    timer.start(100);

    connect(player, &QMediaPlayer::positionChanged, this, &musicplayer::updatePosition);
    connect(player, &QMediaPlayer::durationChanged, this, &musicplayer::updateDuration);
}

musicplayer::~musicplayer()
{
    delete ui;
}

void musicplayer::updatePosition(quint64 position)
{
    ui->horizontalSlider->setValue(position / 1000);//如果音乐进度改变，则改变滑块显示位置
    quint64 moved = position;//让播放时间等于音乐进度（毫秒）
    QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//设置时间
    ui->label->setText(moveTime.toString("mm:ss"));//显示时间
}

void musicplayer::updateDuration(quint64 duration)
{
    ui->listWidget->item(playerlist->currentIndex())->setSelected(true);
    ui->horizontalSlider->setMaximum(player->duration() / 1000);//设置滑块的长度范围为音乐长.00度
    ui->horizontalSlider->setPageStep(duration/10000);
    quint64 moved = duration;//让播放时间等于音乐进度（毫秒）
    QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//设置时间
    ui->label_5->setText(moveTime.toString("mm:ss"));//显示时间
    iteming->setTextColor(QColor(0, 0, 0));
    if(ui->listWidget->currentItem() != NULL)
        iteming = ui->listWidget->item(playerlist->currentIndex());
    iteming->setTextColor(QColor(150, 234, 0));
}

void musicplayer::Download_database(QString &str)
{
    QSqlQuery query;
    query.exec("select * from " + str);
    while(query.next())
    {
        qDebug() << query.value(1).toString();
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(query.value(1).toString());
        ui->listWidget->addItem(item);
        playerlist->addMedia(QUrl::fromLocalFile(query.value(2).toString()));

    }
}

void musicplayer::on_pushButton_5_clicked()
{
    QSqlQuery query;
    QString str = ui->lineEdit_2->text();
    QStringList list = QFileDialog::getOpenFileNames(this, QString("file"), str);

    if(!list.isEmpty())
        for(int i=0;i<list.size();i++)
        {
            QString path = QDir::toNativeSeparators(list.at(i));
            playerlist->addMedia(QUrl::fromLocalFile(path));
            QString Name=path.split("/").last();
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(Name);
            item->setToolTip(Name);
            ui->listWidget->addItem(item);
            query.exec(QString("insert into Music values(%1,'%2','%3')").arg(i).arg(Name).arg(path));
        }


}

void musicplayer::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text() == tr("play"))
    {
        this->player->play();
        ui->pushButton_2->setText(QString("pause"));
    }
    else
    {
        this->player->pause();
        ui->pushButton_2->setText(QString("play"));
    }

}

void musicplayer::on_pushButton_clicked()
{
    int count = playerlist->mediaCount();
    int dd = playerlist->currentIndex();
    if(++dd >= count)
    {
        dd = 0;
    }
    playerlist->setCurrentIndex(dd);
    ui->listWidget->item(playerlist->currentIndex())->setSelected(true);
    if(ui->pushButton_2->text() == tr("pause"))
    {
        player->play();
    }
}

void musicplayer::on_pushButton_3_clicked()
{
    int count = playerlist->mediaCount();
    int dd = playerlist->currentIndex();
    if(--dd < 0)
    {
        dd = count-1;
    }
    playerlist->setCurrentIndex(dd);
    ui->listWidget->item(playerlist->currentIndex())->setSelected(true);
    if(ui->pushButton_2->text() == tr("pause"))
    {
        player->play();
    }
}

void musicplayer::on_listWidget_currentRowChanged(int currentRow)
{
    ui->listWidget->item(currentRow)->setSelected(true);
}

void musicplayer::on_comboBox_activated(const QString &arg1)
{
    qDebug() << "11111111111";
    if(arg1 == tr("Loop"))
        playerlist->setPlaybackMode(QMediaPlaylist::Loop);
    else if(arg1 == tr("CurrentItemInLoop"))
        playerlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    else if(arg1 == tr("Random"))
        playerlist->setPlaybackMode(QMediaPlaylist::Random);
}

void musicplayer::on_listWidget_itemDoubleClicked()
{
    int row = ui->listWidget->currentRow();
    playerlist->setCurrentIndex(row);
    player->play();
    ui->pushButton_2->setText(tr("pause"));
}

void musicplayer::on_horizontalSlider_sliderReleased()
{
    int position = ui->horizontalSlider->value();
    player->setPosition(position * 1000);
}

void musicplayer::paintEvent(QPaintEvent *event)
{
    ui->pushButton->setFlat(true);
    ui->pushButton_2->setFlat(true);
    ui->pushButton_3->setFlat(true);
    ui->pushButton_4->setFlat(true);
    ui->pushButton_5->setFlat(true);


    QPainter paint(this);
    QPixmap backgound;
    backgound.load(":/5b405554a1596.jpg");
    backgound = backgound.scaled(820, 500);

    paint.drawPixmap(0, 0, backgound.width(), backgound.height(),backgound);
    event->accept();
}

