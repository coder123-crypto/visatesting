#ifndef DIALOGDATABASEPARAMS_H
#define DIALOGDATABASEPARAMS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QSettings>
#include <QApplication>
#include <QDesktopWidget>
#include <QIntValidator>

class DialogDatabaseParams : public QDialog
{
    Q_OBJECT
public:
    explicit DialogDatabaseParams(QWidget *parent = 0);

private slots:
    void showEvent(QShowEvent *);
    void saveParams();

private:
    QVBoxLayout *layout;
    QDialogButtonBox *buttons;
    QGroupBox *groupBoxHost;
    QGroupBox *groupBoxPort;
    QGroupBox *groupBoxLogin;
    QGroupBox *groupBoxPassword;
    QLineEdit *lineEditHost;
    QLineEdit *lineEditPort;
    QLineEdit *lineEditLogin;
    QLineEdit *lineEditPassword;
};

#endif // DIALOGDATABASEPARAMS_H
