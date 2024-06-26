#include "advancedsearch.h"
#include "ui_advancedsearch.h"

advancedsearch::advancedsearch(model* m,controller* c,QSqlDatabase* db,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::advancedsearch)
{
    ui->setupUi(this);

    _model = m;
    _database = db;
    _controller = c;

    actions();
}

advancedsearch::~advancedsearch()
{
    delete ui;
}

void advancedsearch::actions()
{
    connect(ui->verlassenButton,&QPushButton::clicked,this, &advancedsearch::leave);
}

void advancedsearch::leave()
{
    this->close();
}
