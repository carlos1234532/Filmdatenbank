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

//actor methods
void controller::addprovider(provider* provider) {
    if(!exists(provider->getname())) {
        _providercache.push_back(provider);
    }
}

void controller::clearprovidercache() {
    _providercache.clear();
}

QList<provider*> controller::getprovidercache() {
    return _providercache;
}

bool controller::existprovider(QString name){
for (provider* p : _providercache) {
    if (p->getname() == name) {
        return true;
        }
    }
        return false;
}

//genre methods
void controller::addgenre(genre* genre) {
    if(!exists(genre->getname())) {
        _genrecache.push_back(genre);
    }
}

void controller::cleargenrecache() {
    _genrecache.clear();
}

QList<genre*> controller::getgenrecache() {
    return _genrecache;
}

bool controller::existgenre(QString name){
for (genre* g : _genrecache) {
    if (g->getname() == name) {
        return true;
        }
    }
        return false;
}

//user methods
void controller::adduser(user* user) {
    _usercache.push_back(user);
}

void controller::clearusercache() {
    _usercache.clear();
}

QList<user*> controller::getusercache() {
    return _usercache;
}

bool controller::existuser(int userid){
for (user* u : _usercache) {
    if (u->getuserid() == userid) {
        return true;
        }
    }
        return false;
}
