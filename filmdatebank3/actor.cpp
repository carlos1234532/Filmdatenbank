#include "actor.h"

actor::actor(QString name,QString url)
    : _name(std::move(name)), _url(std::move(url)){
}
