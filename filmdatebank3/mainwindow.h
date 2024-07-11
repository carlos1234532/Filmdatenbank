#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qsqldatabase.h"
#include <QMainWindow>
#include <model.h>
#include "controller.h"
#include <QObject>
#include <QListWidget>
#include <advancedsearch.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void startQuery();
    void clear();
    void insertRatingQuery();

public:
    MainWindow(model* m,controller* c,QSqlDatabase* db, QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void logoutSuccesfull();

private:
    Ui::MainWindow* ui;

    model* _model;
    controller* _controller;
    QSqlDatabase* _database;
    advancedsearch* _advancedsearch;

    void actions();
    void checkNoteInput();

    void showFilmData();
    void showActorData();
    void showProviderData();
    void showGenreData();
    void showUserData();
    void getFilmDataQuery();
    void backToLoginWindow();

    void loadCovers(QString fileName);
    void preselection();
    void advancedSearch();
    void showPreselection();
    void listItemClicked(QListWidgetItem *item);
    void sizePreselectionWidget();
    void setupListWidget();
    void positionListWidget();
    void updateFilmInputBorder(bool showBottomBorder);
    void setPixmapToLabelSize(QPixmap &pixmap);

    void deactivateView();
    void showOverallRatingDiagramm();

    void deleteChartContainer();
};
#endif // MAINWINDOW_H
