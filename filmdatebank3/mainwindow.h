#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qsqldatabase.h"
#include <QMainWindow>
#include <model.h>
#include "controller.h"
#include <QObject>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void startquery();
    void clear();
    void insertratingquery();

public:
    MainWindow(model* m,controller* c,QSqlDatabase* db, QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void logoutsuccesfull();

private:
    Ui::MainWindow* ui;

    model* _model;
    controller* _controller;
    QSqlDatabase* _database;

    void actions();
    void checknoteinput();

    void showfilmdata();
    void showactordata();
    void showproviderdata();
    void showgenredata();
    void showuserdata();
    void getfilmdataquery();
    void backtologinwindow();

    void loadcovers(QString fileName);
    void preselection();
    void showpreselection();
    void listitemclicked(QListWidgetItem *item);
    void sizepreselectionwidget();
    void setuplistwidget();
    void positionlistwidget();
    void updatefilminputborder(bool showBottomBorder);
    void setpixmaptolabelsize(QPixmap &pixmap);

    void deactivateview();
    void showdoverallratingdiagramm();

    void deletechartcontainer();
};
#endif // MAINWINDOW_H
