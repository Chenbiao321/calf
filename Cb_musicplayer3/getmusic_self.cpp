#include "getmusic_self.h"

GetMusicself::GetMusicself()
{
    this->Website = "";
    this->id = 0;
}
GetMusicself::GetMusicself(const QString &str)
{
    this->Website = str;
    this->id = 0;
}
GetMusicself::~GetMusicself()
{
}
void GetMusicself::SetWebsite(const QString &str)
{
    this->Website = str;
}
const QString &GetMusicself::GetWebsite()
{
    return Website;
}
void GetMusicself::GetMusicMsg()
{
    QUrl url(this->Website);
    qDebug() << this->Website;
    manager = new QNetworkAccessManager;
    reply = manager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::readyRead,
            this, &GetMusicself::readMsg);
    connect(reply, &QNetworkReply::finished,
            this, &GetMusicself::finishMsg);

}

void GetMusicself::readMsg()
{
    qDebug() << "readyRead";
}

void GetMusicself::finishMsg()
{
    allMsg = new QByteArray;
    *allMsg = reply->readAll();
    GetMusic_selfId();
}

void GetMusicself::GetMusic_selfId()
{
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(*allMsg, &json_error));

    QJsonObject rootObj = jsonDoc.object();

    QStringList keys = rootObj.keys();
    for(int i = 0;i < keys.size();i++)
    {
        qDebug() << keys.at(i);
    }
    if(rootObj.contains("playlist"))
    {
        musicmap = new QMap<QString, quint64>;
        qDebug() << QString("1111111111");
        QJsonObject subObj = rootObj.value("playlist").toObject();
        if(subObj.contains("tracks"))
        {
            QJsonArray array = subObj.value("tracks").toArray();
            for(int i=0;i < array.size();i++)
            {
                QJsonObject subbObj = array.at(i).toObject();
                QString name = subbObj.value("name").toString();
                QVariant id1 = subbObj.value("id").toVariant();
                qDebug() << id1.toLongLong();
                musicmap->insert(name, id1.toLongLong());
            }
        }
    }
    emit Havegetmusicself();//发送信号，歌单列表以获取。
}

