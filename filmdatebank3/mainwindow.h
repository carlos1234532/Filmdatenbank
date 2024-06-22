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

public:
    MainWindow(model* m,controller* c,QSqlDatabase* db, QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;

    model* _model;
    controller* _controller;
    QSqlDatabase* _database;

    void connectdb();
    void actions();
    void showfilmdata();
    void showactordata();
    void getfilmdataquery();
    void loadcovers(QString fileName);
    void preselection();
    void showpreselection();
    void listitemclicked(QListWidgetItem *item);
    void sizepreselectionwidget();
    void setuplistwidget();
    void positionlistwidget();
    void updatefilminputborder(bool showBottomBorder);
    void setpixmaptolabelsize(QPixmap &pixmap);
};
#endif // MAINWINDOW_H
