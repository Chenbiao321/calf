#include "getmusic_id.h"

Getmusic_id::Getmusic_id()
{
    this->Website = "";
    this->id = 0;
}
Getmusic_id::Getmusic_id(const QString &str)
{
    this->Website = str;
    this->id = 0;
}
Getmusic_id::~Getmusic_id()
{
}
void Getmusic_id::SetWebsite(const QString &str)
{
    this->Website = str;
}
const QString &Getmusic_id::GetWebsite()
{
    return Website;
}
void Getmusic_id::GetMusicMsg()
{
    QUrl url(this->Website);
    qDebug() << this->Website;
    manager = new QNetworkAccessManager;
    reply = manager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::readyRead,
            this, &Getmusic_id::readMsg);
    connect(reply, &QNetworkReply::finished,
            this, &Getmusic_id::finishMsg);

}

void Getmusic_id::readMsg()
{
    qDebug() << "readyRead";
}

void Getmusic_id::finishMsg()
{
    allMsg = new QByteArray;
    *allMsg = reply->readAll();
    emit Havegetmusic();
    //GetMusic_Id();
}

void Getmusic_id::GetMusic_Id()
{
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(*allMsg, &json_error));

    QJsonObject rootObj = jsonDoc.object();

    QStringList keys = rootObj.keys();
    for(int i = 0;i < keys.size();i++)
    {
        qDebug() << keys.at(i);
    }
//    if(rootObj.contains("sub"))
//    {
//        CannelList = new QStringList;
//        qDebug() << QString("1111111111");
//        QJsonArray array = rootObj.value("sub").toArray();
//        for(int i=0;i < array.size();i++)
//        {
//            QString name = (array.at(i).toObject().value("name").toString());
//            CannelList->append(name);
//        }
//    }
    //emit finishedsignal();//发送信号，歌单列表以获取。
}

