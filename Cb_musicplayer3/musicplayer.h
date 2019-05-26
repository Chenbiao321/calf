#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QTime>
#include <QPaintEvent>
#include <QMediaPlaylist>
#include <QTimer>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QDebug>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QButtonGroup>
#include "getmusic_id.h"
#include "getmusic_cannel.h"
#include "getmusic_self.h"

namespace Ui {
class musicplayer;
}

class musicplayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit musicplayer(QWidget *parent = 0);
    ~musicplayer();
protected:
    void Download_database(QString &str);
    void paintEvent(QPaintEvent *event);
    void Mouserightpress();

protected slots:
    void updatePosition(quint64 position);
    void updateDuration(quint64 duration);
    void showlistmenu(QPoint point);
    void Del_Music();
    void CreatButtonGroup();
    void GetMusicId(QAbstractButton *button);
    void CreatButtonGroup2();
    void GetrealMusic(QAbstractButton *button);
    void Display();
    //void finishMsg();
private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_comboBox_activated(const QString &arg1);

    void on_listWidget_itemDoubleClicked();

    void on_horizontalSlider_sliderReleased();

    void on_verticalSlider_sliderMoved(int position);

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_listWidget_2_itemDoubleClicked(QListWidgetItem *item);
    
private:
    Ui::musicplayer *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playerlist;
    QListWidgetItem *iteming;
    QMenu * menu;
    QAction *del_action;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup2;
    GetmusicCannel *cannel;
    Getmusic_id *musicid;
    GetMusicself *musicself;

};

#endif // MUSICPLAYER_H
