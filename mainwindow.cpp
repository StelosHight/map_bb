#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTime"
#include <QException>
#include <QDebug>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QKeyEvent>
#include <QFileDialog>
#include <QSvgGenerator>
#include <svgreader.h>
#include <QRegExpValidator>
#include <QRegExp>
#include <QCursor>
#include <QCommonStyle>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), ui_dialog(new Ui_Dialog), qGridLayout(new QGridLayout())
{
    ui->setupUi(this);
    ui_dialog->setupUi(&dialog_change_size);
    scene = nullptr;


    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setMouseTracking(true);

    model = new QFileSystemModel(this);

    model->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->setRootPath(QDir::currentPath() + "\\textures"));

    ui->scrollArea_2->widget()->setLayout(qGridLayout);

    connect(ui->action, SIGNAL(triggered()), this, SLOT(load_map()));
    connect(ui->action_2, SIGNAL(triggered()), this, SLOT(save_map()));
    connect(ui->action_4, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->change_size, SIGNAL(triggered()), this, SLOT(show_dialog_change_size()));
    connect(ui_dialog->pushButton_2, SIGNAL(clicked()), &dialog_change_size, SLOT(close()));
    connect(ui_dialog->pushButton, SIGNAL(clicked()), this, SLOT(change_map_size()));

    ui->param_z->setValidator(new QRegExpValidator(QRegExp("\\d{1,}")));

    cursor_del = QCursor(QPixmap("sys_textures/Cursors/c_black.png"), 0, 0);
    cursor_add = QCursor(QPixmap("sys_textures/Cursors/c_white.png"), 0, 0);
    del_pix = QPixmap("sys_textures/Others/del.png");

    ui->graphicsView->setCursor(cursor_add);
    QCommonStyle style;
    ui->toolButton->setIcon(style.standardIcon(QStyle::SP_BrowserReload));
}

MainWindow::~MainWindow()
{
    for(int i = 0; i < l_map_obj_to_delete.size(); i++)
    {
        scene->removeItem(l_map_obj_to_delete[i]);
        delete l_map_obj_to_delete[i];
    }
    l_map_obj_to_delete.clear();

    for(int i = 0; i < l_map_obj.size(); i++)
    {
        scene->removeItem(l_map_obj[i]);
        delete l_map_obj[i];
    }
    l_map_obj.clear();
    l_path_pix.clear();
    l_map_obj_pix.clear();

    for(int i = 0; i < llab.size(); i++)
    {
        qGridLayout->removeWidget(llab[i]);
        delete llab[i];
    }
    llab.clear();

    if (scene != nullptr) delete scene;
    delete model;
    delete qGridLayout;
    delete ui;
}

void MainWindow::re_draw_panel()
{
    if (scene == nullptr)
    {
        scene = new MyScene(texture_size);
        connect(scene, SIGNAL(coord_clicked(int, int)), this, SLOT(add_map_object(int, int)));
        connect(scene, SIGNAL(coord_clicked_right(int, int)), this, SLOT(map_object_info(int, int)));
        connect(scene, SIGNAL(coord_double_clicked_left(int, int)), this, SLOT(clear_del_object(int, int)));
        ui->graphicsView->setScene(scene);
    }
    else {
        scene->change_cell_size(texture_size);
        for(int i = 0; i < l_map_obj.size(); i++)
        {
            l_map_obj[i]->change_texture_size(texture_size);
        }

        for(int i = 0; i < l_map_obj_to_delete.size(); i++)
        {
            l_map_obj_to_delete[i]->change_texture_size(texture_size);
        }
    }
}

void MainWindow::on_radioButton_3_clicked()
{
    texture_size = 16;
    re_draw_panel();
}

void MainWindow::on_radioButton_2_clicked()
{
    texture_size = 24;
    re_draw_panel();
}

