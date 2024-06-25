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

    void connectionbuild(QSqlDatabase* db);
    void customquery(QString Filmname, QSqlDatabase* db, QList<QString> *stringList);

    void preselectionquery(QString film,QSqlDatabase* db);
    void getfilmdataquery(QString filmname,QSqlDatabase* db);
    void getactorquery(QString filmname,QSqlDatabase* db);
    void getproviderquery(QString filmname,QSqlDatabase* db);
    void getgenrequery(QString filmname,QSqlDatabase* db);
    void getuserquery(QString filmname,QSqlDatabase* db);
    void overallratingquery(QString filmname,QSqlDatabase* db);
    void setcurrentuser(QString email,QSqlDatabase* db);

    void insertratingquery(int benutzerid,int filmid,QString bewertung,int grade,QSqlDatabase* db);
    void deleteratingquery(int benutzerid,QSqlDatabase* db);

    QString generatinghashedpassword(QString password,QSqlDatabase* db);
    void insertuserquery(int benutzerID,QString nachname,QString vorname,QString email,QString benutzername,
                         QString password,QSqlDatabase* db);
    bool checklogin(QString username, QString password,QSqlDatabase* db);

    int generaterandomuserid();
    QDir findpath(QString folder);

signals:
    void insertuserquerysuccessful();

private:

    controller* _controller;
};

#endif // MODEL_H
