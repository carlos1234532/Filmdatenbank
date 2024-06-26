#ifndef ADVANCEDSEARCH_H
#define ADVANCEDSEARCH_H

#include <QDialog>
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class advancedsearch; }
QT_END_NAMESPACE

class advancedsearch : public QDialog
{
    Q_OBJECT

public:
    explicit advancedsearch(model* m,controller* c,QSqlDatabase* db,QWidget *parent = nullptr);
    ~advancedsearch();

private:
    Ui::advancedsearch *ui;

    model* _model;
    controller* _controller;
    QSqlDatabase* _database;

    void actions();
    void leave();
};

#endif // ADVANCEDSEARCH_H
