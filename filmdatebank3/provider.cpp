#include "provider.h"

provider::provider(QString name,QString url,int kaufpreis,int leihpreis)
    : _name(std::move(name)), _url(std::move(url)), _kaufpreis(kaufpreis), _leihpreis(leihpreis){
}

