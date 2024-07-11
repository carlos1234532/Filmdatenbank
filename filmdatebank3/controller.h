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
#include <QRegularExpressionValidator>>

class controller : public QObject
{
    Q_OBJECT
public:
    controller();

    //mobie object methods
    void addMovie(movie* movie);
    void clearCache();
    QList<movie*> getCache();
    bool existMovie(QString tittle);

    //actor object methods
    void addActor(actor* actor);
    void clearActorCache();
    QList<actor*> getActorCache();
    bool existActor(QString name);

    //provider object methods
    void addProvider(provider* provider);
    void clearProviderCache();
    QList<provider*> getProviderCache();
    bool existProvider(QString name);

    //genre object methods
    void addGenre(genre* genre);
    void clearGenreCache();
    QList<genre*> getGenreCache();
    bool existGenre(QString name);

    //user object methods
    void addUser(user* user);
    void clearUserCache();
    QList<user*> getUserCache();
    bool existUser(int userid);

    //grade object methods
    void addGrade(grade* grade);
    void clearGradeCache();
    QList<grade*> getGradeCache();
    bool existGrade(int gradevalue);

    //currentuser methoden
    //getter
    user* getCurrentUser() { return _currentuser; }
    user* setCurrentUser(user* u) { _currentuser = u; }

private:
    QList<movie*> _cache;
    QList<actor*> _actorcache;
    QList<provider*> _providercache;
    QList<genre*> _genrecache;
    QList<user*> _usercache;
    QList<grade*> _gradecache;

    user* _currentuser;
};

#endif // CONTROLLER_H
