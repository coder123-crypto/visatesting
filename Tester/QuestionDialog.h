#ifndef QUESTIONWIDGET_H
#define QUESTIONWIDGET_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>
#include <ImageLabel.h>
#include <Question.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QFrame>
#include <QDateTime>
#include <DialogAuthorization.h>
#include <DialogResults.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QByteArray>
#include <DialogSelectAnswer.h>
#include <QSettings>
#include <QCloseEvent>

// Перевод секунд в человеческое время
static inline QString secToStr(const int sec)
{
    if (sec <= 0)
        return QObject::tr("00:00:00");
    int s, m, h;
    s = sec % 60;
    h = sec / 3600;
    m = (sec - h * 3600 - s) / 60;
    return QObject::tr("%1:%2:%3").arg(h, 2, 10, 0, '0').arg(m, 2, 10, 0, '0').arg(s, 2, 10, 0, '0');
}

class QuestionWidget : public QDialog
{
    Q_OBJECT
public:
    enum WorkMode {FreeTimeMode, TestingMode};

    explicit QuestionWidget(QWidget *parent = 0, WorkMode mode = FreeTimeMode);
    ~QuestionWidget();

private slots:
    void timerEvent(QTimerEvent *);
    void goToNextQuestion();
    void rightButtonClick();
    void notRightButtonClick();
    void setNextQuestion();
    void rightAnswer();
    void notRightAnswer();
    void endQuestions();
    void closeEvent(QCloseEvent *);

private:

    ImageLabel *cardImage[2];
    QPushButton *buttonCorrect;
    QPushButton *buttonNotCorrect;
    QLabel *labelLeftTime;
    QLineEdit *editLeftTime;
    QHBoxLayout *layout;
    QVBoxLayout *layoutControls;
    QVBoxLayout *layoutImages;
    QVBoxLayout *layoutGroupBoxLeftTime;
    QFrame *line;

    int timerId;
    int currentTime;
    Questions *questions;
    QuestionPrivate curQuestion;
    WorkMode curMode;
    QByteArray results;
    QDateTime beginOfTesting;
    QDateTime endOfTesting;

    int groupId;
    QString name;
    QString surname;

    bool isTesting;

    inline void createGUI();

signals:
    void rightAnswered();
    void notRightAnswered();
    void endedQuestions();
    void exited(int);
};

#endif // QUESTIONWIDGET_H
