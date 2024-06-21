#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "QString"
#include "QObject"
#include "movie.h"

class controller : public QObject
{
    Q_OBJECT
public:
    controller();

    void addmovie(movie* movie);
    void clearcache();
    QList<movie*> getcache();

    bool exists(QString tittle);

private:
    QList<movie*> _cache;
};

#endif // CONTROLLER_H
