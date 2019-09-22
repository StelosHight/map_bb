/****************************************************************************
** Meta object code from reading C++ file 'main_wind.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../main_wind.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_wind.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Main_Wind_t {
    QByteArrayData data[19];
    char stringdata0[242];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Main_Wind_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Main_Wind_t qt_meta_stringdata_Main_Wind = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Main_Wind"
QT_MOC_LITERAL(1, 10, 13), // "re_draw_panel"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 24), // "on_radioButton_3_clicked"
QT_MOC_LITERAL(4, 50, 24), // "on_radioButton_2_clicked"
QT_MOC_LITERAL(5, 75, 22), // "on_radioButton_clicked"
QT_MOC_LITERAL(6, 98, 19), // "on_treeView_clicked"
QT_MOC_LITERAL(7, 118, 11), // "QModelIndex"
QT_MOC_LITERAL(8, 130, 5), // "index"
QT_MOC_LITERAL(9, 136, 14), // "add_map_object"
QT_MOC_LITERAL(10, 151, 1), // "x"
QT_MOC_LITERAL(11, 153, 1), // "y"
QT_MOC_LITERAL(12, 155, 15), // "map_object_info"
QT_MOC_LITERAL(13, 171, 12), // "selected_lab"
QT_MOC_LITERAL(14, 184, 3), // "pix"
QT_MOC_LITERAL(15, 188, 18), // "selected_lab_scene"
QT_MOC_LITERAL(16, 207, 16), // "selected_texture"
QT_MOC_LITERAL(17, 224, 8), // "load_map"
QT_MOC_LITERAL(18, 233, 8) // "save_map"

    },
    "Main_Wind\0re_draw_panel\0\0"
    "on_radioButton_3_clicked\0"
    "on_radioButton_2_clicked\0"
    "on_radioButton_clicked\0on_treeView_clicked\0"
    "QModelIndex\0index\0add_map_object\0x\0y\0"
    "map_object_info\0selected_lab\0pix\0"
    "selected_lab_scene\0selected_texture\0"
    "load_map\0save_map"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Main_Wind[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    1,   73,    2, 0x08 /* Private */,
       9,    2,   76,    2, 0x08 /* Private */,
      12,    2,   81,    2, 0x08 /* Private */,
      13,    1,   86,    2, 0x08 /* Private */,
      15,    2,   89,    2, 0x08 /* Private */,
      17,    0,   94,    2, 0x08 /* Private */,
      18,    0,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,
    QMetaType::Void, QMetaType::QPixmap,   14,
    QMetaType::Void, QMetaType::QPixmap, QMetaType::Int,   14,   16,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Main_Wind::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Main_Wind *_t = static_cast<Main_Wind *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->re_draw_panel(); break;
        case 1: _t->on_radioButton_3_clicked(); break;
        case 2: _t->on_radioButton_2_clicked(); break;
        case 3: _t->on_radioButton_clicked(); break;
        case 4: _t->on_treeView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: _t->add_map_object((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->map_object_info((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->selected_lab((*reinterpret_cast< const QPixmap(*)>(_a[1]))); break;
        case 8: _t->selected_lab_scene((*reinterpret_cast< const QPixmap(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->load_map(); break;
        case 10: _t->save_map(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Main_Wind::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Main_Wind.data,
      qt_meta_data_Main_Wind,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Main_Wind::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Main_Wind::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Main_Wind.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Main_Wind::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
