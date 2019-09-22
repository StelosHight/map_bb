#ifndef SVGREADER_H
#define SVGREADER_H

#include <QList>
#include <QPixmap>
#include "map_object.h"


class SVGReader
{
public:
    SVGReader();
    static QList<map_object *> getElements(const QString filename, QList<QPixmap> *l_map_obj_pix);
    //static QRectF getSizes(const QString filename);
};

#endif // SVGREADER_H
