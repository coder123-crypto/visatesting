#include "DialogNote.h"

DialogNote::DialogNote(QWidget *parent) : QDialog(parent),
    webView(new QWebView(this))
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::WindowMaximizeButtonHint);
    this->setLayout(new QVBoxLayout);
    this->layout()->addWidget(this->webView);
    this->webView->setUrl(QUrl("qrc:/help.htm"));
}

void DialogNote::showEvent(QShowEvent *e)
{
    const int x = (QApplication::desktop()->width() - this->width()) / 2;
    const int y = (QApplication::desktop()->height() - this->height()) / 2;
    this->move(QPoint(x, y));
    QDialog::showEvent(e);
}