void MainWindow::on_radioButton_clicked()
{
    texture_size = 32;
    re_draw_panel();
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    l_map_obj_info.clear();
    selected_texture = -1;

    if (ui->param_h->isEnabled()) ui->param_h->setEnabled(false);
    if (ui->param_z->isEnabled()) ui->param_z->setEnabled(false);
    if (ui->pushButton->isEnabled()) ui->pushButton->setEnabled(false);

    if (ui->param_x->text() != "") ui->param_x->setText("");
    if (ui->param_y->text() != "") ui->param_y->setText("");
    if (ui->param_z->text() != "") ui->param_z->setText("");
    if (ui->param_h->text() != "") ui->param_h->setText("");

    QModelIndexList list;
    GetAllParents(index, list);

    QString path = "";

    for(int i = list.size() - 1; i >= 0; i--)
    {
        path += list[i].data(Qt::DisplayRole).toString();
        if (i != 0) path +=  "/";
    }

    QFileInfoList dirContent = QDir(path).entryInfoList(QStringList()<< "*",QDir::Files);

    for(int i = 0; i < llab.size(); i++)
    {
        qGridLayout->removeWidget(llab[i]);
        delete llab[i];
    }
    llab.clear();

    int in_row_cell = ui->scrollArea_2->size().width() / 50;
    for(int i = 0; i < dirContent.size(); i++)
    {
        llab.push_back(new MyLabel());
        connect(llab.last(), SIGNAL(clicked(const QPixmap, QString)), this, SLOT(selected_lab(const QPixmap, QString)));
        llab.last()->setPixmap(QPixmap(dirContent[i].filePath()));
        llab.last()->set_path(dirContent[i].filePath());
        llab.last()->setToolTip(dirContent[i].filePath().split("/").last());
        qGridLayout->addWidget(llab.last(), i / in_row_cell, i % in_row_cell, Qt::AlignHCenter | Qt::AlignVCenter);
    }
    ui->label->setText("Текстуры из папки " + path.split("/").last());
}

void MainWindow::GetAllParents(const QModelIndex &index, QModelIndexList &indicies) // Определяет полный путь
{
    indicies.push_back(index);
    if (index.parent().data(Qt::DisplayRole).toString() != QDir::currentPath().split("/").last()) GetAllParents(index.parent(), indicies);
}

void MainWindow::sort_by_z_coords(QList<map_object *> l_obj)
{
    map_object* temp;
    for(int i = 0, ind = 0; i < l_obj.size() - 1; i++)
    {
        temp = nullptr;
        for (int j = i + 1; j < l_obj.size(); j++)
        {
            if (l_obj[i]->zValue() > l_obj[j]->zValue())
            {
                temp = l_obj[j];
                ind = j;
            }
        }
        if (temp != nullptr)
        {
            l_obj[ind] = l_obj[i];
            l_obj[i] = temp;
        }
    }
}

void MainWindow::add_map_object(int x, int y)
{
    if (ui->cur_mode->text() == "Удаление" && scene != nullptr)
    {
        int coord_z = 0;
        int true_x = x / texture_size;
        int true_y = y / texture_size;

        for(int i = 0; i < l_map_obj.size(); i++)
        {
            if(l_map_obj[i]->get_coords(0) == true_x && l_map_obj[i]->get_coords(1) == true_y && l_map_obj[i]->zValue() == coord_z)
            {
                coord_z = l_map_obj[i]->zValue() + 1;
            }
        }

        if (coord_z != 0)
        {
            bool checker = true;
            for(int i = 0; i < l_map_obj_to_delete.size(); i++)
            {
                if(l_map_obj_to_delete[i]->get_coords(0) == true_x && l_map_obj_to_delete[i]->get_coords(1) == true_y)
                {
                    checker = false;
                    break;
                }
            }

            if (checker)
            {
                l_map_obj_to_delete.push_back(new map_object(true_x, true_y, coord_z, 0, texture_size, &del_pix));
                l_map_obj_to_delete.last()->set_angle(0);
                scene->addItem(l_map_obj_to_delete.last());
            }
        }

    }
    else if (ui->cur_picked_texture->pixmap() == nullptr)
    {
        //QMessageBox::critical(this, "Текстура не выбрана", "Выберите текстуру для добавления на сцену.");
    }
    else if (ui->cur_picked_texture->pixmap()->size().width() != 0 && ui->cur_picked_texture->pixmap()->size().height() != 0){
        int coord_z = 0, last_text = -1;
        int true_x = x / texture_size;
        int true_y = y / texture_size;

        /*if (ui->max_cur_z->text() != "")
        {
            coord_z = ui->max_cur_z->text().toInt() + 1;
            for(int i = 0; i < l_map_obj.size(); i++)
            {
                if(l_map_obj[i]->get_coords(0) == true_x && l_map_obj[i]->get_coords(1) == true_y && l_map_obj[i]->zValue() >= coord_z)
                {
                    l_map_obj[i]->setZValue(l_map_obj[i]->zValue() + 1);
                }
            }
        }
        else
        {
            for(int i = 0; i < l_map_obj.size(); i++)
            {
                if(l_map_obj[i]->get_coords(0) == true_x && l_map_obj[i]->get_coords(1) == true_y && l_map_obj[i]->zValue() == coord_z)
                {
                    coord_z = l_map_obj[i]->zValue() + 1;
                }
            }
        }*/

        for(int i = 0; i < l_map_obj.size(); i++)
        {
            if(l_map_obj[i]->get_coords(0) == true_x && l_map_obj[i]->get_coords(1) == true_y && l_map_obj[i]->zValue() == coord_z)
            {
                coord_z = l_map_obj[i]->zValue() + 1;
                last_text = i;
            }
        }

        if (last_text != -1 ? l_map_obj[last_text]->get_texture() != l_path_pix.key(selected_text_path, nullptr) ? true : l_map_obj[last_text]->get_angle() != selected_text_angle : true)
        {
            bool check = true;
            QPixmap* sel = nullptr;

            for(int i = 0; i < l_map_obj_pix.size(); i++)
            {
                if(l_map_obj_pix[i].cacheKey() == ui->cur_picked_texture->pixmap()->cacheKey())
                {
                    check = false;
                    sel = &l_map_obj_pix[i];
                    break;
                }
            }
            if (check)
            {
                sel = l_path_pix.key(selected_text_path, nullptr);

                if (sel == nullptr)
                {
                    l_map_obj_pix.push_back(QPixmap(selected_text_path));
                    l_path_pix.insert(&l_map_obj_pix.last(), selected_text_path);
                    sel = &l_map_obj_pix.last();
                }
            }

            l_map_obj.push_back(new map_object(true_x, true_y, coord_z, 0, texture_size, sel));
            l_map_obj.last()->set_angle(selected_text_angle);
            scene->addItem(l_map_obj.last());
        }
    }
}

