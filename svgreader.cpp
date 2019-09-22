#include "svgreader.h"
#include <QPen>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QStringList>
#include <QByteArray>

SVGReader::SVGReader()
{

}

QList<map_object *> SVGReader::getElements(const QString filename, QList<QPixmap> *l_map_obj_pix)
{
    QList<map_object *> ItemList;    // Объявим в стеке список прямоугольников

    QDomDocument doc;       // объект документа
    QFile file(filename);   // Открываем наш SVG-файл
    // Если он не открылся или не удалось передать содержимое в QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return ItemList;    // то возвратим список, но пустой

    // Ищем в документе все объекты с тегом g
    QDomNodeList gList = doc.elementsByTagName("g");
    // Начинаем их перебирать
    for (int i = 0; i < gList.size(); i++) {
        QDomNode gNode = gList.item(i);     // Выделяем из списка ноду
        QDomElement image = gNode.firstChildElement("image");    // И ищем в ней элемент c тегом image
        // Если полученный элементы не нулевой, то
        if (image.isNull()){
            continue;
        } else {
            // начинаем формировать объект

            bool check = true;
            QPixmap* sel = nullptr;

            const QByteArray &bytes = QByteArray::fromBase64(image.attribute("xlink:href").split("base64,").last().toLatin1());
            QPixmap qpix;
            qpix.loadFromData(bytes, "PNG");

            int coord_z = 0;
            int true_x;
            int true_y;

            // Забираем размеры из тега gNode
            QDomElement gElement = gNode.toElement();

            QString transform = gElement.attribute("transform").split("(").last().split(")").first();
            true_x = transform.split(",")[4].toInt() / qpix.size().width();
            true_y = transform.split(",")[5].toInt() / qpix.size().width();


            for(int i = 0; i < ItemList.size(); i++)
            {
                if(ItemList[i]->get_coords(0) == true_x && ItemList[i]->get_coords(1) == true_y && ItemList[i]->zValue() == coord_z)
                {
                    coord_z = ItemList[i]->zValue() + 1;
                }
            }

            for(int i = 0; i < l_map_obj_pix->size(); i++)
            {
                if(l_map_obj_pix->at(i).cacheKey() == qpix.cacheKey())
                {
                    check = false;
                    sel = &(*l_map_obj_pix)[i];
                    break;
                }
            }
            if (check)
            {
                l_map_obj_pix->push_back(QPixmap(qpix));
                sel = &l_map_obj_pix->last();
            }

            ItemList.push_back(new map_object(true_x, true_y, coord_z, 0, qpix.size().width(), sel));  // добавляем объект в список
        }
    }
    file.close();
    return ItemList;
}

//QRectF SVGReader::getSizes(const QString filename)
//{
//    QDomDocument doc;       // инициализируем в стеке объект QDomDocument
//    QFile file(filename);   // Открываем наш SVG-файл
//    // Если он не открылся или не удалось передать содержимое в QDocDocument
//    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
//        return QRectF(0,0,200,200); // то возвратим значения для сцены по умолчанию

//    /* Далее забираем список элементов с тегом svg.
//     * В случае, если список элементов будет не пустой,
//     * то заберём размеры графической сцены.
//     * */
//    QDomNodeList list = doc.elementsByTagName("svg");
//    if(list.length() > 0) {
//        QDomElement svgElement = list.item(0).toElement();
//        QStringList parameters = svgElement.attribute("viewBox").split(" ");
//        return QRectF(parameters.at(0).toInt(),
//                      parameters.at(1).toInt(),
//                      parameters.at(2).toInt(),
//                      parameters.at(3).toInt());
//    }
//    return QRectF(0,0,200,200);
//}
