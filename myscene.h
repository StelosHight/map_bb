#ifndef MYSCENE_H
#define MYSCENE_H
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>

class MyScene : public QGraphicsScene
{
    Q_OBJECT
private:
    QList<QGraphicsLineItem*> ll;
    bool left_but_press;
    int count_of_width_cells;
    int count_of_height_cells;
    int size;

public:
    MyScene(int size, QObject *parent = nullptr);
    ~MyScene();

    void change_cell_size(int size);
    void change_scene_size(int width, int height);
    int get_count_of_width_cells();
    int get_count_of_height_cells();

signals:
    void coord_clicked(int x, int y);
    void coord_double_clicked_left(int x, int y);
    void coord_clicked_right(int x, int y);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYSCENE_H
