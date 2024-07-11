#ifndef PROVIDER_H
#define PROVIDER_H


#include"QString"
#include"QObject"

class provider : public QObject
    {
        Q_OBJECT
public:
    provider(QString name,QString url,int kaufpreis,int leihpreis);

    QString getName() { return _name; }
    QString getUrl() { return _url; }
    int getPurchasePrice() { return _kaufpreis; }
    int getRentanlPrice() { return _leihpreis; }

private:
    QString _name;
    QString _url;
    int _kaufpreis;
    int _leihpreis;
};


#endif // PROVIDER_H
