#include "getmusic_cannel.h"

void GetmusicCannel::SetWebsite(const QString &str)
{
    this->Website = str;
}

const QString &GetmusicCannel::GetWebsite()
{
    return this->Website;
}
GetmusicCannel::GetmusicCannel()
{
    this->Website = QString("");
}
GetmusicCannel::GetmusicCannel(QString &str)
{
    this->Website = str;
}
GetmusicCannel::~GetmusicCannel()
{
}
void GetmusicCannel::GetMusicMsg()
{
    QUrl url(this->Website);
    qDebug() << this->Website;
    manager = new QNetworkAccessManager;
    reply = manager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::readyRead,
            this, &GetmusicCannel::readMsg);
    connect(reply, &QNetworkReply::finished,
            this, &GetmusicCannel::finishMsg);

}

void GetmusicCannel::readMsg()
{
    qDebug() << "readyRead";
}

void GetmusicCannel::finishMsg()
{
    allMsg = new QByteArray;
    *allMsg = reply->readAll();

    GetCannelName();
}

void GetmusicCannel::GetCannelName()
{
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(*allMsg, &json_error));

    QJsonObject rootObj = jsonDoc.object();

    QStringList keys = rootObj.keys();
    for(int i = 0;i < keys.size();i++)
    {
        qDebug() << keys.at(i);
    }
    if(rootObj.contains("sub"))
    {
        CannelList = new QStringList;
        qDebug() << QString("1111111111");
        QJsonArray array = rootObj.value("sub").toArray();
        for(int i=0;i < array.size();i++)
        {
            QString name = (array.at(i).toObject().value("name").toString());
            CannelList->append(name);
        }
    }
    emit finishedsignal();//发送信号，歌单列表以获取。
}
