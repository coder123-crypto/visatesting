#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalDelegate>
#include <QSqlRecord>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <DelegateTestResult.h>
#include <QMenuBar>
#include <QMenu>
#include <QSettings>
#include <QSet>
#include <DialogDatabaseParams.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void addGroup();
    void renameGroup();
    void deleteGroup();
    void selectGroup(int);
    void removeStudent();
    void showEvent(QShowEvent *);
    void showDbParams();

private:
    QPushButton *butAddGroup;
    QPushButton *butRenameGroup;
    QPushButton *butDeleteGroup;
    QPushButton *butRemoveStudent;
    QVBoxLayout *layoutButtons;
    QHBoxLayout *layout;
    QComboBox *comboBoxGroups;
    QTableView *viewStudents;
    QMenuBar *menuBar;
    QMenu *menuDbParams;

    QSqlDatabase db;
    QSqlRelationalTableModel *model;
    QString lastError;

    void createGUI();
    void loadDataBase();
    bool createDataBase();
};

#endif // MAINWINDOW_H
