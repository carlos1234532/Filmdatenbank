#ifndef MODEL_H
#define MODEL_H

#include "qsqldatabase.h"
#include <QObject>
#include <QWidget>
#include "controller.h"

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

    void insertratingquery(int benutzerid,int filmid,QString bewertung,int grade,QSqlDatabase* db);
    void deleteratingquery(int benutzerid,QSqlDatabase* db);

private:

    controller* _controller;
};

#endif // MODEL_H
