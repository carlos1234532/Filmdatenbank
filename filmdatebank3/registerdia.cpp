#include "registerdia.h"
#include "ui_registerdia.h"

registerdia::registerdia(model* m,QSqlDatabase* db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerdia)
{
    ui->setupUi(this);

    _model = m;
    _database = db;
    actions();

    ui->passwordInput->setEchoMode(QLineEdit::Password);
}

registerdia::~registerdia()
{
    delete ui;
}

void registerdia::actions()
{
    connect(ui->RegisterButton, &QPushButton::clicked, this,&registerdia::registerButtonClicked);
    connect(_model, &model::insertUserQuerySuccessful, this, &registerdia::insertSuccessful);
    connect(ui->BacktoLoginButton,&QPushButton::clicked,this, &registerdia::insertSuccessful);

    connect(ui->BenutzernameInput,&QLineEdit::textChanged, this, &registerdia::checkBenutzernameInput);
    connect(ui->NachnameInput,&QLineEdit::textChanged, this, &registerdia::checkNachnameInput);
    connect(ui->vornameInput,&QLineEdit::textChanged, this, &registerdia::checkVornameInput);
    connect(ui->emailInput,&QLineEdit::textChanged, this, &registerdia::checkEmailInput);

}

void registerdia::checkBenutzernameInput()
{
    QRegularExpression regex("^[a-zA-Z0-9._]{3,20}$");
    QValidator *validator = new QRegularExpressionValidator(regex, ui->BenutzernameInput);
    ui->BenutzernameInput->setValidator(validator);

    QString input = ui->BenutzernameInput->text();
    int start = 0;
    if (validator->validate(input, start) != QValidator::Acceptable) {
        qDebug() << "Valid input!";
    }
}

void registerdia::checkNachnameInput()
{
    QRegularExpression regex("^[a-zA-Z]+$");
    QValidator *validator = new QRegularExpressionValidator(regex, ui->NachnameInput);
    ui->NachnameInput->setValidator(validator);

    QString input = ui->NachnameInput->text();
    int start = 0;
    if (validator->validate(input, start) != QValidator::Acceptable) {
        qDebug() << "Valid input!";
    }
}

void registerdia::checkEmailInput()
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

void registerdia::checkVornameInput()
{
    QRegularExpression regex("^[a-zA-Z]+$");
    QValidator *validator = new QRegularExpressionValidator(regex, ui->vornameInput);
    ui->vornameInput->setValidator(validator);

    QString input = ui->vornameInput->text();
    int start = 0;
    if (validator->validate(input, start) != QValidator::Acceptable) {
        qDebug() << "Valid input!";
    }
}


void registerdia::registerButtonClicked()
{
    if(ui->NachnameInput->text() != "" && ui->vornameInput->text()!= ""&& ui->emailInput->text()!= "" &&
            ui->BenutzernameInput->text()!= "" && ui->passwordInput->text()!= ""){
    _model->insertUserQuery(_model->generateRandomUserID(),ui->NachnameInput->text(),ui->vornameInput->text(),ui->emailInput->text(),
                            ui->BenutzernameInput->text(),ui->passwordInput->text(),_database);
    }
    else{
        qDebug()<<"Fehler";
    }
}

void registerdia::insertSuccessful()
{
    clear();
    emit registrationSuccessful();
    this->close();
}

void registerdia::clear()
{
    ui->BenutzernameInput->clear();
    ui->NachnameInput->clear();
    ui->emailInput->clear();
    ui->vornameInput->clear();
    ui->passwordInput->clear();
}
