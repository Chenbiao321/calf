#ifndef GETMUSIC_ID_H
#define GETMUSIC_ID_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QUrl>
#include <QObject>
#include <QTextBrowser>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>

class Getmusic_id:public QObject
{
    Q_OBJECT

private:
    QString Website;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
signals:
    void Havegetmusic();
public:
    Getmusic_id();
    Getmusic_id(const QString &str);
    ~Getmusic_id();
    void SetWebsite(const QString &str);
    const QString &GetWebsite();
    void GetMusicMsg();
    void GetMusic_Id();

    int id;
    QByteArray *allMsg;
    QMap<QString, quint64> *musicmap;

public slots:
    void readMsg();
    void finishMsg();
};

#endif // GETMUSIC_ID_H
