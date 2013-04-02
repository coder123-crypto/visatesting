#include "DelegateTestResult.h"

DelegateTestResult::DelegateTestResult(QObject *parent) : QItemDelegate(parent)
{
}

void DelegateTestResult::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 4)
    {
        QByteArray array = index.model()->data(index, Qt::DisplayRole).toByteArray();
        int errorNum = 0;
        if (array.isEmpty() == false) {
            for (QByteArray::const_iterator it = array.constBegin(); it != array.constEnd(); it++)
                if (*it == '0')
                    errorNum++;
            drawDisplay(painter, option, option.rect, QString::number(errorNum));
        }
        else
            drawDisplay(painter, option, option.rect, QString());
        drawFocus(painter, option, option.rect);
    }
    else if (index.column() == 5) {
        QString text = index.model()->data(index, Qt::DisplayRole).toDateTime().toString(format);
        drawDisplay(painter, option, option.rect, text);
        drawFocus(painter, option, option.rect);
    }
    else
        QItemDelegate::paint(painter, option, index);
}

QWidget *DelegateTestResult::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 4)
    {
        QLineEdit *editor = new QLineEdit(parent);
        QString currentDate = index.model()->data(index, Qt::DisplayRole).toString();
        editor->setText(currentDate);
        return editor;
    }
    else if (index.column() == 5) {
        QDateTimeEdit *editor = new QDateTimeEdit(parent);
        editor->setDisplayFormat(format);
        QDateTime currentDate = index.model()->data(index, Qt::DisplayRole).toDateTime();
        editor->setDateTime(currentDate);
        return editor;
    }
    else
        QItemDelegate::createEditor(parent, option, index);
}

void DelegateTestResult::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == 4)
    {
        QLineEdit *dateEditor = qobject_cast<QLineEdit*>(editor);
        if (dateEditor != NULL)
        {
            dateEditor->setText(index.model()->data(index, Qt::EditRole).toString());
        }
    }
    else if (index.column() == 5) {
        QDateTimeEdit *dateEditor = qobject_cast<QDateTimeEdit *>(editor);
        if (dateEditor != NULL)
        {
            dateEditor->setDateTime(index.model()->data(index, Qt::EditRole).toDateTime());
        }
    }
    else
        QItemDelegate::setEditorData(editor, index);
}

void DelegateTestResult::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 4)
    {
        QLineEdit *dateEditor = qobject_cast<QLineEdit*>(editor);
        if (dateEditor != NULL)
        {
            model->setData(index, dateEditor->text(), Qt::EditRole);
        }
    }
    else if (index.column() == 5) {
        QDateTimeEdit *dateEditor = qobject_cast<QDateTimeEdit *>(editor);
        if (dateEditor != NULL)
        {
            model->setData(index, dateEditor->dateTime().toString(format), Qt::EditRole);
        }
    }
    else
        QItemDelegate::setModelData(editor, model, index);
}
