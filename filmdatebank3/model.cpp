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
        movie* m = new movie(getfilmdata.value(2).toString(),0,getfilmdata.value(3).toInt(),getfilmdata.value(6).toString(),0
            ,0,getfilmdata.value(4).toInt(),getfilmdata.value(5).toString());

        _controller->addmovie(m);
    }
    getfilmdata.finish();
    qDebug()<<"selectionquery finished";
}

void model::getfilmdataquery(QString filmname,QSqlDatabase* db)
{
    //Prepared Statement
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

    // Cursor durchlaufen

    //controller.clearcache();
    while (getfilmdata.next())
    {
        movie* m = new movie(getfilmdata.value(2).toString(),0,getfilmdata.value(3).toInt(),getfilmdata.value(6).toString(),0
                         ,0,getfilmdata.value(4).toInt(),getfilmdata.value(5).toString());

        _controller->addmovie(m);
    }
    getfilmdata.finish();
    qDebug()<<"getfilmdataquery finished";

    //signal..
}

/*
void model::getdescriptionQuery(QString Filminput,QSqlDatabase* db)
{
    //Prepared Statement
    QSqlQuery getfilmdescription(*db);
    if(!getfilmdescription.prepare("\
    SELECT *\
    FROM Film\
    WHERE Name = :filmname\
    ")) {
    qDebug() << "Prepare failed: " << getfilmdescription.lastError().text();
    exit(1);
    }
    getfilmdescription.bindValue(":filmname", Filminput);

    if (!getfilmdescription.exec()){
        qDebug() << "Execute failed: " << getfilmdescription.lastError().text();
        exit(1);
    }

    // Cursor durchlaufen
    while (getfilmdescription.next()) {
        QString Name = getfilmdescription.value(0).toString();
        QString Beschreibung = getfilmdescription.value(1).toString();

        Datamodel data(name,description,dauer,leihbar,kaufbar);
        stringList->append(Name);
        //qDebug() <<"FilmName: " << Name;
    }

    getfilmdescription.finish();
    qDebug()<<"customquery finished";
}
                         */
