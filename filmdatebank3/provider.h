#ifndef PROVIDER_H
#define PROVIDER_H


#include"QString"
#include"QObject"

class provider : public QObject
    {
        Q_OBJECT
public:
    provider(QString name,QString url,int kaufpreis,int leihpreis);

    QString getname() { return _name; }
    QString geturl() { return _url; }
    int getkaufpreis() { return _kaufpreis; }
    int getleihpreis() { return _leihpreis; }

private:
    QString _name;
    QString _url;
    int _kaufpreis;
    int _leihpreis;
};


#endif // PROVIDER_H
