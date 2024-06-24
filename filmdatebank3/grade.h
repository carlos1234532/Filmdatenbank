#ifndef GRADE_H
#define GRADE_H

#include"QString"
#include"QObject"

class grade : public QObject
    {
        Q_OBJECT
public:
    grade(int grade);

    int getgrade() { return _grade; }

private:
    int _grade;
};
#endif // GRADE_H
