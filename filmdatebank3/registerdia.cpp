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
    connect(ui->RegisterButton, &QPushButton::clicked, this,&registerdia::registerbuttonclicked);
    connect(_model, &model::insertuserquerysuccessful, this, &registerdia::insertsuccessful);
    connect(ui->BacktoLoginButton,&QPushButton::clicked,this, &registerdia::insertsuccessful);

    connect(ui->BenutzernameInput,&QLineEdit::textChanged, this, &registerdia::checkbenutzernameinput);
    connect(ui->NachnameInput,&QLineEdit::textChanged, this, &registerdia::checknachnameinput);
    connect(ui->vornameInput,&QLineEdit::textChanged, this, &registerdia::checkvornameinput);
    connect(ui->emailInput,&QLineEdit::textChanged, this, &registerdia::checkemailinput);

}

void registerdia::checkbenutzernameinput()
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

void registerdia::checknachnameinput()
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

void registerdia::checkemailinput()
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

void registerdia::checkvornameinput()
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


void registerdia::registerbuttonclicked()
{
    if(ui->NachnameInput->text() != "" && ui->vornameInput->text()!= ""&& ui->emailInput->text()!= "" &&
            ui->BenutzernameInput->text()!= "" && ui->passwordInput->text()!= ""){
    _model->insertuserquery(_model->generaterandomuserid(),ui->NachnameInput->text(),ui->vornameInput->text(),ui->emailInput->text(),
                            ui->BenutzernameInput->text(),ui->passwordInput->text(),_database);
    }
    else{
        qDebug()<<"Fehler";
    }
}

void registerdia::insertsuccessful()
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
