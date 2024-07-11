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
    void registerButtonClicked();

private:
    Ui::registerdia *ui;

    model* _model;
    QSqlDatabase* _database;

    void actions();
    void insertSuccessful();

    void checkBenutzernameInput();
    void checkNachnameInput();
    void checkVornameInput();
    void checkEmailInput();
    void clear();
};

#endif // REGISTERDIA_H
