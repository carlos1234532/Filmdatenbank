#ifndef PROVIDER_H
#define PROVIDER_H


#include"QString"
#include"QObject"

class provider : public QObject
    {
        Q_OBJECT
public:
    provider(QString name,QString url);

    QString getname() { return _name; }
    QString geturl() { return _url; }

private:
    QString _name;
    QString _url;
};


#endif // PROVIDER_H
