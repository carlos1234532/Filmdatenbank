#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "qsqldatabase.h"
#include <model.h>
#include "controller.h"
#include "registerdia.h"

QT_BEGIN_NAMESPACE
namespace Ui { class login;}
QT_END_NAMESPACE

class login : public QDialog
{
    Q_OBJECT
public:
    explicit login(model* m,controller* c,QSqlDatabase* db,QWidget *parent = nullptr);

    ~login();

signals:
    void loginSuccessfull();

private slots:
    void loginRequest();
    void registerRequest();

private:
    Ui::login *ui;

    model* _model;
    registerdia * _registerdialog;
    QSqlDatabase* _database;

    void actions();
    void checkEmailInput();
    void leave();
};

#endif // LOGIN_H
