#include "qwidget.h"
#include "login.h"
#include "ui_login.h"

login::login(model* m, controller* c , QSqlDatabase* db,QWidget *parent)
    : QDialog(parent),
    ui(new Ui::login),
    _registerdialog(new registerdia(m,db,this))
{
    ui->setupUi(this);

    ui->PasswordInput->setEchoMode(QLineEdit::Password);

    _model = m;
    _database = db;

    actions();
}

login::~login()
{
    delete ui;
    delete _registerdialog;
}

void login::actions()
{
    connect(ui->LogInButton, &QPushButton::clicked, this,&login::loginRequest);
    connect(ui->RegisterButton, &QPushButton::clicked, this,&login::registerRequest);
    connect(ui->emailInput,&QLineEdit::textChanged, this, &login::checkEmailInput);

    connect(ui->verlassenButton,&QPushButton::clicked, this, &login::leave);
}
void login::leave(){
    this->close();
}

void login::checkEmailInput()
{
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    QValidator *validator = new QRegularExpressionValidator(regex, ui->emailInput);
    ui->emailInput->setValidator(validator);

    QString input = ui->emailInput->text();
    int start = 0;
    if (validator->validate(input, start) != QValidator::Acceptable) {
        qDebug() << "Valid input!";
    }
}

void login::loginRequest()
{
    qDebug() <<"loginrequest() started";

    if((ui->emailInput->text() != "" ||ui->PasswordInput->text() != "") && _model->checkLogin(ui->emailInput->text(),ui->PasswordInput->text(),_database)){
        _model->setCurrentUser(ui->emailInput->text(),_database);
        ui->emailInput->clear();
        ui->PasswordInput->clear();
        emit loginSuccessfull();
        accept();
    }
    else{
        qDebug() <<"Fehler";
    }
}

void login::registerRequest()
{
    qDebug() <<"registerrequest() started";

    _registerdialog->show();
}
