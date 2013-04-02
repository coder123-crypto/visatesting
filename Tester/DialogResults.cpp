#include "DialogResults.h"

DialogResults::DialogResults(QWidget *parent, QByteArray results) : QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->layout = new QVBoxLayout(this);
    this->edit = new QTextEdit(this);
    this->edit->setReadOnly(true);
    this->button = new QPushButton(tr("OK"), this);
    this->layout->addWidget(this->edit);
    this->layout->addWidget(this->button);
    int i = 0;
    for (QByteArray::const_iterator it = results.constBegin(); it != results.constEnd(); it++) {
        i++;
        const QString res = (*it != '0') ? tr("верно") : tr("не верно");
        this->edit->append(tr("Вопрос %1: %2").arg(i).arg(res));
    }
    this->edit->append(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm"));

    connect(this->button, SIGNAL(clicked()), this, SLOT(close()));
}
