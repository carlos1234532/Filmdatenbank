#ifndef MODEL_H
#define MODEL_H

#include "qsqldatabase.h"
#include <QObject>
#include <QWidget>
#include "controller.h"
#include "qapplication.h"
#include "qdir.h"

class model: public QObject
{
    Q_OBJECT
public:
    model(controller* c);

    void connectionBuild(QSqlDatabase* db);
    void customQuery(QString Filmname, QSqlDatabase* db, QList<QString> *stringList);

    void preselectionQuery(QString film,QSqlDatabase* db);
    void getFilmDataQuery(QString filmname,QSqlDatabase* db);
    void getActorQuery(QString filmname,QSqlDatabase* db);
    void getProviderQuery(QString filmname,QSqlDatabase* db);
    void getGenreQuery(QString filmname,QSqlDatabase* db);
    void getUserQuery(QString filmname,QSqlDatabase* db);
    void overallRatingQuery(QString filmname,QSqlDatabase* db);
    void setCurrentUser(QString email,QSqlDatabase* db);
    void getFilmSeriesQuery(QString filmreihename,QSqlDatabase* db);
    void getProviderOfferQuery(QString anbieter,QSqlDatabase* db);

    void insertRatingQuery(int benutzerid,int filmid,QString bewertung,int grade,QSqlDatabase* db);
    void deleteRatingQuery(int benutzerid,QSqlDatabase* db);

    QString generatingHashedPassword(QString password,QSqlDatabase* db);
    void insertUserQuery(int benutzerID,QString nachname,QString vorname,QString email,QString benutzername,
                         QString password,QSqlDatabase* db);
    bool checkLogin(QString username, QString password,QSqlDatabase* db);

    int generateRandomUserID();
    QDir findPath(QString folder);

signals:
    void insertUserQuerySuccessful();

private:

    controller* _controller;
};

#endif // MODEL_H
