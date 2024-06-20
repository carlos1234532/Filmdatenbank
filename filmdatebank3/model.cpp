#include "model.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

model::model()
{
}

void model::connectionbuild(QSqlDatabase* db)
{
    // Datenbanktreiber laden
    db->setHostName("localhost");
    db->setPort(3033);
    db->setDatabaseName("abgabe");
    db->setUserName("carlos");
    db->setPassword("ForHonor40044!");

    // Verbindung zur Datenbank aufbauen
    if (!db->open()) {
        qDebug() << "Connection failed: " << db->lastError().text();
        exit(1);
    }
    else{
        qDebug() << "Connection successfull";
    }
}

void model::customquery(QString Filmreihe,QSqlDatabase* db,QList<QString>* stringList)
{
    qDebug() <<"customquery started";

    //Prepared Statement
    QSqlQuery getFilmreihe(*db);
    if(!getFilmreihe.prepare("\
        SELECT Film.Name AS FilmName\
        FROM Film\
        JOIN FilmReihe ON Film.FilmReiheID = FilmReihe.FilmReiheID\
        WHERE FilmReihe.Name = :filmreihename\
        ")) {
        qDebug() << "Prepare failed: " << getFilmreihe.lastError().text();
        exit(1);
    }
    getFilmreihe.bindValue(":filmreihename", Filmreihe);

    if (!getFilmreihe.exec()){
        qDebug() << "Execute failed: " << getFilmreihe.lastError().text();
        exit(1);
    }

    // Cursor durchlaufen
    while (getFilmreihe.next()) {
        QString Name = getFilmreihe.value(0).toString();

        stringList->append(Name);
        //qDebug() <<"FilmName: " << Name;
    }

    getFilmreihe.finish();
    qDebug()<<"customquery finished";
}
