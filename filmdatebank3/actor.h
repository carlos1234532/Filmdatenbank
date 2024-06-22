#ifndef ACTOR_H
#define ACTOR_H

#include"QString"
#include"QObject"

class actor : public QObject
    {
        Q_OBJECT
public:
    actor(QString name,QString url);

    QString getname() { return _name; }
    QString geturl() { return _url; }

private:
    QString _name;
    QString _url;
};

#endif // ACTOR_H
