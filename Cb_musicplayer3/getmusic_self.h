#ifndef GETMUSIC_SELF_H
#define GETMUSIC_SELF_H


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

class GetMusicself:public QObject
{
    Q_OBJECT

private:
    QString Website;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
signals:
    void Havegetmusicself();
public:
    GetMusicself();
    GetMusicself(const QString &str);
    ~GetMusicself();
    void SetWebsite(const QString &str);
    const QString &GetWebsite();
    void GetMusicMsg();
    void GetMusic_selfId();

    int id;
    QByteArray *allMsg;
    QMap<QString, quint64> *musicmap;

public slots:
    void readMsg();
    void finishMsg();
};

#endif // GETMUSIC_SELF_H
