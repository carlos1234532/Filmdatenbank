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
    connect(ui->sucheButton,&QPushButton::clicked,this,&advancedsearch::startQuery);
    connect(ui->leerenButton,&QPushButton::clicked,this,&advancedsearch::clear);
}

void advancedsearch::leave()
{
    this->close();
}

void advancedsearch::startQuery()
{
    clear();
    if(ui->filmreiheCheckBox->isChecked() && !ui->schauspielerCheckBox->isChecked() && !ui->anbieterCheckBox->isChecked()){
        _model->getFilmSeriesQuery(ui->FilmnreiheInput->text(),_database);
        showData();
    }
    else if (!ui->filmreiheCheckBox->isChecked() && ui->schauspielerCheckBox->isChecked() && !ui->anbieterCheckBox->isChecked()){
    }
    else if (!ui->filmreiheCheckBox->isChecked() && !ui->schauspielerCheckBox->isChecked() && ui->anbieterCheckBox->isChecked()){
        _model->getProviderOfferQuery(ui->AnbieterInput->text(),_database);
        showData();
    }
}

void advancedsearch::clear()
{
    ui->Ausgabe->clear();

    _controller->clearCache();
}

//for filmseries and provider offer overview
void advancedsearch::showData()
{
    const QList<movie*>& movies= _controller->getCache();
    for (movie* m : movies) {
        QString newEntry = "<p><b>" + m->getTitle() + "</b><br><img src='" + _model->findPath("filmcover").filePath(m->getUrl()) + "' width='140' height='210'></p>";
        QString newText = ui->Ausgabe->toHtml() + newEntry;
        ui->Ausgabe->setHtml(newText);
    }
}
