#include "DialogSelectAnswer.h"

DialogSelectAnswer::DialogSelectAnswer(QWidget *parent, QStringList answers) : QDialog(parent)
{
    this->mapper = new QSignalMapper(this);

    this->layout = new QVBoxLayout(this);
    this->button = new QPushButton*[answers.count()];

    for (int i = 0; i < answers.count(); i++) {
        this->button[i] = new QPushButton(answers.at(i), this);
        this->layout->addWidget(this->button[i]);
        connect(this->button[i], SIGNAL(clicked()), this->mapper, SLOT(map()));
        this->mapper->setMapping(this->button[i], i);
    }

    connect(this->mapper, SIGNAL(mapped(int)), this, SLOT(selectAnswer(int)));
}

void DialogSelectAnswer::selectAnswer(int id)
{
    this->done(id + 12);
}
