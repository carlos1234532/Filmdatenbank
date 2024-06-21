#include "controller.h"

controller::controller()
{
}

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
