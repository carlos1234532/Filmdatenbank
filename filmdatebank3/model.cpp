#include "model.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <controller.h>


//Balkendiagramm
#include <QtCharts/QHorizontalBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>

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

QDir model::findpath(QString imagesPath)
{
    QString executablePath = QCoreApplication::applicationDirPath();
    //qDebug()<<"ApplicationPath: " << executablePath;
    //QString imagesPath = executablePath ;

    QDir imageDir(executablePath);
    imageDir.cdUp();
    imageDir.cdUp();
    imageDir.cd(imagesPath);
    return imageDir;
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
    SELECT film.name\
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
        movie* m = new movie(getfilmdata.value(0).toString());
        _controller->addmovie(m);
    }
    getfilmdata.finish();
    qDebug()<<"selectionquery finished";
}

void model::getfilmdataquery(QString filmname,QSqlDatabase* db)
{
    QSqlQuery getfilmdata(*db);
    if(!getfilmdata.prepare("\
    SELECT filmid,filmreiheid,url,name,dauer,erscheinungsjahr,produzent,beschreibung\
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
            ,0,getfilmdata.value(5).toInt(),getfilmdata.value(6).toString(),getfilmdata.value(2).toString(),
            getfilmdata.value(0).toInt());

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
    Select anbieter.name,anbieter.url,filmanbieterzuordnung.kaufpreis,filmanbieterzuordnung.leihpreis\
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

    provider* p = new provider(getproviderdata.value(0).toString(),getproviderdata.value(1).toString(),
                               getproviderdata.value(2).toInt(),getproviderdata.value(3).toInt());
    _controller->addprovider(p);
    }
    getproviderdata.finish();
    qDebug()<<"getproviderquery finished";
}

void model::getgenrequery(QString filmname,QSqlDatabase* db)
{
    QSqlQuery getgenredata(*db);
    if(!getgenredata.prepare("\
    Select genre.name\
    FROM genre\
    JOIN filmgenrezuordnung ON genre.genreid = filmgenrezuordnung.genreid\
    JOIN film ON filmgenrezuordnung.filmid = film.filmid\
    WHERE film.name = :filmname\
    ")){
    qDebug() <<"Prepare failed: " << getgenredata.lastError().text();
    exit(1);
    }
    getgenredata.bindValue(":filmname",filmname);

    if (!getgenredata.exec()){
    qDebug() << "Execute failed: "<< getgenredata.lastError().text();
    exit(1);
    }

    while (getgenredata.next())
    {
    qDebug() << getgenredata.value(0).toString();

    genre* g = new genre(getgenredata.value(0).toString());
    _controller->addgenre(g);
    }
    getgenredata.finish();
    qDebug()<<"getproviderquery finished";
}

void model::getuserquery(QString filmname,QSqlDatabase* db)
{
    QSqlQuery getuserdata(*db);
    if(!getuserdata.prepare("\
    Select benutzer.vorname,benutzer.nachname,benutzerfilmzuordnung.bewertung,benutzerfilmzuordnung.note\
    FROM benutzer\
    JOIN benutzerfilmzuordnung ON benutzer.benutzerid = benutzerfilmzuordnung.benutzerid\
    JOIN film ON benutzerfilmzuordnung.filmid = film.filmid\
    WHERE film.name = :filmname\
    ")){
    qDebug() <<"Prepare failed: " << getuserdata.lastError().text();
    exit(1);
    }
    getuserdata.bindValue(":filmname",filmname);

    if (!getuserdata.exec()){
        qDebug() << "Execute failed: "<< getuserdata.lastError().text();
        exit(1);
    }

    while (getuserdata.next())
    {
    qDebug() << getuserdata.value(0).toString();
    qDebug() << getuserdata.value(1).toString();
    qDebug() << getuserdata.value(2).toString();
    qDebug() << getuserdata.value(3).toString();

     user* u = new user(getuserdata.value(0).toString(),getuserdata.value(1).toString(),
                        getuserdata.value(2).toString(),getuserdata.value(3).toInt());
     _controller->adduser(u);
     }
     getuserdata.finish();
     qDebug()<<"getuserquery finished";
}

void model::insertratingquery(int benutzerid,int filmid,QString bewertung,int grade,QSqlDatabase* db)
{
    deleteratingquery(benutzerid,db);

    QSqlQuery insertratingdata(*db);
    if(!insertratingdata.prepare("\
        INSERT INTO benutzerfilmzuordnung(benutzerid,filmid,bewertung,note)\
        VALUES(:benutzerid,:filmid,:bewertung,:grade)\
        ")){
        qDebug() <<"Prepare failed: " << insertratingdata.lastError().text();
        exit(1);
    }

    insertratingdata.bindValue(":benutzerid", benutzerid);
    insertratingdata.bindValue(":filmid", filmid);
    insertratingdata.bindValue(":bewertung", bewertung);
    insertratingdata.bindValue(":grade", grade);

    if (!insertratingdata.exec()){
        qDebug() << "Execute failed: "<< insertratingdata.lastError().text();
        //exit(1);
    }

    insertratingdata.finish();
    qDebug()<<"insertratingdata finished";
}

void model::deleteratingquery(int benutzerid,QSqlDatabase* db)
{
    QSqlQuery deleteratingdata(*db);
    if(!deleteratingdata.prepare("\
        DELETE FROM benutzerfilmzuordnung\
        WHERE benutzerfilmzuordnung.benutzerid = :benutzerid\
        ")){
        qDebug() <<"Prepare failed: " << deleteratingdata.lastError().text();
        exit(1);
    }

    deleteratingdata.bindValue(":benutzerid", benutzerid);

    if (!deleteratingdata.exec()){
        qDebug() << "Execute failed: "<< deleteratingdata.lastError().text();
        exit(1);
    }

    deleteratingdata.finish();
    qDebug()<<"deleteratingdata finished";
}

void model::overallratingquery(QString filmname,QSqlDatabase* db){
    QSqlQuery getgradedata(*db);
    if(!getgradedata.prepare("\
        WITH NoteCounts AS (SELECT n.note AS Note, COUNT(b.note) AS NoteCount\
        FROM (SELECT 1 AS note UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6) AS n\
        LEFT JOIN benutzerfilmzuordnung b ON n.note = b.note\
        JOIN film f ON b.filmid = f.filmid WHERE f.name = :filmname\
        GROUP BY n.note),\
        TotalCount AS (SELECT COUNT(*) AS TotalNoteCount\
        FROM benutzerfilmzuordnung b\
        JOIN film f ON b.filmid = f.filmid\
        WHERE f.name = :filmname)\
        SELECT ROUND((COALESCE(nc.NoteCount, 0) * 100.0 / tc.TotalNoteCount), 1) AS ProzentualerAnteil\
        FROM (SELECT 1 AS note UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6) AS n\
        LEFT JOIN NoteCounts nc ON n.note = nc.Note\
        CROSS JOIN TotalCount tc\
        ORDER BY n.note\
        ")){
        qDebug() <<"Prepare failed: " << getgradedata.lastError().text();
        exit(1);
        }
        getgradedata.bindValue(":filmname",filmname);

    if (!getgradedata.exec()){
        qDebug() << "Execute failed: "<< getgradedata.lastError().text();
    }

    while (getgradedata.next())
    {
    qDebug() << getgradedata.value(0).toInt();

    grade* g = new grade(getgradedata.value(0).toInt());
    _controller->addgrade(g);
    }
    getgradedata.finish();
    qDebug()<<"getgradequery finished";
}
