#include "QuestionDialog.h"

QuestionWidget::QuestionWidget(QWidget *parent, WorkMode mode) : QDialog(parent)
{
    // BEGIN =====================================
//    this->questions = new Questions("/home/skandinavijos/VISAVISA.db");
//    QImage img1[10];
//    QImage img2[10];
//    QStringList list[10];

//    img1[0].load("/home/skandinavijos/cards/1true/Front.png");
//    img1[1].load("/home/skandinavijos/cards/2true/Front.png");
//    img1[2].load("/home/skandinavijos/cards/3false/Front.png");
//    img1[3].load("/home/skandinavijos/cards/4false/Front.png");
//    img1[4].load("/home/skandinavijos/cards/5true/Front.png");
//    img1[5].load("/home/skandinavijos/cards/6false/Front.png");
//    img1[6].load("/home/skandinavijos/cards/7false/Front.png");
//    img1[7].load("/home/skandinavijos/cards/8false/Front.png");
//    img1[8].load("/home/skandinavijos/cards/9true/Front.png");
//    img1[9].load("/home/skandinavijos/cards/10false/Front.png");

//    img2[0].load("/home/skandinavijos/cards/1true/Back.png");
//    img2[1].load("/home/skandinavijos/cards/2true/Back.png");
//    img2[2].load("/home/skandinavijos/cards/3false/Back.png");
//    img2[3].load("/home/skandinavijos/cards/4false/Back.png");
//    img2[4].load("/home/skandinavijos/cards/5true/Back.png");
//    img2[5].load("/home/skandinavijos/cards/6false/Back.png");
//    img2[6].load("/home/skandinavijos/cards/7false/Back.png");
//    img2[7].load("/home/skandinavijos/cards/8false/Back.png");
//    img2[8].load("/home/skandinavijos/cards/9true/Back.png");
//    img2[9].load("/home/skandinavijos/cards/10false/Back.png");

//    list[0] << tr("Неправильный формат даты окончания использования") <<
//               tr("Неправильное положение логотипа VISA") <<
//               tr("Неправильное положение магнитной полосы");

//    list[1] << tr("Неправильное положение логотипа VISA") <<
//               tr("Неправильное положение магнитной полосы") <<
//               tr("Неправильный формат даты окончания использования");

//    list[2] << tr("Неправильный формат БИН") << // !
//               tr("Неправильный формат даты окончания использования") <<
//               tr("Неправильное положение логотипа VISA");

//    list[3] << tr("Неправильное положение логотипа VISA") <<
//               tr("Неправильный формат даты окончания использования") << // !
//               tr("Не совпадают цифры кода безопасности на оборотной стороне");

//    list[4] << tr("Не совпадают цифры кода безопасности на оборотной стороне") <<
//               tr("Отсутствует логотип банка-эмитента") <<
//               tr("Неправильный формат даты окончания использования");

//    list[5] << tr("Неправильный формат имени держателя") <<
//               tr("Неправильное положение логотипа VISA") <<
//               tr("Неправильный формат кода безопасности"); // !

//    list[6] << tr("Неправильное положение логотипа VISA") <<
//               tr("Неправильный логотип VISA") <<   // !
//               tr("Неправильный формат даты окончания использования");

//    list[7] << tr("Не совпадают цифры под номером карты") <<
//               tr("Неправильный логотип VISA") <<
//               tr("Неправильный формат имени держателя");   // !

//    list[8] << tr("Неправильное положение логотипа VISA") <<
//               tr("Неправильный формат даты окончания использования") <<
//               tr("Не совпадают цифры кода безопасности на оборотной стороне");

//    list[9] << tr("Неправильный формат номер карты") << // !
//               tr("Отсутствует логотип банка-эмитента") <<
//               tr("Неправильный формат даты окончания использования");

//    this->questions->saveNewQuestion(img1[0], img2[0], list[0], -1, 1);
//    this->questions->saveNewQuestion(img1[1], img2[1], list[1], -1, 1);
//    this->questions->saveNewQuestion(img1[2], img2[2], list[2], 0, 0);
//    this->questions->saveNewQuestion(img1[3], img2[3], list[3], 1, 0);
//    this->questions->saveNewQuestion(img1[4], img2[4], list[4], -1, 1);

//    this->questions->saveNewQuestion(img1[5], img2[5], list[5], 2, 0);
//    this->questions->saveNewQuestion(img1[6], img2[6], list[6], 1, 0);
//    this->questions->saveNewQuestion(img1[7], img2[7], list[7], 2, 0);
//    this->questions->saveNewQuestion(img1[8], img2[8], list[8], -1, 1);
//    this->questions->saveNewQuestion(img1[9], img2[9], list[9], 0, 0);

//    return;

    // END =======================================

#ifdef Q_OS_LINUX
    this->questions = new Questions("/home/skandinavijos/VISAVISA.db");
#else
    this->questions = new Questions("D:/VISAVISA.db");
#endif

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
    }

    this->questions->loadQuestions();
    if (this->questions->count() > 0) {
        this->results.clear();
        this->beginOfTesting = QDateTime::currentDateTime();
        this->setNextQuestion();
    }
    else
        QMessageBox::warning(this, "", tr("Вопросы не были загружены"));
}

