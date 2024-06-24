#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "QString"
#include "QObject"
#include "movie.h"
#include "actor.h"
#include "provider.h"
#include "genre.h"
#include "user.h"
#include "grade.h"

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

    //provider object methods
    void addprovider(provider* provider);
    void clearprovidercache();
    QList<provider*> getprovidercache();
    bool existprovider(QString name);

    //genre object methods
    void addgenre(genre* genre);
    void cleargenrecache();
    QList<genre*> getgenrecache();
    bool existgenre(QString name);

    //user object methods
    void adduser(user* user);
    void clearusercache();
    QList<user*> getusercache();
    bool existuser(int userid);

    //grade object methods
    void addgrade(grade* grade);
    void cleargradecache();
    QList<grade*> getgradecache();
    bool existgrade(int gradevalue);
private:
    QList<movie*> _cache;
    QList<actor*> _actorcache;
    QList<provider*> _providercache;
    QList<genre*> _genrecache;
    QList<user*> _usercache;
    QList<grade*> _gradecache;
};

#endif // CONTROLLER_H
