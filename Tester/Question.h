#ifndef QUESTION_H
#define QUESTION_H

#include <QSqlDatabase>
#include <QByteArray>
#include <QSqlQuery>
#include <QSqlError>
#include <QImage>
#include <QStringList>
#include <QString>
#include <QVector>
#include <QVariant>
#include <QBuffer>
#include <QTime>
#include <QSettings>

struct QuestionPrivate
{
    QImage imageFront;
    QImage imageBack;
    QStringList answers;
    bool isRight;
    int rightAnswerIndex;
};

class Questions
{
public:
    Questions(QString fileName);
    ~Questions();

    bool loadQuestions();
    bool isDbOpened() const;
    QString lastError() const;
    int count() const;
    bool nextQuestion(QuestionPrivate* dst);

//private:
    QVector<QuestionPrivate> questions;
    QSqlDatabase db;
    QString _err;
    int currentIndex;

    bool saveNewQuestion(QImage &front, QImage &back, QStringList answeres, unsigned int rightAnswerIndex, bool isRight);
    void mixQuestions();
};

#endif // QUESTION_H