QuestionWidget::~QuestionWidget()
{
    delete this->questions;
}

void QuestionWidget::timerEvent(QTimerEvent *)
{
    if (this->questions->count() > 0) {
        this->editLeftTime->setText(secToStr(--this->currentTime));
        if (this->currentTime == 0)
            emit this->notRightAnswered();
    }
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
#include <QDebug>
void QuestionWidget::rightButtonClick()
{
    if (this->questions->count() > 0) {
        switch (this->curMode) {
        case QuestionWidget::FreeTimeMode:
            if (this->curQuestion.isRight == true) {
                emit this->rightAnswered();
            }
            else if (this->curQuestion.isRight == false) {
                QMessageBox::information(this, "", tr("Правильный ответ: %1").arg(this->curQuestion.answers.at(this->curQuestion.rightAnswerIndex)));
                emit this->notRightAnswered();
            }
            break;

        case QuestionWidget::TestingMode:
            if (this->curQuestion.isRight == true) {
                emit this->rightAnswered();
            }
            else if (this->curQuestion.isRight == false) {
                emit this->notRightAnswered();
            }
            break;
        }
    }
}

void QuestionWidget::notRightButtonClick()
{
    if (this->questions->count() > 0) {
        switch (this->curMode) {
        case QuestionWidget::FreeTimeMode:
            if (this->curQuestion.isRight == true) {
                QMessageBox::information(this, "", tr("Карта верна!"));
                emit this->notRightAnswered();
            }
            else if (this->curQuestion.isRight == false) {
                emit this->rightAnswered();
            }
            break;

        case QuestionWidget::TestingMode:
            if (this->curQuestion.isRight == true) {
                emit this->notRightAnswered();
            }
            else if (this->curQuestion.isRight == false) {
                DialogSelectAnswer d(this, this->curQuestion.answers);
                if (d.exec() - 12 == this->curQuestion.rightAnswerIndex) {
                    emit this->rightAnswered();
                }
                else {
                    emit this->notRightAnswered();
                }
            }
            break;
        }
    }
}

void QuestionWidget::setNextQuestion()
{
    if (this->questions->count() > 0) {
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
        QSqlDatabase::removeDatabase("ytrewq");
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "ytrewq");
        db.setHostName(s.value("HostName").toString());
        db.setUserName(s.value("UserName").toString());
        db.setPort(s.value("Port").toInt());
        db.setPassword(s.value("Password").toString());

        if (db.open() == true) {
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
        else
            QMessageBox::critical(this, "", db.lastError().text());
    }

    DialogResults d(this, this->results);
    d.exec();
}
