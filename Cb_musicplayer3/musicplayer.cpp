#include "musicplayer.h"
#include "ui_musicplayer.h"

#include <QFileDialog>
#include <QPainter>

musicplayer::musicplayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::musicplayer)
{
    ui->setupUi(this);
    this->setMaximumSize(830, 530);
    this->setMinimumSize(830, 530);

    Mouserightpress();
    player = new QMediaPlayer;
    buttonGroup = new QButtonGroup;
    buttonGroup2 = new QButtonGroup;
    this->cannel = new GetmusicCannel;
    this->musicid = new Getmusic_id;
    this->musicself = new GetMusicself;
    playerlist = new QMediaPlaylist;
    QString str("Music");
    Download_database(str);
    player->setPlaylist(playerlist);
    iteming = new QListWidgetItem;
    QTimer timer;
    timer.start(100);

    connect(cannel, &GetmusicCannel::finishedsignal, this, &musicplayer::CreatButtonGroup);
    connect(musicid, &Getmusic_id::Havegetmusic, this, &musicplayer::CreatButtonGroup2);
    connect(musicself, &GetMusicself::Havegetmusicself, this, &musicplayer::Display);
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
    iteming = ui->listWidget->item(playerlist->currentIndex());
    iteming->setTextColor(QColor(150, 234, 0));
    ui->label_3->setText(iteming->text());
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
    QStringList list = QFileDialog::getOpenFileNames(this, QString("file"), str, tr("(*.mp3)"));

    query.exec(QString("select id from Music"));
    int count;
    if(query.last())
        count = query.value(0).toInt()+1;
    else
        count = 0;
    qDebug() << count;
    if(!list.isEmpty())
        for(int i=0;i<list.size();i++)
        {
            QString path = QDir::toNativeSeparators(list.at(i));
            playerlist->addMedia(QUrl::fromLocalFile(path));
            QString Name=path.split("/").last();
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(Name);
            ui->listWidget->addItem(item);
            query.exec(QString("insert into Music values(%1,'%2','%3')").arg(count).arg(Name).arg(path));
            ++count;
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
    backgound = backgound.scaled(830, 530);

    paint.drawPixmap(0, 0, backgound.width(), backgound.height(),backgound);
    event->accept();
}

void musicplayer::Mouserightpress()
{
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    menu = new QMenu;
    del_action = new QAction("del music", this);
    menu->addAction(del_action);

    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(showlistmenu(QPoint)));
    connect(this->del_action, SIGNAL(triggered(bool)),
            SLOT(Del_Music()));

}

void musicplayer::showlistmenu(QPoint point)
{
    qDebug() << point;
    menu->exec(QCursor::pos());
}

void musicplayer::Del_Music()
{
    QSqlQuery query;
    int row = ui->listWidget->currentRow();
    if(row == 0)
        return;
    QListWidgetItem * item = ui->listWidget->takeItem(row);
    delete item;
    playerlist->removeMedia(row);

    query.prepare(QString("delete from Music where id=?"));
    query.addBindValue(row);
    query.exec();
    query.exec(QString("update Music set id=id-1 where id > ?"));
    query.addBindValue(row);
    query.exec();

}


void musicplayer::on_verticalSlider_sliderMoved(int position)
{
    player->setVolume(position);
}

void musicplayer::on_pushButton_4_clicked()
{
    QString str = ui->lineEdit->text();
    for(int i = 0;i < ui->listWidget->count();i++)
    {
        if(ui->listWidget->item(i)->text() == str)
        {
             ui->listWidget->item(i)->setSelected(true);
             break;
        }
    }
}

void musicplayer::on_pushButton_6_clicked()
{
    QString str = ui->lineEdit_3->text();

    cannel->SetWebsite(str);
    cannel->GetMusicMsg();
}

void musicplayer::CreatButtonGroup()//创建按钮组
{
    int row = cannel->CannelList->size()/10;
    int col = cannel->CannelList->size()%10;
    for(int i=0;i < row;i++)
        for(int j=0;j < 10;j++)
        {
            QPushButton *button = new QPushButton;
            button->setText(cannel->CannelList->at(i*10+j));
            buttonGroup->addButton(button);
            ui->gridLayout->addWidget(button, i, j);
        }
    for(int i=0;i < col;i++)
    {
        QPushButton *button = new QPushButton;
        button->setText(cannel->CannelList->at(row*10+i));
        buttonGroup->addButton(button);
        ui->gridLayout->addWidget(button, row, i);
    }
    connect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(GetMusicId(QAbstractButton *)));
}

void musicplayer::GetMusicId(QAbstractButton* button)
{
    QString str = button->text();
    QString Web = tr("http://musicapi.leanapp.cn/top/playlist?tag=") + str;
    musicid->SetWebsite(Web);

    musicid->GetMusicMsg();
}

void musicplayer::CreatButtonGroup2()//创建按钮组
{
    ui->stackedWidget->setCurrentIndex(1);
    int row = musicid->musicmap->size()/7;
    int col = musicid->musicmap->size()%7;
    QMap<QString, quint64>::iterator tt = musicid->musicmap->begin();
    for(int i=0;i < row;i++)
        for(int j=0;j < 7;j++)
        {
            QPushButton *button = new QPushButton;
            button->setText(tt.key());
            buttonGroup2->addButton(button);
            ui->gridLayout_2->addWidget(button, i, j);
            ++tt;
        }
    for(int i=0;i < col;i++)
    {
        QPushButton *button = new QPushButton;
        button->setText(tt.key());
        buttonGroup2->addButton(button);
        ui->gridLayout_2->addWidget(button, row, i);
        ++tt;
    }
    connect(buttonGroup2, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(GetrealMusic(QAbstractButton *)));
}

void musicplayer::GetrealMusic(QAbstractButton* button)
{
    quint64 id = musicid->musicmap->value(button->text());
    QString Web = tr("http://musicapi.leanapp.cn/playlist/detail?id=") + QString::number(id);
    musicself->SetWebsite(Web);

    musicself->GetMusicMsg();
    qDebug() << Web;
}

void musicplayer::Display()
{
    //qDebug() << *(musicself->allMsg);
    ui->stackedWidget->setCurrentIndex(2);
    QMap<QString, quint64>::iterator tt = musicself->musicmap->begin();
    for(int i=0;i < musicself->musicmap->size();i++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        ui->listWidget_2->addItem(item);
        item->setText(tt.key());
        ++tt;
    }
}

void musicplayer::on_listWidget_2_itemDoubleClicked(QListWidgetItem *item)
{
    //int row = ui->listWidget->currentRow();
    QString Web = tr(" https://music.163.com/song/media/outer/url?id=") + QString::number(musicself->musicmap->value(item->text()));
    //QString Web = tr("https://m8.music.126.net/20190524212140/c96f8b8fe8c1b5ec936a0fe0000875e3/ymusic/020f/075f/5653/5e255ec8980530db6eeea953d35e2475.mp3");
    qDebug() << Web;
    QUrl url(Web);
    player->setMedia(url);
    player->play();
//    manager = new QNetworkAccessManager;
//    reply = manager->get(QNetworkRequest(url));
//    connect(reply, &QNetworkReply::finished,
//            this, &musicplayer::finishMsg);
}

//void musicplayer::finishMsg()
//{
//    QByteArray all = reply->readAll();
//    qDebug() << all;
//}
