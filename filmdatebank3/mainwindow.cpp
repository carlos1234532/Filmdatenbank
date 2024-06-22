#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileDialog>

MainWindow::MainWindow(model* m, controller* c , QSqlDatabase* db, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->setVisible(false);

    _model = m;
    _controller = c;
    _database = db;

    //loadpictures();
    setuplistwidget();

    connectdb();
    actions();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadpictures()
{
    QString filePath = ":/bilder/hauptfenster.jpg";
    QPixmap pixmap(filePath);
    //ui->mainpicture->setPixmap(pixmap);
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

void MainWindow::showresultsinwindow(QList<QString> *stringList)
{
    QString listText;
    for (const QString &str : *stringList) {
        listText += str + "\n";
    }
    ui->OutputName->setText(ui->OutputName->text() + listText);
}

void MainWindow::showfilmdata()
{
    const QList<movie*>& movies= _controller->getcache();
        for (movie* m : movies) {
            ui->OutputName->setText(m->gettitle());
            ui->OutputDauer->setText(QString::number(m->getduration()));
            ui->OutputErscheinungsjahr->setText(QString::number(m->getrelease()));
            ui->OutputProduzent->setText(m->getproducer());
            ui->OutputBeschreibung->setText(m->getdescription());
        }
    _controller->clearcache();
}

void MainWindow::clear()
{
    ui->OutputName->setText("");
    ui->OutputDauer->setText("");
    ui->OutputProduzent->setText("");
    ui->OutputErscheinungsjahr->setText("");
    ui->OutputBeschreibung->setText("");
}
void MainWindow::startquery()
{
    clear();
    QList<QString> stringList;
    QList<QString>* List = &stringList;

    qDebug() <<"suchebutton wurde geklickt";
    if(ui->erweitereSucheCheckBox->isChecked()){
        _model->customquery(ui->FilmInput->text(),_database,List);
        showresultsinwindow(List);
    }
    else{
        _model->getfilmdataquery(ui->FilmInput->text(),_database);

        showfilmdata();
    }
}
