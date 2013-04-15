#include "DialogSelectAnswer.h"

DialogSelectAnswer::DialogSelectAnswer(QWidget *parent, QStringList answers) : QDialog(parent),
    mapper(new QSignalMapper(this)),
    button(new QPushButton*[answers.count()]),
    layout(new QVBoxLayout(this))
{
    this->count = answers.count();
    for (int i = 0; i < answers.count(); i++) {
        this->button[i] = new QPushButton(answers.at(i), this);
        this->layout->addWidget(this->button[i]);
        connect(this->button[i], SIGNAL(clicked()), this->mapper, SLOT(map()));
        this->mapper->setMapping(this->button[i], i);
    }

    connect(this->mapper, SIGNAL(mapped(int)), this, SLOT(selectAnswer(int)));
}

DialogSelectAnswer::~DialogSelectAnswer()
{
    for (int i = 0; i < this->count; i++)
        delete this->button[i];
    delete this->button;
}

void DialogSelectAnswer::selectAnswer(int id)
{
    this->done(id + 12);
}
