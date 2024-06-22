#include "provider.h"

provider::provider(QString name,QString url)
    : _name(std::move(name)), _url(std::move(url)){
}

