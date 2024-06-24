#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileDialog>
#include <QScrollArea>

MainWindow::MainWindow(model* m, controller* c , QSqlDatabase* db, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    deactivateview();
    _model = m;
    _controller = c;
    _database = db;

    setuplistwidget();

    //connectdb();
    actions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deactivateview()
{
    ui->listWidget->setVisible(false);
    ui->SchauspielerLabel->setVisible(false);
    ui->BewertungInput->setVisible(false);
    ui->bewertungButton->setVisible(false);
    ui->NoteInput->setVisible(false);
}

void MainWindow::loadcovers(QString fileName)
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString imagesPath = executablePath +"/filmcover/";

    QDir imageDir(imagesPath);

    QPixmap pixmap(imageDir.filePath(fileName));
    setpixmaptolabelsize(pixmap);
    ui->filmcover->setPixmap(pixmap);
    qDebug()<<"loadpictures beendet";
}

void MainWindow::setpixmaptolabelsize(QPixmap &pixmap)
{
    pixmap = pixmap.scaled(ui->filmcover->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void MainWindow::connectdb()
{
    qDebug() <<"MainWindow::connectDB wurde geöffnet";
    _model->connectionbuild(_database);
}

void MainWindow::actions()
{
    connect(ui->sucheButton, &QPushButton::clicked, this,&MainWindow::startquery);
    connect(ui->leerenButton, &QPushButton::clicked, this, &MainWindow::clear);
    connect(ui->FilmInput, &QLineEdit::textChanged, this ,&MainWindow::preselection);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::listitemclicked);
    connect(ui->bewertungButton, &QPushButton::clicked, this, &MainWindow::insertratingquery);
}

void MainWindow::insertratingquery()
{
    //Beutzerid die wir benutzen
    //10009
    qDebug() <<ui->NoteInput->text().toInt();
    if(!ui->BewertungInput->toPlainText().isEmpty() && (!ui->NoteInput->text().isEmpty() ||
        (ui->NoteInput->text().toInt() <= 6 && ui->NoteInput->text().toInt() >= 1))){
        int filmid;
        const QList<movie*>& movies= _controller->getcache();
        for (movie* m : movies) {
            filmid = m->getfilmid();
        }
        _model->insertratingquery(10009,filmid,ui->BewertungInput->toPlainText(),ui->NoteInput->text().toInt(),_database);
        _controller->clearusercache();
        ui->OutputBenutzerBewertung->setPlainText("");
        _model->getuserquery(ui->FilmInput->text(),_database);
        showuserdata();

        ui->BewertungInput->clear();
        ui->NoteInput->clear();
    }
}

void MainWindow::listitemclicked(QListWidgetItem *item)
{
    ui->FilmInput->setText(item->text());
    ui->listWidget->setVisible(false);
    updatefilminputborder(false);
}

void MainWindow::sizepreselectionwidget()
{
    int itemcount = ui->listWidget->count();
    int widgetHeight = itemcount * ui->listWidget->sizeHintForRow(0);

    qDebug() <<"itemcount: "<< itemcount << "höhe zeile 0: ",ui->listWidget->sizeHintForRow(0);

    ui->listWidget->resize(ui->listWidget->width(), widgetHeight);
}

void MainWindow::preselection()
{
    qDebug() <<"preselection started";
    QString film = ui->FilmInput->text();
    positionlistwidget();
    if(!film.isEmpty()){
        _model->preselectionquery(film,_database);

        if(ui->listWidget->count()){
            ui->listWidget->clear();
            ui->listWidget->setVisible(true);
            ui->FilmInput->setFocus();
            updatefilminputborder(true);
        }
        else{
            updatefilminputborder(false);
        }
        showpreselection();
        sizepreselectionwidget();
        _controller->clearcache();
    }
    else{
        ui->listWidget->clear();
        ui->listWidget->setVisible(false);
        updatefilminputborder(false);
    }
}

void MainWindow::showpreselection()
{
    const QList<movie*>& movies= _controller->getcache();
        for (movie* m : movies) {
            ui->listWidget->addItem(m->gettitle());;
        }
}

void MainWindow::setuplistwidget()
{
    ui->listWidget->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
}

void MainWindow::positionlistwidget()
{
    ui->listWidget->setFixedWidth(ui->FilmInput->width());
    QPoint pos = ui->FilmInput->mapToGlobal(QPoint(0, ui->FilmInput->height() - 8));
    ui->listWidget->move(pos);
}

void MainWindow::updatefilminputborder(bool showBottomBorder)
{
    QString styleSheet = ui->FilmInput->styleSheet();

    if (!showBottomBorder) {
        styleSheet.replace("border-bottom: none;","border-bottom: 2px solid rgb(37,39,48);");
        styleSheet.replace("border-bottom-left-radius: 0px;","border-bottom-left-radius: 10px;");
        styleSheet.replace("border-bottom-right-radius: 0px;","border-bottom-right-radius: 10px;");
    }
    else{
        styleSheet.replace("border-bottom: 2px solid rgb(37,39,48);","border-bottom: none;");
        styleSheet.replace("border-bottom-left-radius: 10px;","border-bottom-left-radius: 0px;");
        styleSheet.replace("border-bottom-right-radius: 10px;","border-bottom-right-radius: 0px;");
    }
    ui->FilmInput->setStyleSheet(styleSheet);
}

void MainWindow::showfilmdata()
{
    const QList<movie*>& movies= _controller->getcache();
    for (movie* m : movies) {
        ui->OutputName->setText(m->gettitle());
        ui->OutputDauer->setText("Dauer:\n"+QString::number(m->getduration())+" min");
        ui->OutputErscheinungsjahr->setText("Erscheinungsjahr:\n"+QString::number(m->getrelease()));
        ui->OutputProduzent->setText("Produzent:<br>"+m->getproducer());
        ui->OutputBeschreibung->setText(m->getdescription());

        loadcovers(m->geturl());
    }
}

void MainWindow::showactordata()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString imagesPath = executablePath +"/actors/";
    QDir imageDir(imagesPath);

    const QList<actor*>& actors = _controller->getactorcache();
    for (actor* a : actors) {

        QString newEntry = "<p><b>" + a->getname() + "</b><br><img src='" + imageDir.filePath(a->geturl()) + "' width='100' height='150'></p>";
        QString newText = ui->OutputSchauspieler->toHtml() + newEntry;
        ui->OutputSchauspieler->setHtml(newText);

    }
    ui->SchauspielerLabel->setVisible(true);
}

