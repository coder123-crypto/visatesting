#ifndef DELEGATETESTRESULT_H
#define DELEGATETESTRESULT_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QByteArray>

static const QString format = "yyyy.MM.dd hh:mm";

class DelegateTestResult : public QItemDelegate
{
    Q_OBJECT

public:
    DelegateTestResult(QObject *parent = NULL);

private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};


#endif // DELEGATETESTRESULT_H
