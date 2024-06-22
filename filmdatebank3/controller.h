#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "QString"
#include "QObject"
#include "movie.h"
#include "actor.h"

class controller : public QObject
{
    Q_OBJECT
public:
    controller();

    //mobie object methods
    void addmovie(movie* movie);
    void clearcache();
    QList<movie*> getcache();
    bool exists(QString tittle);

    //actor object methods
    void addactor(actor* actor);
    void clearactorcache();
    QList<actor*> getactorcache();
    bool existsactor(QString name);


private:
    QList<movie*> _cache;

    QList<actor*> _actorcache;
};

#endif // CONTROLLER_H