void MainWindow::clear_del_object(int x, int y)
{
    if (ui->cur_mode->text() == "Удаление" && scene != nullptr)
    {
        int true_x = x / texture_size;
        int true_y = y / texture_size;
        for(int i = 0; i < l_map_obj_to_delete.size(); i++)
        {
            if(l_map_obj_to_delete[i]->get_coords(0) == true_x && l_map_obj_to_delete[i]->get_coords(1) == true_y)
            {
                scene->removeItem(l_map_obj_to_delete[i]);
                delete l_map_obj_to_delete[i];
                l_map_obj_to_delete.removeAt(i);

                break;
            }
        }
    }
}

void MainWindow::map_object_info(int x, int y)
{
    int true_x = x / texture_size;
    int true_y = y / texture_size;

    for(int i = 0; i < llab.size(); i++)
    {
        qGridLayout->removeWidget(llab[i]);
        delete llab[i];
    }
    llab.clear();

    l_map_obj_info.clear();

    for(int i = 0; i < l_map_obj.size(); i++)
    {
        if(l_map_obj[i]->get_coords(0) == true_x && l_map_obj[i]->get_coords(1) == true_y)
        {
            l_map_obj_info.push_back(l_map_obj[i]);
        }
    }

    sort_by_z_coords(l_map_obj_info);

    int in_row_cell = ui->scrollArea_2->size().width() / 50;
    for(int i = 0; i < l_map_obj_info.size(); i++)
    {
        llab.push_back(new MyLabel(i));
        connect(llab.last(), SIGNAL(clicked(const QPixmap, int, QString)), this, SLOT(selected_lab_scene(const QPixmap, int, QString)));
        switch (l_map_obj_info[i]->get_angle()) {
        case 90:
            llab.last()->setPixmap(l_map_obj_info[i]->get_texture()->
                                   transformed(QTransform().translate(texture_size, 0).rotate(90)));
            break;
        case 180:
            llab.last()->setPixmap(l_map_obj_info[i]->get_texture()->
                                   transformed(QTransform().translate(texture_size, texture_size).rotate(180)));
            break;
        case 270:
            llab.last()->setPixmap(l_map_obj_info[i]->get_texture()->
                                   transformed(QTransform().translate(0, texture_size).rotate(270)));
            break;
        case 0:
            llab.last()->setPixmap(QPixmap(*l_map_obj_info[i]->get_texture()));
            break;
        }
        llab.last()->set_path(l_path_pix[l_map_obj_info[i]->get_texture()]);
        llab.last()->setToolTip(l_path_pix[l_map_obj_info[i]->get_texture()].split("/").last());
        qGridLayout->addWidget(llab.last(), i / in_row_cell, i % in_row_cell, Qt::AlignHCenter | Qt::AlignVCenter);
    }

    ui->label->setText("Текстуры из сцены: " + QString::number(true_x) + "x" + QString::number(true_y));
}

