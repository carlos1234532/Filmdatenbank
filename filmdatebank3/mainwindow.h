#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qsqldatabase.h"
#include <QMainWindow>
#include <model.h>
#include <QObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void startquery();

public:
    MainWindow(model* m,QSqlDatabase* db, QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    model* _model;
    QSqlDatabase* _database;

    void connectdb();
    void actions();
    void showresultsinwindow(QList<QString> *stringList);

    void loadpictures();
};
#endif // MAINWINDOW_H
