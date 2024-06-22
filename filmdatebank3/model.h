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

private:

    controller* _controller;
};

#endif // MODEL_H