void MainWindow::selected_lab(const QPixmap pix, QString path)
{
    ui->cur_picked_texture->setPixmap(pix);
    selected_text_path = path;
    selected_text_angle = 0;
}

void MainWindow::selected_lab_scene(const QPixmap pix, int selected_texture, QString path)
{
    if (!ui->param_h->isEnabled()) ui->param_h->setEnabled(true);
    if (!ui->param_z->isEnabled()) ui->param_z->setEnabled(true);
    if (!ui->pushButton->isEnabled()) ui->pushButton->setEnabled(true);

    this->selected_texture = selected_texture;
    ui->cur_picked_texture->setPixmap(pix);
    ui->param_x->setText(QString::number(l_map_obj_info[selected_texture]->get_coords(0)));
    ui->param_y->setText(QString::number(l_map_obj_info[selected_texture]->get_coords(1)));
    ui->param_z->setText(QString::number(l_map_obj_info[selected_texture]->zValue()));
    ui->param_h->setText(QString::number(l_map_obj_info[selected_texture]->get_h()));
    selected_text_angle = l_map_obj_info[selected_texture]->get_angle();
    selected_text_path = path;
}

void MainWindow::load_map()
{
    QString newPath = QFileDialog::getOpenFileName(this, trUtf8("Загрузить BBMap"), "", tr("(*.BBMap)"));

    if (newPath.isEmpty())
        return;

    l_map_obj_info.clear();

    if (l_map_obj.size() != 0)
    {
        for(int i = 0; i < l_map_obj.size(); i++)
        {
            scene->removeItem(l_map_obj[i]);
            delete l_map_obj[i];
        }
        l_map_obj.clear();
    }

    try {
        QFile file(newPath);
        if (file.open(QIODevice::ReadOnly)) {

            QDataStream in(&file);

            int n, x, y, z, h, angle;

            QString path;

            QPixmap* sel = nullptr;

            in >> n;
            in >> texture_size;

            re_draw_panel();

            for(int i = 0; i < n; i++)
            {
                in >> x;
                in >> y;
                in >> z;
                in >> h;
                in >> path;
                in >> angle;


                sel = l_path_pix.key(path, nullptr);

                if (sel == nullptr)
                {
                    l_map_obj_pix.push_back(QPixmap(path));
                    l_path_pix.insert(&l_map_obj_pix.last(), path);
                    sel = &l_map_obj_pix.last();
                }

                l_map_obj.push_back(new map_object(x, y, z, h, texture_size, sel));
                l_map_obj.last()->set_angle(angle);
                scene->addItem(l_map_obj.last());
            }

            int size_width, size_height;

            in >> size_width;
            in >> size_height;

            scene->change_scene_size(size_width, size_height);
            re_draw_panel();

            file.close();

            switch(texture_size)
            {
            case 16:
                ui->radioButton_3->setChecked(true);
                break;
            case 24:
                ui->radioButton_2->setChecked(true);
                break;
            case 32:
                ui->radioButton->setChecked(true);
                break;
            }

            for(int i = 0; i < llab.size(); i++)
            {
                qGridLayout->removeWidget(llab[i]);
                delete llab[i];
            }
            llab.clear();
            ui->label->setText("Текстуры");

            ui->cur_picked_texture->setPixmap(QPixmap());
            ui->param_x->setText("");
            ui->param_y->setText("");
            ui->param_z->setText("");
            ui->param_h->setText("");

            QMessageBox::information(this, "Успех!", "Карта загружена!");
        }

        /*l_map_obj = SVGReader::getElements(newPath, &l_map_obj_pix);

        if (l_map_obj.size() != 0)
        {
            texture_size = l_map_obj_pix[0].size().width();

            switch(texture_size)
            {
            case 16:
                ui->radioButton_3->setChecked(true);
                break;
            case 24:
                ui->radioButton_2->setChecked(true);
                break;
            case 32:
                ui->radioButton->setChecked(true);
                break;
            }

            if (scene == nullptr)
            {
                scene = new MyScene(texture_size);
                connect(scene, SIGNAL(coord_clicked(int, int)), this, SLOT(add_map_object(int, int)));
                connect(scene, SIGNAL(coord_clicked_right(int, int)), this, SLOT(map_object_info(int, int)));
                ui->graphicsView->setScene(scene);
            }
            for(int i = 0; i < l_map_obj.size(); i++)
            {
                scene->addItem(l_map_obj[i]);
            }

            for(int i = 0; i < llab.size(); i++)
            {
                qGridLayout->removeWidget(llab[i]);
                delete llab[i];
            }
            llab.clear();
            ui->label->setText("Текстуры");

            ui->cur_picked_texture->setPixmap(QPixmap());
            ui->param_x->setText("");
            ui->param_y->setText("");
            ui->param_z->setText("");
            ui->param_h->setText("");

            QMessageBox::information(this, "Успех!", "Карта загружена!");
        }
        else {
            QMessageBox::critical(this, "Ошибка!", "Карта не загружена!");
        }
        */

    } catch (...) {
        QMessageBox::critical(this, "Ошибка!", "Выгрузить элементы не удалось!");
    }
}

