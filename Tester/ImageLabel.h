#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QPainter>

class ImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageLabel(QWidget *parent = 0);
    void setImage(const QImage &img);

private slots:
    void paintEvent(QPaintEvent *);

private:
    QPixmap _pixmap;
};

#endif // IMAGELABEL_H
