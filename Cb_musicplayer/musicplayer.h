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

protected slots:
    void updatePosition(quint64 position);
    void updateDuration(quint64 duration);
private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_comboBox_activated(const QString &arg1);

    void on_listWidget_itemDoubleClicked();

    void on_horizontalSlider_sliderReleased();

private:
    Ui::musicplayer *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playerlist;
    QListWidgetItem *iteming;

};

#endif // MUSICPLAYER_H
