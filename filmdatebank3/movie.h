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
        QString rating,int grade,int release,QString producer,QString url,int filmid);
    movie(QString title);
    movie(int grade);

    movie(QString title,QString url);

    //getter
    QString getTitle() { return _title; }
    int getDuration() const { return _duration; }
    QString getDescription() const { return _description; }
    QString getRating() const { return _rating; }
    int getGrade() const { return _grade;}
    QString getGenre() { return _genre; }
    int getRelease() { return _release; }
    QString getProducer() { return _producer; }
    QString getUrl() { return _url; }
    int getMovieId() { return _filmid; }

    // setter
    void setTitle(QString newtitle) {_title = newtitle;}
    void setDuration(int newduration) {_duration = newduration;}
    void setDescription(QString newdescription) {_description = newdescription;}
    void setRating(QString newrating) {_rating = newrating;}
    void setGrade(int newgrade) {_grade = newgrade;}
    void setGenre(QString newgenre) {_genre = newgenre;}
    void setRelease(int newrealease) {_release = newrealease;}
    void setProducer(QString newproducer) {_producer = newproducer;}

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
    int _filmid;
};


#endif // MOVIE_H
