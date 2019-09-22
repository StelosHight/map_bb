#include "myscene.h"
#include <QDebug>

MyScene::MyScene(int size, QObject *parent) : QGraphicsScene(parent)
{
    left_but_press = false;

    count_of_width_cells = 4000;
    count_of_height_cells = 4000;
    this->size = size;

    int max_width = size * count_of_width_cells;
    int max_height = size * count_of_height_cells;

    this->setSceneRect(0, 0, max_width, max_height);

    for(int i = size, j = size; i < max_width || j < max_height; i += size, j += size)
    {
        if (j < max_height) ll.push_back(this->addLine(0, j, max_width, j, QPen(Qt::black))); // горизонтальная линия
        if (i < max_width) ll.push_back(this->addLine(i, 0, i, max_height,QPen(Qt::black))); // вертикальная линия
    }
}

MyScene::~MyScene()
{
    for(int i = 0; i < ll.size(); i+=2)
    {
        removeItem(ll[i]);
        removeItem(ll[i + 1]);
        delete ll[i];
        delete ll[i + 1];
    }
    ll.clear();
    //QGraphicsScene::~QGraphicsScene();
}

void MyScene::change_cell_size(int size)
{
    this->size = size;
    int max_width = size * count_of_width_cells;
    int max_height = size * count_of_height_cells;

    this->setSceneRect(0, 0, max_width, max_height);

    for(int i = size, j = size, l_x = 0; i < max_width || j < max_height; i += size, j += size)
    {
        if (j < max_height)
        {
            ll[l_x]->setLine(0, j, max_width, j); // горизонтальная линия
            l_x++;
        }
        if (i < max_width)
        {
            ll[l_x]->setLine(i, 0, i, max_height); // вертикальная линия
            l_x++;
        }
    }

    /*int max_size = size * count_of_cells;
    this->setSceneRect(0, 0, max_size, max_size);

    for(int i = size, j = size, l_x = 0; i < max_size; i += size, j += size, l_x += 2)
    {
        ll[l_x]->setLine(0, j, max_size, j); // горизонтальная линия
        ll[l_x + 1]->setLine(i, 0, i, max_size); // вертикальная линия
    }*/
}

void MyScene::change_scene_size(int width, int height)
{    
    int max_width = size * count_of_width_cells;
    int max_height = size * count_of_height_cells;

    for(int i = size, j = size; i < max_width || j < max_height; i += size, j += size)
    {
        if (j < max_height) {delete ll.last(); ll.removeLast(); } // горизонтальная линия
        if (i < max_width) {delete ll.last(); ll.removeLast(); } // вертикальная линия
    }

    count_of_width_cells = width;
    count_of_height_cells = height;

    max_width = size * count_of_width_cells;
    max_height = size * count_of_height_cells;

    this->setSceneRect(0, 0, max_width, max_height);

    for(int i = size, j = size; i < max_width || j < max_height; i += size, j += size)
    {
        if (j < max_height) ll.push_back(this->addLine(0, j, max_width, j, QPen(Qt::black))); // горизонтальная линия
        if (i < max_width) ll.push_back(this->addLine(i, 0, i, max_height,QPen(Qt::black))); // вертикальная линия
    }
}

int MyScene::get_count_of_width_cells()
{
    return count_of_width_cells;
}

int MyScene::get_count_of_height_cells()
{
    return count_of_height_cells;
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        left_but_press = true;
        emit coord_clicked(event->scenePos().x(), event->scenePos().y());
    }
    else if (event->button() == Qt::RightButton)
    {
        emit coord_clicked_right(event->scenePos().x(), event->scenePos().y());
    }
    QGraphicsScene::mousePressEvent(event);
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (left_but_press)
    {
        emit coord_clicked(event->scenePos().x(), event->scenePos().y());
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        left_but_press = false;
    }
}

void MyScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit coord_double_clicked_left(event->scenePos().x(), event->scenePos().y());
    }
}
