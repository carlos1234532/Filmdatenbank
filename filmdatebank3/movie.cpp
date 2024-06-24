#include "movie.h"

movie::movie(QString title, QString genre, const int duration, QString description, QString rating, int grade, int release,
             QString producer,QString url,int filmid)
    : _title(std::move(title)), _duration(duration), _description(std::move(description)), _rating(std::move(rating)),_grade(grade),
      _genre(std::move(genre)),_release(release),_producer(std::move(producer)),_url(std::move(url)),_filmid(filmid){
}

movie::movie(QString title)
    : _title(std::move(title)){

}
