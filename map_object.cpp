#include "map_object.h"

map_object::map_object(int x, int y, int z, int h, int texture_size, QPixmap *texture, QObject *parent) : QObject(parent), QGraphicsItem()
{
    Coords[0] = x;
    Coords[1] = y;
    this->setZValue(z);

    this->h = h;

    this->texture_size = texture_size;
    this->texture = texture;

    setX(x * texture_size);
    setY(y * texture_size);
}

map_object::~map_object()
{

}

void map_object::change_texture_size(int size)
{
    this->texture_size = size;
    setX(Coords[0] * texture_size);
    setY(Coords[1] * texture_size);
}

qint64 map_object::get_texture_cacheKey()
{
    return texture->cacheKey();
}

void map_object::set_h(int h)
{
    this->h = h;
}

void map_object::set_angle(int angle)
{
    this->angle = angle;
}

int map_object::get_coords(int i)
{
    if (i >= 0 && i < 2) return Coords[i];
    else return -1;
}

int map_object::get_h()
{
    return h;
}

int map_object::get_angle()
{
    return angle;
}

QPixmap *map_object::get_texture()
{
    return texture;
}

void map_object::set_texture(QPixmap *new_texture)
{
    this->texture = new_texture;
}

void map_object::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

QRectF map_object::boundingRect() const
{
    return QRectF(0, 0, texture_size, texture_size);
}

void map_object::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch (angle) {
    case 90:
        painter->translate(texture_size, 0);
        break;
    case 180:
        painter->translate(texture_size, texture_size);
        break;
    case 270:
        painter->translate(0, texture_size);
        break;
    case 0:
        painter->translate(0, 0);
        break;
    }
    painter->rotate(angle);

    painter->drawPixmap(0, 0, texture_size, texture_size, *texture);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}
