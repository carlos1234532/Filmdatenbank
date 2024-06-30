#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileDialog>
#include <QScrollArea>

//qchart test
#include <QtCharts/QChartView>
#include <QtCharts/QHorizontalBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>

MainWindow::MainWindow(model* m, controller* c , QSqlDatabase* db, QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    _advancedsearch(new class advancedsearch(m,c,db,this))
{
    ui->setupUi(this);

    deactivateview();
    _model = m;
    _controller = c;
    _database = db;

    setuplistwidget();
    actions();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _advancedsearch;
}

void MainWindow::showdoverallratingdiagramm()
{
    deletechartcontainer();

    QList<int> values;
    const QList<grade*>& grades = _controller->getgradecache();
    for (grade* g : grades) {

        values << g->getgrade();
    }
    QStringList categories;
    categories << "Note 1" << "Note 2" << "Note 3" << "Note 4" << "Note 5" << "Note 6";

    QBarSet *b = new QBarSet("Werte");
        for (int value : values) {
            *b << value;
        }

    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    series->append(b);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisY = new QBarCategoryAxis();
    axisY->append(categories);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->hide();

    QFont labelsFont;
    labelsFont.setPointSize(5);
    axisY->setLabelsFont(labelsFont);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(ui->chartcontainer);
    layout->addWidget(chartView);
    ui->chartcontainer->setLayout(layout);

    ui->Kundenrezensionen->setVisible(true);
    qDebug()<<"creatediagramm() finished";
}

void MainWindow::deactivateview()
{
    ui->listWidget->setVisible(false);
    //ui->SchauspielerLabel->setVisible(false);
    ui->Kundenrezensionen->setVisible(false);
    ui->BewertungInput->setVisible(false);
    ui->bewertungButton->setVisible(false);
    ui->NoteInput->setVisible(false);
}

void MainWindow::loadcovers(QString fileName)
{
    QPixmap pixmap(_model->findpath("filmcover").filePath(fileName));
    setpixmaptolabelsize(pixmap);
    ui->filmcover->setPixmap(pixmap);
    qDebug()<<"loadpictures beendet";
}

void MainWindow::setpixmaptolabelsize(QPixmap &pixmap)
{
    pixmap = pixmap.scaled(ui->filmcover->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void MainWindow::actions()
{
    connect(ui->sucheButton, &QPushButton::clicked, this,&MainWindow::startquery);
    connect(ui->leerenButton, &QPushButton::clicked, this, &MainWindow::clear);
    connect(ui->FilmInput, &QLineEdit::textChanged, this ,&MainWindow::preselection);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::listitemclicked);
    connect(ui->bewertungButton, &QPushButton::clicked, this, &MainWindow::insertratingquery);

    connect(ui->NoteInput,&QLineEdit::textChanged, this, &MainWindow::checknoteinput);
    connect(ui->backtoLoginButton,&QPushButton::clicked, this, &MainWindow::backtologinwindow);

    connect(ui->erweitereSucheCheckBox, &QCheckBox::clicked, this, &MainWindow::advancedsearch);
}

void MainWindow::advancedsearch()
{
    qDebug()<<"advancedsearch started";

    ui->erweitereSucheCheckBox->setChecked(false);
    clear();
    _advancedsearch->show();
}

void MainWindow::backtologinwindow()
{
    clear();
    emit logoutsuccesfull();
    this->close();
}

void MainWindow::checknoteinput()
{
    QRegularExpression regex("^[1-6]$");
    QValidator *validator = new QRegularExpressionValidator(regex, ui->NoteInput);
    ui->NoteInput->setValidator(validator);

    QString input = ui->NoteInput->text();
    int start = 0;
    if (validator->validate(input, start) != QValidator::Acceptable) {
        qDebug() << "Invalid input!";
    }
}

void MainWindow::insertratingquery()
{
    if(!ui->BewertungInput->toPlainText().isEmpty() && (!ui->NoteInput->text().isEmpty() ||
        (ui->NoteInput->text().toInt() <= 6 && ui->NoteInput->text().toInt() >= 1))){
        int filmid;
        const QList<movie*>& movies = _controller->getcache();
        for (movie* m : movies) {
            filmid = m->getfilmid();
        }
        _model->insertratingquery(_controller->getcurrentuser()->getuserid(),filmid,ui->BewertungInput->toPlainText(),ui->NoteInput->text().toInt(),_database);

        _controller->clearusercache();
        ui->OutputBenutzerBewertung->setPlainText("");

        _model->getuserquery(ui->FilmInput->text(),_database);
        showuserdata();

        _controller->cleargradecache();
        _model->overallratingquery(ui->FilmInput->text(),_database);
        showdoverallratingdiagramm();

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
    const QList<actor*>& actors = _controller->getactorcache();
    for (actor* a : actors) {

        QString newEntry = "<p><b>" + a->getname() + "</b><br><img src='" + _model->findpath("actors").filePath(a->geturl()) + "' width='140' height='210'></p>";
        QString newText = ui->OutputSchauspieler->toHtml() + newEntry;
        ui->OutputSchauspieler->setHtml(newText);

    }
    //ui->SchauspielerLabel->setVisible(true);
}

void MainWindow::showproviderdata()
{
    const QList<provider*>& providers = _controller->getprovidercache();
    for (provider* p : providers) {

        QString newEntry = "<p><b>" + p->getname() + "</b>" + "<br>"+"<img src='" + _model->findpath("provider").filePath(p->geturl()) + "' width='110' height='130'>"
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
    //ui->SchauspielerLabel->setVisible(false);
    ui->Kundenrezensionen->setVisible(false);
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
    _controller->cleargradecache();

    deletechartcontainer();
}

void MainWindow::deletechartcontainer()
{

    QWidget *chartcontainer = ui->chartcontainer;
    QVBoxLayout *layout = dynamic_cast<QVBoxLayout*>(chartcontainer->layout());

    if (layout) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        chartcontainer->setLayout(nullptr);
        delete layout;
    }
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
    _model->overallratingquery(film,_database);
    if(!_controller->getgradecache().isEmpty()){
        showdoverallratingdiagramm();
    }
    //darf nicht wird für insertquery benötigt
    //ui->FilmInput->clear();
}


