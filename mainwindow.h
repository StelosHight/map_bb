#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <myscene.h>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QLabel>
#include <map_object.h>
#include <mylabel.h>
#include <ui_dialog_scene_size_change.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void re_draw_panel();
    void on_radioButton_3_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_treeView_clicked(const QModelIndex &index);

    void add_map_object(int x, int y);
    void clear_del_object(int x, int y);
    void map_object_info(int x, int y);

    void selected_lab(const QPixmap pix, QString path);
    void selected_lab_scene(const QPixmap pix, int selected_texture, QString path);

    void load_map();
    void save_map();

    void on_pushButton_clicked();

    void on_toolButton_clicked();

    void show_dialog_change_size();
    void change_map_size();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QCursor cursor_del;
    QCursor cursor_add;

    QList<MyLabel*> llab;
    QList<map_object*> l_map_obj_info;
    int selected_texture;
    MyScene *scene;

    Ui_Dialog *ui_dialog;
    QDialog dialog_change_size;

    QList<map_object*> l_map_obj;
    QList<QPixmap> l_map_obj_pix;
    QMap <QPixmap*, QString> l_path_pix;

    QList<map_object*> l_map_obj_to_delete;
    QPixmap del_pix;

    QString selected_text_path;
    int selected_text_angle;

    QGridLayout *qGridLayout;

    int texture_size;
    //int max_z;

    void GetAllParents(const QModelIndex &index, QModelIndexList &indicies);

    void sort_by_z_coords(QList<map_object*> l_obj);
    //QTimer *re_draw;

protected:
    void keyPressEvent(QKeyEvent * event) override;
};

#endif // MAINWINDOW_H
