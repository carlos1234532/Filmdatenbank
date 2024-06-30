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
    connect(ui->sucheButton,&QPushButton::clicked,this,&advancedsearch::startquery);
    connect(ui->leerenButton,&QPushButton::clicked,this,&advancedsearch::clear);
}

void advancedsearch::leave()
{
    this->close();
}

void advancedsearch::startquery()
{
    clear();
    if(ui->filmreiheCheckBox->isChecked() && !ui->schauspielerCheckBox->isChecked() && !ui->anbieterCheckBox->isChecked()){
        _model->getfilmseriesquery(ui->FilmnreiheInput->text(),_database);
        showdata();
    }
    else if (!ui->filmreiheCheckBox->isChecked() && ui->schauspielerCheckBox->isChecked() && !ui->anbieterCheckBox->isChecked()){
    }
    else if (!ui->filmreiheCheckBox->isChecked() && !ui->schauspielerCheckBox->isChecked() && ui->anbieterCheckBox->isChecked()){
        _model->getproviderofferquery(ui->AnbieterInput->text(),_database);
        showdata();
    }
}

void advancedsearch::clear()
{
    ui->Ausgabe->clear();

    _controller->clearcache();
}

//for filmseries and provider offer overview
void advancedsearch::showdata()
{
    const QList<movie*>& movies= _controller->getcache();
    for (movie* m : movies) {
        QString newEntry = "<p><b>" + m->gettitle() + "</b><br><img src='" + _model->findpath("filmcover").filePath(m->geturl()) + "' width='140' height='210'></p>";
        QString newText = ui->Ausgabe->toHtml() + newEntry;
        ui->Ausgabe->setHtml(newText);
    }
}
