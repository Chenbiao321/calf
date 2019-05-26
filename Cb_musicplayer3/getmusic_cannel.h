#ifndef GETMUSICCANNEL_H
#define GETMUSICCANNEL_H

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

class GetmusicCannel:public QObject
{
    Q_OBJECT

private:
    QString Website;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
signals:
    void finishedsignal();

public:
    GetmusicCannel();
    GetmusicCannel(QString &str);
    ~GetmusicCannel();
    void GetMusicMsg();
    void GetCannelName();
    void SetWebsite(const QString &str);
    const QString &GetWebsite();
    QByteArray *allMsg;
    QStringList *CannelList;
public slots:
    void readMsg();
    void finishMsg();

};

#endif // GETMUSICCANNEL_H
