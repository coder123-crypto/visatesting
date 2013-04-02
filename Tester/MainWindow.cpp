#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setCentralWidget(new QWidget(this));
    this->layout = new QVBoxLayout(this->centralWidget());

    this->buttonNote = new QPushButton(tr("Описание карт VISA"), this->centralWidget());
    this->buttonFreeTime = new QPushButton(tr("Практические занятия"), this->centralWidget());
    this->buttonTesting = new QPushButton(tr("Тестирование"), this->centralWidget());

    this->layout->addWidget(this->buttonNote);
    this->layout->addWidget(this->buttonFreeTime);
    this->layout->addWidget(this->buttonTesting);

    connect(this->buttonFreeTime, SIGNAL(clicked()), this, SLOT(execFreeTimeMode()));
    connect(this->buttonTesting, SIGNAL(clicked()), this, SLOT(execTesting()));
    connect(this->buttonNote, SIGNAL(clicked()), this, SLOT(execNote()));
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::showEvent(QShowEvent *e)
{
    this->setFixedSize(this->size());
    const int x = (QApplication::desktop()->width() - this->width()) / 2;
    const int y = (QApplication::desktop()->height() - this->height()) / 2;
    this->move(QPoint(x, y));
    QMainWindow::showEvent(e);
}

void MainWindow::execNote()
{
    DialogNote d(this);
    d.exec();
}

void MainWindow::execFreeTimeMode()
{
    QuestionWidget d(this, QuestionWidget::FreeTimeMode);
    d.exec();
}

void MainWindow::execTesting()
{
    QuestionWidget d(this, QuestionWidget::TestingMode);
    d.exec();
}
