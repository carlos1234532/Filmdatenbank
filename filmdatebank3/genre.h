#ifndef GENRE_H
#define GENRE_H

#include"QString"
#include"QObject"

class genre : public QObject
    {
        Q_OBJECT
public:
    genre(QString name);

    QString getname() { return _name; }

private:
    QString _name;
};
#endif // GENRE_H
