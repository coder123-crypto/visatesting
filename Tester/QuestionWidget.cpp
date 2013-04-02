#include "QuestionWidget.h"

QuestionWidget::QuestionWidget(QWidget *parent, WorkMode mode) : QDialog(parent)
{
    //        QImage img1("D:/Front.png");
    //        QImage img2("D:/Back.png");
    //        QStringList list;
    //        list << tr("Неправильное положение логотипа VISA") << tr("Неправильный формат даты окончания использования") << tr("Неправильное положение магнитной полосы");

    //        this->questions.saveNewQuestion(img1, img2, list, 0, 0);
    //    return;


    this->questions = new Questions("D:/VISAVISA.db");

    this->curMode = mode;
    this->createGUI();

    connect(this->buttonCorrect, SIGNAL(clicked()), this, SLOT(rightButtonClick()));
    connect(this->buttonNotCorrect, SIGNAL(clicked()), this, SLOT(notRightButtonClick()));
    connect(this, SIGNAL(rightAnswered()), this, SLOT(rightAnswer()));
    connect(this, SIGNAL(notRightAnswered()), this, SLOT(notRightAnswer()));
    connect(this, SIGNAL(endedQuestions()), this, SLOT(endQuestions()));

    if (this->curMode == TestingMode) {
        DialogAuthorization d(this);
        if (d.exec() == 12) {
            this->name = d.name;
            this->surname = d.surname;
            this->groupId = d.groupId;
        }
        else
            return;
    }

    this->questions->loadQuestions();
    this->results.clear();
    this->beginOfTesting = QDateTime::currentDateTime();
    this->setNextQuestion();
}

QuestionWidget::~QuestionWidget()
{
    delete this->questions;
}

void QuestionWidget::timerEvent(QTimerEvent *)
{
    this->editLeftTime->setText(secToStr(--this->currentTime));
    if (this->currentTime == 0)
        emit this->notRightAnswered();
}

void QuestionWidget::goToNextQuestion()
{
}

// Построить интерфейс
void QuestionWidget::createGUI()
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    this->resize(QApplication::desktop()->size() / 2);

    this->layout = new QHBoxLayout(this);

    this->layoutControls = new QVBoxLayout;
    this->labelLeftTime = new QLabel(tr("Осталось времени:"), this);
    this->editLeftTime = new QLineEdit(this);
    this->editLeftTime->setReadOnly(true);
    this->editLeftTime->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    this->layoutGroupBoxLeftTime = new QVBoxLayout;
    this->layoutControls->addWidget(this->labelLeftTime);
    this->layoutControls->addWidget(this->editLeftTime);
    this->layoutControls->addStretch();

    this->buttonCorrect = new QPushButton(tr("Карта верна"), this);
    this->buttonNotCorrect = new QPushButton(tr("Карта не верна"), this);
    this->layoutControls->addWidget(this->buttonCorrect);
    this->layoutControls->addWidget(this->buttonNotCorrect);

    this->line = new QFrame(this);
    this->line->setFrameShape(QFrame::VLine);
    this->line->setFrameShadow(QFrame::Sunken);

    this->layoutImages = new QVBoxLayout;
    for (int i = 0; i < 2; i++) {
        this->cardImage[i] = new ImageLabel(this);
        this->cardImage[i]->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        this->layoutImages->addWidget(this->cardImage[i]);
    }

    this->layout->addLayout(this->layoutControls);
    this->layout->addWidget(this->line);
    this->layout->addLayout(this->layoutImages);
}

void QuestionWidget::rightButtonClick()
{
    if (this->questions->count() == 0)
        return;

    switch (this->curMode) {
    case QuestionWidget::FreeTimeMode:
        if (this->curQuestion.isRight == true)
            emit this->rightAnswered();
        else if (this->curQuestion.isRight == false) {
            QMessageBox::information(this, "", tr("Правильный ответ: %1").arg(this->curQuestion.answers.at(this->curQuestion.rightAnswerIndex)));
            emit this->notRightAnswered();
        }
        break;

    case QuestionWidget::TestingMode:
        if (this->curQuestion.isRight == true)
            emit this->rightAnswered();
        else if (this->curQuestion.isRight == false)
            emit this->notRightAnswered();
        break;
    }
}

void QuestionWidget::notRightButtonClick()
{
    if (this->questions->count() == 0)
        return;

    switch (this->curMode) {
    case QuestionWidget::FreeTimeMode:
        if (this->curQuestion.isRight == true) {
            QMessageBox::information(this, "", tr("Карта верна!"));
            emit this->notRightAnswered();
        }
        else if (this->curQuestion.isRight == false)
            emit this->rightAnswered();
        break;

    case QuestionWidget::TestingMode:
        if (this->curQuestion.isRight == true)
            emit this->notRightAnswered();
        else if (this->curQuestion.isRight == false) {
            DialogSelectAnswer d(this, this->curQuestion.answers);
            if (d.exec() - 12 == this->curQuestion.rightAnswerIndex)
                emit this->rightAnswered();
            else
                emit this->notRightAnswered();
        }
        break;
    }
}

void QuestionWidget::setNextQuestion()
{
    if (this->questions->nextQuestion(&this->curQuestion) == true) {
        this->cardImage[0]->setImage(this->curQuestion.imageFront);
        this->cardImage[1]->setImage(this->curQuestion.imageBack);

        if (this->curMode == QuestionWidget::TestingMode) {
            // Если режим тестирования - запускаем таймер обратного отчета
            this->currentTime = 3;
            this->editLeftTime->setText(secToStr(this->currentTime));
            this->timerId = this->startTimer(1000);
        }
    }
    else
        emit this->endedQuestions();
}

// Правильный ответ
void QuestionWidget::rightAnswer()
{
    this->killTimer(this->timerId);
    this->results.append('1');
    this->setNextQuestion();
}

// Неправильный ответ
void QuestionWidget::notRightAnswer()
{
    this->killTimer(this->timerId);
    this->results.append('0');
    this->setNextQuestion();
}

// Вопросы закончились
void QuestionWidget::endQuestions()
{
    this->endOfTesting = QDateTime::currentDateTime();
    this->buttonCorrect->setEnabled(false);
    this->buttonNotCorrect->setEnabled(false);

    if (this->curMode == TestingMode) {
        QSettings s("MIREA", "VisaTestViewer");
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "ytrewq");
        db.setHostName(s.value("HostName").toString());
        db.setUserName(s.value("UserName").toString());
        db.setPort(s.value("Port").toInt());
        db.setPassword(s.value("Password").toString());

        if (db.open() == false)
            QMessageBox::critical(this, "", db.lastError().text());

        QSqlQuery query(db);
        if (query.exec("USE VISATESTING;") == false)
            QMessageBox::critical(this, "", query.lastError().text());

        query.prepare("INSERT INTO Students (GroupId, FirstName, LastName, Results, TestDate) VALUES (?, ?, ?, ?, ?);");
        query.addBindValue(this->groupId);
        query.addBindValue(this->name);
        query.addBindValue(this->surname);
        query.addBindValue(this->results);
        query.addBindValue(QDateTime::currentDateTime());
        if (query.exec() == false)
            QMessageBox::critical(this, "", query.lastError().text());
        db.close();
    }

    DialogResults d(this, this->results);
    d.exec();
}
