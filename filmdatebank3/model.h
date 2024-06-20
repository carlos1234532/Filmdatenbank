#ifndef MODEL_H
#define MODEL_H

#include "qsqldatabase.h"
#include <QObject>
#include <QWidget>

class model: public QObject
{
    Q_OBJECT

public:
    model();

    void connectionbuild(QSqlDatabase* db);
    void customquery(QString Filmname, QSqlDatabase* db, QList<QString> *stringList);
};

#endif // MODEL_H
