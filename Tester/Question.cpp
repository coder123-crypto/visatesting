#include "Question.h"

Questions::Questions(QString fileName)
{
    this->db = QSqlDatabase::addDatabase("QSQLITE", "abracadabra");
    this->db.setDatabaseName(fileName);
    this->db.open();

    QSqlQuery *query = new QSqlQuery(this->db);
    query->exec("CREATE TABLE 'questions' (num INTEGER, frontImage BLOB, backImage BLOB, answeres BLOB, rightAnswer INTEGER, isRight INTEGER, PRIMARY KEY (num));");
    this->_err = query->lastError().text();
    delete query;

    this->currentIndex = 0;
}

Questions::~Questions()
{
    this->db.close();
    this->db.removeDatabase(QSqlDatabase::database().connectionName());
}

bool Questions::loadQuestions()
{
    if (this->db.isOpen() == false)
        return false;
    QSqlQuery *query = new QSqlQuery(this->db);
    bool ok = query->exec("SELECT num, frontImage, backImage, answeres, rightAnswer, isRight FROM 'questions'");
    if (ok == true) {
        this->questions.clear();
        while (query->next()) {
            QuestionPrivate newQuestion;
            newQuestion.imageFront = QImage::fromData(query->value(1).toByteArray());
            newQuestion.imageBack = QImage::fromData(query->value(2).toByteArray());
            newQuestion.answers = QString(query->value(3).toByteArray()).split("\r\n");
            newQuestion.isRight = query->value(5).toInt() == 0 ? false : true;
            newQuestion.rightAnswerIndex = query->value(4).toUInt();
            this->questions.append(newQuestion);
        }
        this->mixQuestions();
        this->currentIndex = 0;
    }
    this->_err = query->lastError().text();
    delete query;

    return ok;
}

bool Questions::isDbOpened() const
{
    return this->db.isOpen();
}

QString Questions::lastError() const
{
    return this->_err;
}

int Questions::count() const
{
    return this->questions.count();
}

bool Questions::nextQuestion(QuestionPrivate *dst)
{
    if (this->currentIndex < this->questions.count()) {
        dst->answers = this->questions.at(this->currentIndex).answers;
        dst->imageBack = this->questions.at(this->currentIndex).imageBack;
        dst->imageFront = this->questions.at(this->currentIndex).imageFront;
        dst->isRight = this->questions.at(this->currentIndex).isRight;
        dst->rightAnswerIndex = this->questions.at(this->currentIndex).rightAnswerIndex;
        this->currentIndex++;
        return true;
    }
    else
        return false;
}

bool Questions::saveNewQuestion(QImage &front, QImage &back, QStringList answeres, unsigned int rightAnswerIndex, bool isRight)
{
    QSqlQuery *query = new QSqlQuery(this->db);

    QByteArray byteArrayFront;
    QByteArray byteArrayBack;
    QByteArray byteArrayAnsweres;
    QBuffer bufferFront(&byteArrayFront);
    QBuffer bufferBack(&byteArrayBack);

    bufferFront.open(QIODevice::WriteOnly);
    bufferBack.open(QIODevice::WriteOnly);

    front.save(&bufferFront, "png");
    back.save(&bufferBack, "png");

    byteArrayAnsweres = answeres.join("\r\n").toUtf8();

    query->prepare("INSERT INTO 'questions' (frontImage, backImage, answeres, rightAnswer, isRight) VALUES (?, ?, ?, ?, ?)");
    query->addBindValue(byteArrayFront);
    query->addBindValue(byteArrayBack);
    query->addBindValue(byteArrayAnsweres);
    query->addBindValue(rightAnswerIndex);
    query->addBindValue(isRight);
    bool ok = query->exec();

    delete query;
    return ok;
}

void Questions::mixQuestions()
{
    const int size = this->questions.count();
    qsrand(QTime::currentTime().msec());
    for (int i = 0; i < size; i++)
        qSwap(this->questions[i], this->questions[qrand() % (size - i) + i]);
}


