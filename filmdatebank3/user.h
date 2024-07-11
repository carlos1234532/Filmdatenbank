#ifndef USER_H
#define USER_H

#include"QString"
#include"QObject"

class user : public QObject
    {
        Q_OBJECT
public:
    user(int userid,QString name,QString lastname,QString email,QString benutzername);
    user(QString firstname,QString lastname,QString rating,int grade);

    int getUserid() { return _userid; }
    QString getFirstname() { return _firstname; }
    QString getLastname() { return _lastname; }
    QString getEmail() { return _email; }
    QString getRating() { return _rating; }
    int getGrade() { return _grade; }

private:
    int _userid;
    QString _firstname;
    QString _lastname;
    QString _email;
    QString _benutzername;
    QString _rating;
    int _grade;
};
#endif // USER_H
