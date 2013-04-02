#ifndef DIALOGNOTE_H
#define DIALOGNOTE_H

#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>

class DialogNote : public QDialog
{
    Q_OBJECT
public:
    explicit DialogNote(QWidget *parent = 0);

private slots:
    void showEvent(QShowEvent *);
    
};

#endif // DIALOGNOTE_H
