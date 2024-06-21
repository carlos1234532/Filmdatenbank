#include "mainwindow.h"
#include "model.h"
#include "controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    controller* c = new controller();
    model* m = new model(c);
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    MainWindow w(m,c,&db);
    w.show();
    return a.exec();
}
