#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(const QString& text="", QWidget* parent = nullptr);
    explicit MyLabel(int coord_texture, const QString& text="", QWidget* parent = nullptr);

    void set_path(QString path);

    void set_coord_texture(int coord_texture);

signals:
    void clicked(const QPixmap& pix,  QString path);
    void clicked(const QPixmap& pix, int coord_texture,  QString path);
protected:
    void mouseReleaseEvent(QMouseEvent* event);
private:
    int coord_texture;
    QString path;
};

#endif // MYLABEL_H
