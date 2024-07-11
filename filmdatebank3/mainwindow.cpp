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

    deactivateView();
    _model = m;
    _controller = c;
    _database = db;

    setupListWidget();
    actions();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _advancedsearch;
}

void MainWindow::showOverallRatingDiagramm()
{
    deleteChartContainer();

    QList<int> values;
    const QList<grade*>& grades = _controller->getGradeCache();
    for (grade* g : grades) {

        values << g->getGrade();
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

void MainWindow::deactivateView()
{
    ui->listWidget->setVisible(false);
    //ui->SchauspielerLabel->setVisible(false);
    ui->Kundenrezensionen->setVisible(false);
    ui->BewertungInput->setVisible(false);
    ui->bewertungButton->setVisible(false);
    ui->NoteInput->setVisible(false);
}

void MainWindow::loadCovers(QString fileName)
{
    QPixmap pixmap(_model->findPath("filmcover").filePath(fileName));
    setPixmapToLabelSize(pixmap);
    ui->filmcover->setPixmap(pixmap);
    qDebug()<<"loadpictures beendet";
}

void MainWindow::setPixmapToLabelSize(QPixmap &pixmap)
{
    pixmap = pixmap.scaled(ui->filmcover->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void MainWindow::actions()
{
    connect(ui->sucheButton, &QPushButton::clicked, this,&MainWindow::startQuery);
    connect(ui->leerenButton, &QPushButton::clicked, this, &MainWindow::clear);
    connect(ui->FilmInput, &QLineEdit::textChanged, this ,&MainWindow::preselection);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::listItemClicked);
    connect(ui->bewertungButton, &QPushButton::clicked, this, &MainWindow::insertRatingQuery);

    connect(ui->NoteInput,&QLineEdit::textChanged, this, &MainWindow::checkNoteInput);
    connect(ui->backtoLoginButton,&QPushButton::clicked, this, &MainWindow::backToLoginWindow);

    connect(ui->erweitereSucheCheckBox, &QCheckBox::clicked, this, &MainWindow::advancedSearch);
}

void MainWindow::advancedSearch()
{
    qDebug()<<"advancedsearch started";

    ui->erweitereSucheCheckBox->setChecked(false);
    clear();
    _advancedsearch->show();
}

void MainWindow::backToLoginWindow()
{
    clear();
    emit logoutSuccesfull();
    this->close();
}

void MainWindow::checkNoteInput()
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

void MainWindow::insertRatingQuery()
{
    if(!ui->BewertungInput->toPlainText().isEmpty() && (!ui->NoteInput->text().isEmpty() ||
        (ui->NoteInput->text().toInt() <= 6 && ui->NoteInput->text().toInt() >= 1))){
        int filmid;
        const QList<movie*>& movies = _controller->getCache();
        for (movie* m : movies) {
            filmid = m->getMovieId();
        }
        _model->insertRatingQuery(_controller->getCurrentUser()->getUserid(),filmid,ui->BewertungInput->toPlainText(),ui->NoteInput->text().toInt(),_database);

        _controller->clearUserCache();
        ui->OutputBenutzerBewertung->setPlainText("");

        _model->getUserQuery(ui->FilmInput->text(),_database);
        showUserData();

        _controller->clearGradeCache();
        _model->overallRatingQuery(ui->FilmInput->text(),_database);
        showOverallRatingDiagramm();

        ui->BewertungInput->clear();
        ui->NoteInput->clear();
    }
}

void MainWindow::listItemClicked(QListWidgetItem *item)
{
    ui->FilmInput->setText(item->text());
    ui->listWidget->setVisible(false);
    updateFilmInputBorder(false);
}

void MainWindow::sizePreselectionWidget()
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
    positionListWidget();
    if(!film.isEmpty()){
        _model->preselectionQuery(film,_database);

        if(ui->listWidget->count()){
            ui->listWidget->clear();
            ui->listWidget->setVisible(true);
            ui->FilmInput->setFocus();
            updateFilmInputBorder(true);
        }
        else{
            updateFilmInputBorder(false);
        }
        showPreselection();
        sizePreselectionWidget();
        _controller->clearCache();
    }
    else{
        ui->listWidget->clear();
        ui->listWidget->setVisible(false);
        updateFilmInputBorder(false);
    }
}

void MainWindow::showPreselection()
{
    const QList<movie*>& movies= _controller->getCache();
        for (movie* m : movies) {
            ui->listWidget->addItem(m->getTitle());;
        }
}

void MainWindow::setupListWidget()
{
    ui->listWidget->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
}

void MainWindow::positionListWidget()
{
    ui->listWidget->setFixedWidth(ui->FilmInput->width());
    QPoint pos = ui->FilmInput->mapToGlobal(QPoint(0, ui->FilmInput->height() - 8));
    ui->listWidget->move(pos);
}

void MainWindow::updateFilmInputBorder(bool showBottomBorder)
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

void MainWindow::showFilmData()
{
    const QList<movie*>& movies= _controller->getCache();
    for (movie* m : movies) {
        ui->OutputName->setText(m->getTitle());
        ui->OutputDauer->setText("Dauer:\n"+QString::number(m->getDuration())+" min");
        ui->OutputErscheinungsjahr->setText("Erscheinungsjahr:\n"+QString::number(m->getRelease()));
        ui->OutputProduzent->setText("Produzent:<br>"+m->getProducer());
        ui->OutputBeschreibung->setText(m->getDescription());

        loadCovers(m->getUrl());
    }
}

void MainWindow::showActorData()
{
    const QList<actor*>& actors = _controller->getActorCache();
    for (actor* a : actors) {

        QString newEntry = "<p><b>" + a->getName() + "</b><br><img src='" + _model->findPath("actors").filePath(a->getUrl()) + "' width='140' height='210'></p>";
        QString newText = ui->OutputSchauspieler->toHtml() + newEntry;
        ui->OutputSchauspieler->setHtml(newText);

    }
    //ui->SchauspielerLabel->setVisible(true);
}

void MainWindow::showProviderData()
{
    const QList<provider*>& providers = _controller->getProviderCache();
    for (provider* p : providers) {

        QString newEntry = "<p><b>" + p->getName() + "</b>" + "<br>"+"<img src='" + _model->findPath("provider").filePath(p->getUrl()) + "' width='110' height='130'>"
                "<br>"+"Kaufen: "+ QString::number(p->getPurchasePrice()) +"Euro<br>"+ "Leihen: " + QString::number(p->getRentanlPrice())+"Euro"+"</p>";
        QString newText = ui->OutputAnbieter->toHtml() + newEntry;
        ui->OutputAnbieter->setHtml(newText);
    }
}

void MainWindow::showGenreData()
{
    QString newtext = "";
    const QList<genre*>& genres = _controller->getGenreCache();
    for (genre* g : genres) {
        newtext += (ui->OutputGenre->toPlainText()+g->getName() + "      ");
    }
    ui->OutputGenre->setText(newtext);
}

void MainWindow::showUserData()
{
    QString imagepath = ":/rezensionen/";
    QString image ="user.jpg";
    QDir imageDir(imagepath);

    const QList<user*>& users = _controller->getUserCache();
    for (user* u : users) {
        QString newEntry = ("<img src='" + imageDir.filePath(image) + "' width='30' height='30'>" +"<b>   "+u->getFirstname()+
                    " "+u->getLastname() + "</b><br>" + u->getRating() + "<br><br>" + "sNote: "+QString::number(u->getGrade())+ "<br><br><br>");
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
    updateFilmInputBorder(false);
    ui->OutputAnbieter->setText("");
    ui->OutputGenre->setText("");
    ui->OutputBenutzerBewertung->setText("");
    deactivateView();

    _controller->clearCache();
    _controller->clearActorCache();
    _controller->clearProviderCache();
    _controller->clearGenreCache();
    _controller->clearUserCache();
    _controller->clearGradeCache();

    deleteChartContainer();
}

void MainWindow::deleteChartContainer()
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
void MainWindow::startQuery()
{
    qDebug() <<"suchebutton wurde geklickt";
    clear();

    QString film = ui->FilmInput->text();
    if(!film.isEmpty()){
    _model->getFilmDataQuery(film,_database);
    showFilmData();
    _model->getActorQuery(film,_database);
    if(!_controller->getActorCache().isEmpty()){
        showActorData();
    }
    _model->getProviderQuery(film,_database);
    showProviderData();

    _model->getGenreQuery(film,_database);
    showGenreData();

    _model->getUserQuery(film,_database);
    if(!_controller->getUserCache().isEmpty()){
        showUserData();
    }
    _model->overallRatingQuery(film,_database);
        if(!_controller->getGradeCache().isEmpty()){
            showOverallRatingDiagramm();
        }
    }
    //darf nicht wird für insertquery benötigt
    //ui->FilmInput->clear();
}


