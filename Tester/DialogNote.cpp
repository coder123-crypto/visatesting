#include "DialogNote.h"

DialogNote::DialogNote(QWidget *parent) : QDialog(parent)
{
}

void DialogNote::showEvent(QShowEvent *e)
{
    const int x = (QApplication::desktop()->width() - this->width()) / 2;
    const int y = (QApplication::desktop()->height() - this->height()) / 2;
    this->move(QPoint(x, y));
    QDialog::showEvent(e);
}
