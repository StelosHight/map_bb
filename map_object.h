#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include <QGraphicsItem>
#include <QPainter>

class map_object : public QObject, public QGraphicsItem
{
    Q_OBJECT
private:

    int Coords[2];
    int h;
    int angle;
    int texture_size;

    QPixmap *texture;
    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    explicit map_object(int x, int y, int z, int h, int texture_size, QPixmap *texture, QObject *parent = nullptr);
    ~map_object();

    void change_texture_size(int size);

    qint64 get_texture_cacheKey();

    void set_h(int h);
    void set_angle(int angle);

    int get_coords(int i);
    int get_h();
    int get_angle();
    QPixmap *get_texture();
    void set_texture(QPixmap* new_texture);

signals:
    void info(int Coords[2], int h);
protected:
    /* Перегружаем метод нажатия мышью, для его перехвата
     * */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MAP_OBJECT_H
