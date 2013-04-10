#include "ImageLabel.h"

ImageLabel::ImageLabel(QWidget *parent) : QLabel(parent)
{
}

void ImageLabel::setImage(const QImage &img)
{
    this->_pixmap = QPixmap::fromImage(img);
    this->repaint();
}

void ImageLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (!this->_pixmap.isNull()) {
        QPixmap pixmap = this->_pixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap((this->width() - pixmap.width()) / 2, 0, pixmap);
    }
}
