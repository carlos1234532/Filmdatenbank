#ifndef MOVIE_H
#define MOVIE_H

#include"QString"
#include"QObject"

class movie : public QObject
{
    Q_OBJECT
public:
    //Constructor
    movie(QString title,QString genre,int duration,QString description,
        QString rating,int grade,int release,QString producer,QString url);

    movie(QString title);

    //getter
    QString gettitle() { return _title; }
    int getduration() const { return _duration; }
    QString getdescription() const { return _description; }
    QString getrating() const { return _rating; }
    int getgrade() const { return _grade;}
    QString getgenre() { return _genre; }
    int getrelease() { return _release; }
    QString getproducer() { return _producer; }
    QString geturl() { return _url; }

    // setter
    void settitle(QString newtitle) {_title = newtitle;}
    void setduration(int newduration) {_duration = newduration;}
    void setdescription(QString newdescription) {_description = newdescription;}
    void setrating(QString newrating) {_rating = newrating;}
    void setgrade(int newgrade) {_grade = newgrade;}
    void setgenre(QString newgenre) {_genre = newgenre;}
    void setrelease(int newrealease) {_release = newrealease;}
    void setproducer(QString newproducer) {_producer = newproducer;}

private:
    QString _title;
    int _duration;
    QString _description;
    QString _rating;
    int _grade;
    QString _genre;
    int _release;
    QString _producer;
    QString _url;
};


#endif // MOVIE_H
