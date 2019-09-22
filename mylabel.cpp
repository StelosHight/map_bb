#include "mylabel.h"

MyLabel::MyLabel(const QString& text, QWidget* parent) : QLabel(parent)
{
    setText(text);
}

MyLabel::MyLabel(int coord_texture, const QString &text, QWidget *parent): QLabel(parent)
{
    setText(text);
    this->coord_texture = coord_texture;
}

void MyLabel::set_path(QString path)
{
    this->path = path;
}

void MyLabel::set_coord_texture(int coord_texture)
{
    this->coord_texture = coord_texture;
}

void MyLabel::mouseReleaseEvent(QMouseEvent *event)
{
    emit clicked(*this->pixmap(), this->path);
    emit clicked(*this->pixmap(), coord_texture, this->path);
    Q_UNUSED(event);
}