void MainWindow::save_map()
{
    QString newPath = QFileDialog::getSaveFileName(this, trUtf8("Сохранить BBMap"), "", tr("(*.BBmap)"));

    QFile file(newPath);
    if (file.open(QIODevice::WriteOnly)) {

        QDataStream out(&file);

        int n, x, y, z, h, angle;

        QString path;

        n = l_map_obj.size();

        out << n;
        out << texture_size;

        for(int i = 0; i < n; i++)
        {
            x = l_map_obj[i]->get_coords(0);
            y = l_map_obj[i]->get_coords(1);
            z = static_cast<int>(l_map_obj[i]->zValue());
            h = l_map_obj[i]->get_h();
            path = l_path_pix[l_map_obj[i]->get_texture()];
            angle = l_map_obj[i]->get_angle();

            out << x;
            out << y;
            out << z;
            out << h;
            out << path;
            out << angle;
        }

        int size_width = scene->get_count_of_width_cells(), size_height = scene->get_count_of_height_cells();

        out << size_width;
        out << size_height;

        file.close();
    }
    /*QString newPath = QFileDialog::getSaveFileName(this, trUtf8("Сохранить SVG"), "", tr("(*.svg)"));

    if (newPath.isEmpty())
        return;

    QSvgGenerator generator;        // Создаём объект генератора файла
    generator.setFileName(newPath);    // Устанавливаем путь к файлу, куда будет сохраняться векторная графика
    generator.setSize(QSize(scene->width(), scene->height()));  // Устанавливаем размеры рабочей области документа в миллиметрах
    generator.setViewBox(QRect(0, 0, scene->width(), scene->height())); // Устанавливаем рабочую область в координатах
    generator.setTitle(newPath.split("/").last());            // Титульное название документа
    generator.setDescription(trUtf8("Карта BB для BB49"));    // Описание документа

    // С помощью класса QPainter
    QPainter painter;
    painter.begin(&generator);  // Устанавливаем устройство/объект в котором будем производить отрисовку
    scene->render(&painter);    // Отрисовываем содержимое сцены с помощью painter в целевое устройство/объект
    painter.end();              // Заканчиваем отрисовку
    */
    QMessageBox::information(this, "Успех!", "Карта сохранена!");
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_Delete == event->key())
    {
        if (l_map_obj_info.size() != 0 && selected_texture != -1)
        {
            for(int i = selected_texture + 1; i < l_map_obj_info.size(); i++) // всех последующих на один слой вниз
            {
                l_map_obj_info[i]->setZValue(l_map_obj_info[i]->zValue() - 1);
            }

            scene->removeItem(l_map_obj_info[selected_texture]);

            ui->cur_picked_texture->setPixmap(QPixmap());
            ui->param_x->setText("");
            ui->param_y->setText("");
            ui->param_z->setText("");
            ui->param_h->setText("");

            qGridLayout->removeWidget(llab[selected_texture]);
            delete llab[selected_texture];
            llab.removeAt(selected_texture);

            for(int i = 0; i < llab.size(); i++)
            {
                llab[i]->set_coord_texture(i);
                qGridLayout->addWidget(llab[i], i / 2, i % 2, Qt::AlignHCenter | Qt::AlignVCenter);
            }

            l_map_obj.removeOne(l_map_obj_info[selected_texture]);
            delete l_map_obj_info[selected_texture];
            l_map_obj_info.removeAt(selected_texture);
        }
    }
    else if ((event->key() == Qt::Key_W) && (event->modifiers() == Qt::ControlModifier))
    {
        QMessageBox::warning(this, "Ctrl + W", "Ctrl + W");
    }
    else if ((event->key() == Qt::Key_D) && (event->modifiers() == Qt::ControlModifier))
    {
        if (ui->cur_mode->text() == "Добавление")
        {
            ui->cur_mode->setText("Удаление");
            ui->graphicsView->setCursor(cursor_del);
        }
        else if (ui->cur_mode->text() == "Удаление")
        {
            for(int i = 0; i < l_map_obj_to_delete.size(); i++)
            {
                for(int j = 0; j < l_map_obj.size(); j++)
                {
                    if(l_map_obj[j]->get_coords(0) == l_map_obj_to_delete[i]->get_coords(0) &&
                            l_map_obj[j]->get_coords(1) == l_map_obj_to_delete[i]->get_coords(1))
                    {
                        scene->removeItem(l_map_obj[j]);
                        delete l_map_obj[j];
                        l_map_obj.removeAt(j);
                        j--;
                    }
                }
                scene->removeItem(l_map_obj_to_delete[i]);
                delete l_map_obj_to_delete[i];
            }
            l_map_obj_to_delete.clear();

            for(int i = 0; i < llab.size(); i++)
            {
                qGridLayout->removeWidget(llab[i]);
                delete llab[i];
            }
            llab.clear();

            ui->cur_mode->setText("Добавление");
            ui->graphicsView->setCursor(cursor_add);
        }
    }
    else if ((event->key() == Qt::Key_R) && (event->modifiers() == Qt::ControlModifier))
    {
        on_toolButton_clicked();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->param_z->text() != "")
    {
        int coord_z = ui->param_z->text().toInt();

        if (l_map_obj_info[selected_texture]->zValue() != coord_z)
        {
            if (coord_z < l_map_obj_info.size())
            {
                map_object* tmp;

                tmp = l_map_obj_info[selected_texture];

                if (coord_z > l_map_obj_info[selected_texture]->zValue())
                {
                    for(int i = selected_texture + 1; i <= coord_z; i++) // на один слой вниз
                    {
                        l_map_obj_info[i]->setZValue(l_map_obj_info[i]->zValue() - 1);
                        l_map_obj_info[i - 1] = l_map_obj_info[i];
                    }
                }
                else
                {
                    for(int i = selected_texture - 1, n = coord_z; i >= n; i--) // на один слой вверх
                    {
                        l_map_obj_info[i]->setZValue(l_map_obj_info[i]->zValue() + 1);
                        l_map_obj_info[i + 1] = l_map_obj_info[i];
                    }
                }

                l_map_obj_info[coord_z] = tmp;

                l_map_obj_info[coord_z]->setZValue(coord_z);

                selected_texture = coord_z;

                sort_by_z_coords(l_map_obj_info);

                for(int i = 0; i < llab.size(); i++)
                {
                    qGridLayout->removeWidget(llab[i]);
                    delete llab[i];
                }
                llab.clear();

                int in_row_cell = ui->scrollArea_2->size().width() / 50;

                for(int i = 0; i < l_map_obj_info.size(); i++)
                {
                    llab.push_back(new MyLabel(i));
                    connect(llab.last(), SIGNAL(clicked(const QPixmap, int, QString)), this, SLOT(selected_lab_scene(const QPixmap, int, QString)));
                    switch (l_map_obj_info[i]->get_angle()) {
                    case 90:
                        llab.last()->setPixmap(l_map_obj_info[i]->get_texture()->
                                               transformed(QTransform().translate(texture_size, 0).rotate(90)));
                        break;
                    case 180:
                        llab.last()->setPixmap(l_map_obj_info[i]->get_texture()->
                                               transformed(QTransform().translate(texture_size, texture_size).rotate(180)));
                        break;
                    case 270:
                        llab.last()->setPixmap(l_map_obj_info[i]->get_texture()->
                                               transformed(QTransform().translate(0, texture_size).rotate(270)));
                        break;
                    case 0:
                        llab.last()->setPixmap(QPixmap(*l_map_obj_info[i]->get_texture()));
                        break;
                    }
                    llab.last()->set_path(l_path_pix[l_map_obj_info[i]->get_texture()]);
                    llab.last()->setToolTip(l_path_pix[l_map_obj_info[i]->get_texture()].split("/").last());
                    qGridLayout->addWidget(llab.last(), i / in_row_cell, i % in_row_cell, Qt::AlignHCenter | Qt::AlignVCenter);
                }
            }
            else QMessageBox::warning(this, "Ошибка!", "Нельзя переместить на слой\nвыше максимального.");
        }
    }
    /*if (l_map_obj_info[selected_texture]->get_texture_cacheKey() != ui->cur_picked_texture->pixmap()->cacheKey())
    {
        bool check = true;
        QPixmap* sel = nullptr;

        for(int i = 0; i < l_map_obj_pix.size(); i++)
        {
            if(l_map_obj_pix[i].cacheKey() == ui->cur_picked_texture->pixmap()->cacheKey())
            {
                check = false;
                sel = &l_map_obj_pix[i];
                break;
            }
        }
        if (check)
        {
            l_map_obj_pix.push_back(QPixmap(*ui->cur_picked_texture->pixmap()));
            sel = &l_map_obj_pix.last();
        }

        l_map_obj_info[selected_texture]->set_texture(sel);
        l_map_obj_info[selected_texture]->update();

        for(int i = 0; i < llab.size(); i++)
        {
            qGridLayout->removeWidget(llab[i]);
        }

        for(int i = 0; i < l_map_obj_info.size(); i++)
        {
            llab.push_back(new MyLabel(i));
            connect(llab.last(), SIGNAL(clicked(const QPixmap, int)), this, SLOT(selected_lab_scene(const QPixmap, int)));
            llab.last()->setPixmap(QPixmap(*l_map_obj_info[i]->get_texture()));
            qGridLayout->addWidget(llab.last(), i / 2, i % 2, Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }*/
    if (l_map_obj_info[selected_texture]->get_angle() != selected_text_angle)
    {
        l_map_obj_info[selected_texture]->set_angle(selected_text_angle);
        l_map_obj_info[selected_texture]->update();
    }
    if (ui->param_h->text() != "")
    {
        int h = ui->param_h->text().toInt();
        if (h > 0)
        {
            l_map_obj_info[selected_texture]->set_h(h);
        }
    }
}

