#ifndef DIALOGRESULTS_H
#define DIALOGRESULTS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QByteArray>
#include <QDateTime>

class DialogResults : public QDialog
{
    Q_OBJECT
public:
    explicit DialogResults(QWidget *parent = 0, QByteArray results = QByteArray());

private:
    QVBoxLayout *layout;
    QTextEdit *edit;
    QPushButton *button;
};

#endif // DIALOGRESULTS_H
