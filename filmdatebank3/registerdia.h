#ifndef REGISTERDIA_H
#define REGISTERDIA_H

#include <QDialog>
#include <model.h>

QT_BEGIN_NAMESPACE
namespace Ui { class registerdia; }
QT_END_NAMESPACE

class registerdia : public QDialog
{
    Q_OBJECT

public:
    explicit registerdia(model* m,QSqlDatabase* db,QWidget *parent = nullptr);
    ~registerdia();

signals:
    void registrationSuccessful();

private slots:
    void registerbuttonclicked();

private:
    Ui::registerdia *ui;

    model* _model;
    QSqlDatabase* _database;

    void actions();
    void insertsuccessful();

    void checkbenutzernameinput();
    void checknachnameinput();
    void checkvornameinput();
    void checkemailinput();
    void clear();
};

#endif // REGISTERDIA_H
