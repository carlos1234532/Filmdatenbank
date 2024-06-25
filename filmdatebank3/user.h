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

    int getuserid() { return _userid; }
    QString getfirstname() { return _firstname; }
    QString getlastname() { return _lastname; }
    QString getemail() { return _email; }
    QString getrating() { return _rating; }
    int getgrade() { return _grade; }

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
