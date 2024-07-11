#include "mainwindow.h"
#include "model.h"
#include "controller.h"
#include "login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    controller* c = new controller();
    model* m = new model(c);
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    m->connectionBuild(&db);

    login l(m,c,&db);
    l.show();

    MainWindow w(m,c,&db);
    QObject::connect(&l, &login::loginSuccessfull, [&w](){
        w.showFullScreen();
    });

    QObject::connect(&w, &MainWindow::logoutSuccesfull, [&l]() {
            l.show();
        });
    return a.exec();
}
