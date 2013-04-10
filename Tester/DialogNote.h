#ifndef DIALOGNOTE_H
#define DIALOGNOTE_H

#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QWebView>
#include <QVBoxLayout>

class DialogNote : public QDialog
{
    Q_OBJECT
public:
    explicit DialogNote(QWidget *parent = 0);

private slots:
    void showEvent(QShowEvent *);

private:
    QWebView *webView;
};

#endif // DIALOGNOTE_H
