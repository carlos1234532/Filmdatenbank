#include "mainwindow.h"
#include "model.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    model* m = new model();
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    MainWindow w(m,&db);
    w.show();
    return a.exec();
}
