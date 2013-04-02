#ifndef DIALOGAUTHORIZATION_H
#define DIALOGAUTHORIZATION_H

#include <QDialog>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QSqlError>

class DialogAuthorization : public QDialog
{
    Q_OBJECT
public:
    explicit DialogAuthorization(QWidget *parent = 0);
    ~DialogAuthorization();
    QString name;
    QString surname;
    int groupId;

private slots:
    void showEvent(QShowEvent *);
    void ok();
    void cancel();

private:
    QVBoxLayout *layout;
    QGroupBox *groupBoxGroups;
    QGroupBox *groupBoxName;
    QGroupBox *groupBoxSurname;
    QDialogButtonBox *buttons;
    QComboBox *comboGroup;
    QLineEdit *lineEditName;
    QLineEdit *lineEditSurname;

    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif // DIALOGAUTHORIZATION_H