void MainWindow::on_toolButton_clicked()
{
    if (ui->cur_picked_texture->pixmap() != nullptr){
        ui->cur_picked_texture->setPixmap(ui->cur_picked_texture->pixmap()->
                                          transformed(QTransform().translate(ui->cur_picked_texture->pixmap()->size().width(), 0).rotate(90)));

        selected_text_angle += 90;
        if (selected_text_angle >= 360) selected_text_angle = 0;
    }
}

void MainWindow::show_dialog_change_size()
{
    if (scene != nullptr)
    {
        int size_width = scene->get_count_of_width_cells(), size_height = scene->get_count_of_height_cells();

        ui_dialog->spinBox->setValue(size_width);
        ui_dialog->spinBox_2->setValue(size_height);

        dialog_change_size.exec();
    }
}

void MainWindow::change_map_size()
{
    dialog_change_size.close();
    int size_width = scene->get_count_of_width_cells(), size_height = scene->get_count_of_height_cells();
    if (size_width != ui_dialog->spinBox->value()
            || size_height != ui_dialog->spinBox_2->value())
    {
        for(int j = 0; j < l_map_obj.size(); j++)
        {
            if(l_map_obj[j]->get_coords(0) >= size_width ||
                    l_map_obj[j]->get_coords(1) >= size_height)
            {
                scene->removeItem(l_map_obj[j]);
                delete l_map_obj[j];
                l_map_obj.removeAt(j);
                j--;
            }
        }

        for(int i = 0; i < llab.size(); i++)
        {
            qGridLayout->removeWidget(llab[i]);
            delete llab[i];
        }
        llab.clear();

        scene->change_scene_size(ui_dialog->spinBox->value(), ui_dialog->spinBox_2->value());
        //re_draw_panel();
    }
}
