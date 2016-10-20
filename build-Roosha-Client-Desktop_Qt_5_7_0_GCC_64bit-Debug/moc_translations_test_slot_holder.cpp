/****************************************************************************
** Meta object code from reading C++ file 'translations_test_slot_holder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Roosha-Client/Test/Network/translations_test_slot_holder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'translations_test_slot_holder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TranslationsTestSlotHolder_t {
    QByteArrayData data[9];
    char stringdata0[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TranslationsTestSlotHolder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TranslationsTestSlotHolder_t qt_meta_stringdata_TranslationsTestSlotHolder = {
    {
QT_MOC_LITERAL(0, 0, 26), // "TranslationsTestSlotHolder"
QT_MOC_LITERAL(1, 27, 24), // "translationSucceededSlot"
QT_MOC_LITERAL(2, 52, 0), // ""
QT_MOC_LITERAL(3, 53, 2), // "id"
QT_MOC_LITERAL(4, 56, 12), // "Translations"
QT_MOC_LITERAL(5, 69, 12), // "translations"
QT_MOC_LITERAL(6, 82, 21), // "translationFailedSlot"
QT_MOC_LITERAL(7, 104, 12), // "grpc::Status"
QT_MOC_LITERAL(8, 117, 6) // "status"

    },
    "TranslationsTestSlotHolder\0"
    "translationSucceededSlot\0\0id\0Translations\0"
    "translations\0translationFailedSlot\0"
    "grpc::Status\0status"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TranslationsTestSlotHolder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x0a /* Public */,
       6,    2,   29,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 7,    3,    8,

       0        // eod
};

void TranslationsTestSlotHolder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TranslationsTestSlotHolder *_t = static_cast<TranslationsTestSlotHolder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->translationSucceededSlot((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< Translations(*)>(_a[2]))); break;
        case 1: _t->translationFailedSlot((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< grpc::Status(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject TranslationsTestSlotHolder::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TranslationsTestSlotHolder.data,
      qt_meta_data_TranslationsTestSlotHolder,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TranslationsTestSlotHolder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TranslationsTestSlotHolder::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TranslationsTestSlotHolder.stringdata0))
        return static_cast<void*>(const_cast< TranslationsTestSlotHolder*>(this));
    return QObject::qt_metacast(_clname);
}

int TranslationsTestSlotHolder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
