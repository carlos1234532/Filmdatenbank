#include "controller.h"

controller::controller()
{
}
//movie methods
void controller::addMovie(movie* movie) {
    if(!existMovie(movie->getTitle())) {
        _cache.push_back(movie);
    }
}

void controller::clearCache() {
    _cache.clear();
}

QList<movie*> controller::getCache() {
    return _cache;
}

bool controller::existMovie(QString title){
for (movie* m : _cache) {
    if (m->getTitle() == title) {
        return true;
        }
    }
        return false;
}

//actor methods
void controller::addActor(actor* actor) {
    if(!existActor(actor->getName())) {
        _actorcache.push_back(actor);
    }
}

void controller::clearActorCache() {
    _actorcache.clear();
}

QList<actor*> controller::getActorCache() {
    return _actorcache;
}

bool controller::existActor(QString name){
for (actor* a : _actorcache) {
    if (a->getName() == name) {
        return true;
        }
    }
        return false;
}

//actor methods
void controller::addProvider(provider* provider) {
    if(!existProvider(provider->getName())) {
        _providercache.push_back(provider);
    }
}

void controller::clearProviderCache() {
    _providercache.clear();
}

QList<provider*> controller::getProviderCache() {
    return _providercache;
}

bool controller::existProvider(QString name){
for (provider* p : _providercache) {
    if (p->getName() == name) {
        return true;
        }
    }
        return false;
}

//genre methods
void controller::addGenre(genre* genre) {
    if(!existGenre(genre->getName())) {
        _genrecache.push_back(genre);
    }
}

void controller::clearGenreCache() {
    _genrecache.clear();
}

QList<genre*> controller::getGenreCache() {
    return _genrecache;
}

bool controller::existGenre(QString name){
for (genre* g : _genrecache) {
    if (g->getName() == name) {
        return true;
        }
    }
        return false;
}

//user methods
void controller::addUser(user* user) {
    _usercache.push_back(user);
}

void controller::clearUserCache() {
    _usercache.clear();
}

QList<user*> controller::getUserCache() {
    return _usercache;
}

bool controller::existUser(int userid){
for (user* u : _usercache) {
    if (u->getUserid() == userid) {
        return true;
        }
    }
        return false;
}

//grade methods
void controller::addGrade(grade* grade) {
    _gradecache.push_back(grade);
}

void controller::clearGradeCache() {
    _gradecache.clear();
}

QList<grade*> controller::getGradeCache() {
    return _gradecache;
}

bool controller::existGrade(int gradevalue){
for (grade* g : _gradecache) {
    if (g->getGrade() == gradevalue) {
        return true;
        }
    }
        return false;
}

