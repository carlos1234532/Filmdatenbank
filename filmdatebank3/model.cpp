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

int model::generaterandomuserid()
{
    // Initialisiere den Zufallszahlengenerator mit der aktuellen Zeit
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generiere eine Zufallszahl zwischen 0 und 10.000 (da RAND_MAX mindestens 32.767 beträgt)
    int randomNumber = std::rand() % 10001;

    // Verschiebe die Zufallszahl in den Bereich von 10.000 bis 20.000
    int randomUserId = randomNumber + 10000;

    return randomUserId;
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

void model::overallratingquery(QString filmname,QSqlDatabase* db)
{
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

QString model::generatinghashedpassword(QString password,QSqlDatabase* db)
{
    QSqlQuery generatingpassword(*db);
    if(!generatingpassword.prepare("\
        SELECT gen_salt('bf',8);\
        ")){
        qDebug() <<"Prepare failed: " << generatingpassword.lastError().text();
        exit(1);
        }

    if(!generatingpassword.exec() ||!generatingpassword.next()){
        qDebug() << "Execute failed: "<< generatingpassword.lastError().text();
    }

    QString salt = generatingpassword.value(0).toString();
    qDebug() <<"salt: "<< salt;
    QString hashedpassword;

    if(!generatingpassword.prepare("\
        SELECT crypt(:password, :salt);\
        ")){
        qDebug() <<"Prepare failed: " << generatingpassword.lastError().text();
        exit(1);
        }


    generatingpassword.bindValue(":password", password);
    generatingpassword.bindValue(":salt", salt);

    if(generatingpassword.exec() && generatingpassword.next()) {
        hashedpassword = generatingpassword.value(0).toString();
    }
    else{
        qDebug() <<"Error hashing password: " <<generatingpassword.lastError();
        return "Error";
    }
    qDebug()<<"generatinghashedpassword finished"<< "hashwert: "<<hashedpassword;
    return hashedpassword;
}


void model::insertuserquery(int benutzerid,QString nachname,QString vorname,QString email,QString benutzername,QString password,QSqlDatabase* db)
{
    QString hashedpassword = generatinghashedpassword(password, db);
    if (hashedpassword == "Error") {
        qDebug() << "Error generating hashed password";
        return;
    }

    QSqlQuery insertuserdata(*db);
    if(!insertuserdata.prepare("\
        INSERT INTO benutzer(benutzerid,nachname,vorname,email,benutzername,password)\
        VALUES(:benutzerid,:nachname,:vorname,:email,:benutzername,:password)\
        ")){
        qDebug() <<"Prepare failed: " << insertuserdata.lastError().text();
        //exit(1);
        }

    insertuserdata.bindValue(":benutzerid", benutzerid);
    insertuserdata.bindValue(":nachname", nachname);
    insertuserdata.bindValue(":vorname", vorname);
    insertuserdata.bindValue(":email", email);
    insertuserdata.bindValue(":benutzername", benutzername);
    insertuserdata.bindValue(":password", hashedpassword);

    if (!insertuserdata.exec()){
       qDebug() << "Execute failed: "<< insertuserdata.lastError().text();
       //exit(1);
    }

    insertuserdata.finish();
    qDebug()<<"insertuserdata finished";

    emit insertuserquerysuccessful();
}

bool model::checklogin(QString email, QString password,QSqlDatabase* db)
{
    QSqlQuery checkuserdata(*db);
    if(!checkuserdata.prepare("\
        SELECT password FROM benutzer\
        WHERE email = :email\
        ")){
        qDebug() <<"Prepare failed: " << checkuserdata.lastError().text();
        //exit(1);
        }

    checkuserdata.bindValue(":email", email);

    if (!checkuserdata.exec() || !checkuserdata.next()) {
        qDebug() << "User not found or query error:" << checkuserdata.lastError();
        return false;
    }

    QString storedhash = checkuserdata.value(0).toString();

    if(!checkuserdata.prepare("\
        SELECT crypt(:plainpassword, :storedhash);\
        ")){
        qDebug() <<"Prepare failed: " << checkuserdata.lastError().text();
        //exit(1);
        }

    checkuserdata.bindValue(":plainpassword", password);
    checkuserdata.bindValue(":storedhash", storedhash);

    if (!checkuserdata.exec() || !checkuserdata.next()) {
        qDebug() << "Error checking password:" << checkuserdata.lastError();
        return false;
    }

    QString enteredHash = checkuserdata.value(0).toString();
    return storedhash == enteredHash;
}


void model::setcurrentuser(QString email,QSqlDatabase* db)
{
    QSqlQuery getuserdata(*db);
    if(!getuserdata.prepare("\
        Select benutzerid,vorname,nachname,benutzername\
        FROM benutzer\
        WHERE email = :email\
        ")){
        qDebug() <<"Prepare failed: " << getuserdata.lastError().text();
        exit(1);
        }
        getuserdata.bindValue(":email",email);

    if (!getuserdata.exec()){
    qDebug() << "Execute failed: "<< getuserdata.lastError().text();
    exit(1);
    }

    while (getuserdata.next()){
        user* u = new user(getuserdata.value(0).toInt(),getuserdata.value(1).toString(),
        getuserdata.value(2).toString(),email,getuserdata.value(3).toString());
        _controller->setcurrentuser(u);
    }
    getuserdata.finish();
    qDebug()<<"setcurrentuser finished";
}

void model::getfilmseriesquery(QString filmreihename,QSqlDatabase* db)
{
    QSqlQuery getfilmseriesdata(*db);
    if(!getfilmseriesdata.prepare("\
        Select film.name,film.url\
        FROM film\
        JOIN filmreihe ON film.filmreiheid = filmreihe.filmreiheid\
        WHERE filmreihe.name = :filmreihe\
        ")){
        qDebug() <<"Prepare failed: " << getfilmseriesdata.lastError().text();
        exit(1);
        }
        getfilmseriesdata.bindValue(":filmreihe",filmreihename);

    if (!getfilmseriesdata.exec()){
    qDebug() << "Execute failed: "<< getfilmseriesdata.lastError().text();
    exit(1);
    }

    while (getfilmseriesdata.next()){

        movie* m = new movie(getfilmseriesdata.value(0).toString(),getfilmseriesdata.value(1).toString());
        _controller->addmovie(m);

    }
    getfilmseriesdata.finish();
    qDebug()<<"getfilmseriesquery finished";
}

void model::getproviderofferquery(QString anbieter,QSqlDatabase* db)
{
    QSqlQuery getofferdata(*db);
    if(!getofferdata.prepare("\
        Select film.name,film.url\
        FROM film\
        JOIN filmanbieterzuordnung ON film.filmid = filmanbieterzuordnung.filmid\
        JOIN anbieter ON filmanbieterzuordnung.anbieterid = anbieter.anbieterid\
        WHERE anbieter.name = :anbieter\
        ")){
        qDebug() <<"Prepare failed: " << getofferdata.lastError().text();
        exit(1);
        }

    getofferdata.bindValue(":anbieter",anbieter);

    if (!getofferdata.exec()){
        qDebug() << "Execute failed: "<< getofferdata.lastError().text();
        exit(1);
    }

    while (getofferdata.next()){

        movie* m = new movie(getofferdata.value(0).toString(),getofferdata.value(1).toString());
        _controller->addmovie(m);
    }

    getofferdata.finish();
    qDebug()<<"getofferquery finished";
}

                                  /*
void model::getquery(QString filmreihename,QSqlDatabase* db)
{
    QSqlQuery getactordata(*db);
    if(!getactordata.prepare("\
        Select schauspieler.name,schauspieler.url\
        FROM schauspieler\
        JOIN
        ")){
        qDebug() <<"Prepare failed: " << getactordata.lastError().text();
        exit(1);
    }

    if (!getactordata.exec()){
    qDebug() << "Execute failed: "<< getactordata.lastError().text();
    exit(1);
    }

    while (getactordata.next()){


    }
    getactordata.finish();
    qDebug()<<"getallactorquery finished";
}
                              */
