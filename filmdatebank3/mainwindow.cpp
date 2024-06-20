#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileDialog>

MainWindow::MainWindow(model* m, QSqlDatabase *db, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _model = m;
    _database = db;

    loadpictures();

    connectdb();
    actions();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadpictures()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    QPixmap pixmap(filePath); // Pfad zum Bild anpassen
    ui->mainpicture->setPixmap(pixmap);
}

void MainWindow::connectdb()
{
    qDebug() <<"MainWindow::connectDB wurde geöffnet";
    _model->connectionbuild(_database);
}

void MainWindow::actions()
{
    connect(ui->sucheButton, &QPushButton::clicked, this,&MainWindow::startquery);
}

void MainWindow::showresultsinwindow(QList<QString> *stringList)
{
    QString listText;
    for (const QString &str : *stringList) {
        listText += str + "\n";
    }
    ui->Output->setText(ui->Output->text() + listText);
}

void MainWindow::startquery()
{
    ui->Output->setText("");

    QList<QString> stringList;
    QList<QString>* List = &stringList;

    qDebug() <<"suchebutton wurde geklickt";
    if(ui->FilmreiheCheckbox->isChecked()){
        _model->customquery(ui->FilmreiheInputLine->text(),_database,List);
    }

    showresultsinwindow(List);
}
