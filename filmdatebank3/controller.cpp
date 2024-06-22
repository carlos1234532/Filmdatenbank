#include "controller.h"

controller::controller()
{
}
//movie methods
void controller::addmovie(movie* movie) {
    if(!exists(movie->gettitle())) {
        _cache.push_back(movie);
    }
}

void controller::clearcache() {
    _cache.clear();
}

QList<movie*> controller::getcache() {
    return _cache;
}

bool controller::exists(QString title){
for (movie* m : _cache) {
    if (m->gettitle() == title) {
        return true;
        }
    }
        return false;
}

//actor methods
void controller::addactor(actor* actor) {
    if(!exists(actor->getname())) {
        _actorcache.push_back(actor);
    }
}

void controller::clearactorcache() {
    _actorcache.clear();
}

QList<actor*> controller::getactorcache() {
    return _actorcache;
}

bool controller::existsactor(QString name){
for (actor* a : _actorcache) {
    if (a->getname() == name) {
        return true;
        }
    }
        return false;
}
