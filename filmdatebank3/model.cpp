#include "model.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <controller.h>

model::model(controller* c)
{
    _controller = c;
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
/*
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

        //stringList->append(Name);
        //qDebug() <<"FilmName: " << Name;
    }

    getFilmreihe.finish();
    qDebug()<<"customquery finished";
}
*/
void model::preselectionquery(QString film,QSqlDatabase* db)
{
    QSqlQuery getfilmdata(*db);
    if(!getfilmdata.prepare("\
    SELECT *\
    FROM Film\
    WHERE Name like :filmname\
    ")) {
    qDebug() << "Prepare failed: " << getfilmdata.lastError().text();
    exit(1);
    }
    QString input = film +"%";
    getfilmdata.bindValue(":filmname", input);

    if (!getfilmdata.exec()){
        qDebug() << "Execute failed: " << getfilmdata.lastError().text();
        exit(1);
    }
    while (getfilmdata.next())
    {
        movie* m = new movie(getfilmdata.value(3).toString());
        _controller->addmovie(m);
    }
    getfilmdata.finish();
    qDebug()<<"selectionquery finished";
}

void model::getfilmdataquery(QString filmname,QSqlDatabase* db)
{
    QSqlQuery getfilmdata(*db);
    if(!getfilmdata.prepare("\
    SELECT *\
    FROM Film\
    WHERE Name = :filmname\
    ")) {
    qDebug() << "Prepare failed: " << getfilmdata.lastError().text();
    exit(1);
    }
    getfilmdata.bindValue(":filmname", filmname);

    if (!getfilmdata.exec()){
        qDebug() << "Execute failed: " << getfilmdata.lastError().text();
        exit(1);
    }

    while (getfilmdata.next())
    {
        movie* m = new movie(getfilmdata.value(3).toString(),0,getfilmdata.value(4).toInt(),getfilmdata.value(7).toString(),0
            ,0,getfilmdata.value(5).toInt(),getfilmdata.value(6).toString(),getfilmdata.value(2).toString());

        _controller->addmovie(m);
    }
    getfilmdata.finish();
    qDebug()<<"getfilmdataquery finished";
}

void model::getactorquery(QString filmname,QSqlDatabase* db)
{
    QSqlQuery getactordata(*db);
    if(!getactordata.prepare("\
    Select schauspieler.name,schauspieler.url\
    FROM Schauspieler\
    JOIN filmschauspielerzuordnung ON schauspieler.schauspielerid = filmschauspielerzuordnung.schauspielerid\
    JOIN film ON filmschauspielerzuordnung.filmid = film.filmid\
    WHERE film.name = :filmname\
    ")){
    qDebug() <<"Prepare failed: " << getactordata.lastError().text();
    exit(1);
    }
    getactordata.bindValue(":filmname",filmname);

    if (!getactordata.exec()){
       qDebug() << "Execute failed: "<< getactordata.lastError().text();
       exit(1);
    }

    while (getactordata.next())
    {
        qDebug() << getactordata.value(0).toString();

        actor* a = new actor(getactordata.value(0).toString(),getactordata.value(1).toString());
        _controller->addactor(a);
    }
    getactordata.finish();
    qDebug()<<"getactorquery finished";
}

void model::getproviderquery(QString filmname,QSqlDatabase* db)
{
    QSqlQuery getproviderdata(*db);
    if(!getproviderdata.prepare("\
    Select anbieter.name,anbieter.url\
    FROM anbieter\
    JOIN filmanbieterzuordnung ON anbieter.anbieterid = filmanbieterzuordnung.anbieterid\
    JOIN film ON filmanbieterzuordnung.filmid = film.filmid\
    WHERE film.name = :filmname\
    ")){
    qDebug() <<"Prepare failed: " << getproviderdata.lastError().text();
    exit(1);
    }
    getproviderdata.bindValue(":filmname",filmname);

    if (!getproviderdata.exec()){
    qDebug() << "Execute failed: "<< getproviderdata.lastError().text();
    exit(1);
    }

    while (getproviderdata.next())
    {
    qDebug() << getproviderdata.value(0).toString();

    provider* p = new provider(getactordata.value(0).toString(),getactordata.value(1).toString());
    _controller->addprovider(p);
    }
    getproviderdata.finish();
    qDebug()<<"getactorquery finished";


}
