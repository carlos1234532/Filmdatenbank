#include "user.h"

user::user(int userid,QString firstname,QString lastname,QString email,QString benutzername)
   : _userid(userid),_firstname(std::move(firstname)), _lastname(std::move(lastname)), _email(std::move(email)),
     _benutzername(std::move(benutzername))
   {
}

user::user(QString firstname,QString lastname,QString rating,int grade)
   : _firstname(std::move(firstname)), _lastname(std::move(lastname))
   , _rating(std::move(rating)), _grade(grade){
}
