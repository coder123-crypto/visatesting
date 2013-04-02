#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QuestionDialog.h>
#include <DialogNote.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showEvent(QShowEvent *);
    void execNote();
    void execFreeTimeMode();
    void execTesting();

private:
    QVBoxLayout *layout;
    QPushButton *buttonFreeTime;
    QPushButton *buttonTesting;
    QPushButton *buttonNote;
};

#endif // MAINWINDOW_H