void MainWindow::showproviderdata()
{
    QString executablePath = QCoreApplication::applicationDirPath();
    QString imagesPath = executablePath +"/provider/";
    QDir imageDir(imagesPath);

    const QList<provider*>& providers = _controller->getprovidercache();
    for (provider* p : providers) {

        QString newEntry = "<p><b>" + p->getname() + "</b>" + "<br>"+"<img src='" + imageDir.filePath(p->geturl()) + "' width='75' height='112'>"
                "<br>"+"Kaufen: "+ QString::number(p->getkaufpreis()) +"Euro<br>"+ "Leihen: " + QString::number(p->getleihpreis())+"Euro"+"</p>";
        QString newText = ui->OutputAnbieter->toHtml() + newEntry;
        ui->OutputAnbieter->setHtml(newText);
    }
}

void MainWindow::showgenredata()
{
    QString newtext = "";
    const QList<genre*>& genres = _controller->getgenrecache();
    for (genre* g : genres) {
        newtext += (ui->OutputGenre->toPlainText()+g->getname() + "      ");
    }
    ui->OutputGenre->setText(newtext);
}

void MainWindow::showuserdata()
{
    QString imagepath = ":/rezensionen/";
    QString image ="user.jpg";
    QDir imageDir(imagepath);

    if (imageDir.exists()) {
        qDebug() << "Das Bild existiert.";
    } else {
        qDebug() << "Das Bild konnte nicht gefunden werden.";
    }

    const QList<user*>& users = _controller->getusercache();
    for (user* u : users) {
        QString newEntry = ("<img src='" + imageDir.filePath(image) + "' width='30' height='30'>" +"<b>   "+u->getfirstname()+
                    " "+u->getlastname() + "</b><br>" + u->getrating() + "<br><br>" + "Note: "+QString::number(u->getgrade())+ "<br><br><br>");
        QString newText = newEntry + ui->OutputBenutzerBewertung->toHtml();
        ui->OutputBenutzerBewertung->setHtml(newText);
    }
    ui->BewertungInput->setVisible(true);
    ui->bewertungButton->setVisible(true);
    ui->NoteInput->setVisible(true);
}

void MainWindow::clear()
{
    ui->OutputName->setText("");
    ui->OutputDauer->setText("");
    ui->OutputProduzent->setText("");
    ui->OutputErscheinungsjahr->setText("");
    ui->OutputBeschreibung->setText("");
    QPixmap pixmap = QPixmap();
    ui->filmcover->setPixmap(pixmap);
    ui->OutputSchauspieler->setText("");
    ui->SchauspielerLabel->setVisible(false);
    ui->listWidget->clear();
    ui->listWidget->setVisible(false);
    updatefilminputborder(false);
    ui->OutputAnbieter->setText("");
    ui->OutputGenre->setText("");
    ui->OutputBenutzerBewertung->setText("");
    deactivateview();

    _controller->clearcache();
    _controller->clearactorcache();
    _controller->clearprovidercache();
    _controller->cleargenrecache();
    _controller->clearusercache();
}
void MainWindow::startquery()
{
    qDebug() <<"suchebutton wurde geklickt";
    clear();

    QString film = ui->FilmInput->text();
    if(!film.isEmpty()){
    _model->getfilmdataquery(film,_database);
    showfilmdata();
    _model->getactorquery(film,_database);
    if(!_controller->getactorcache().isEmpty()){
        showactordata();
    }
    _model->getproviderquery(film,_database);
    showproviderdata();
    }
    _model->getgenrequery(film,_database);
    showgenredata();

    _model->getuserquery(film,_database);
    if(!_controller->getusercache().isEmpty()){
        showuserdata();
    }
}
    /*
    if(ui->erweitereSucheCheckBox->isChecked()){
        _model->customquery(ui->FilmInput->text(),_database,List);
        showresultsinwindow(List);
    }
    else{
        _model->getfilmdataquery(ui->FilmInput->text(),_database);

        showfilmdata();
    }
    */

