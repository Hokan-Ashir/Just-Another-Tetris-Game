/****************************************************************************
** Meta object code from reading C++ file 'irrwidget.h'
**
** Created: Wed Jul 2 17:53:58 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "headers/irrwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'irrwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QirrWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      56,   12,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QirrWidget[] = {
    "QirrWidget\0\0device\0"
    "updateIrrlichtQuery(IrrlichtDevice*)\0"
    "updateIrrlicht(IrrlichtDevice*)\0"
};

void QirrWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QirrWidget *_t = static_cast<QirrWidget *>(_o);
        switch (_id) {
        case 0: _t->updateIrrlichtQuery((*reinterpret_cast< IrrlichtDevice*(*)>(_a[1]))); break;
        case 1: _t->updateIrrlicht((*reinterpret_cast< IrrlichtDevice*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QirrWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QirrWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QirrWidget,
      qt_meta_data_QirrWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QirrWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QirrWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QirrWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QirrWidget))
        return static_cast<void*>(const_cast< QirrWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QirrWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QirrWidget::updateIrrlichtQuery(IrrlichtDevice * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
