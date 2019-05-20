#ifndef CREATDATABASE_H
#define CREATDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

static bool Createdatabase()
{
    QSqlDatabase music = QSqlDatabase::addDatabase("QSQLITE");
    music.setDatabaseName("my_music");
    if(!music.open())
    {
        QMessageBox::critical(0, "Cannot open database",
                              "unable to establish a database connection.",
                              QMessageBox::Cancel);
        return false;
    }
    QSqlQuery Query;
    if(Query.exec(QString("create table Music(id int primary key,"
                       "name varchar, path varchar)")))
    {
        qDebug() << "creat table success";
    }
    Query.exec(QString("select * from Music"));
    while(Query.next())
    {
        //qDebug() << Query.value(1).toString();
    }

    return true;
}

#endif // CREATDATABASE_H
