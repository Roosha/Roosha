/****************************************************************************
** Meta object code from reading C++ file 'translation_service.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Roosha-Client/Network/translation_service.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'translation_service.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TranslationService_t {
    QByteArrayData data[14];
    char stringdata0[208];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TranslationService_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TranslationService_t qt_meta_stringdata_TranslationService = {
    {
QT_MOC_LITERAL(0, 0, 18), // "TranslationService"
QT_MOC_LITERAL(1, 19, 20), // "translationSucceeded"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 9), // "requestId"
QT_MOC_LITERAL(4, 51, 33), // "roosha::translation::Translat..."
QT_MOC_LITERAL(5, 85, 12), // "translations"
QT_MOC_LITERAL(6, 98, 17), // "translationFailed"
QT_MOC_LITERAL(7, 116, 12), // "grpc::Status"
QT_MOC_LITERAL(8, 129, 6), // "status"
QT_MOC_LITERAL(9, 136, 9), // "translate"
QT_MOC_LITERAL(10, 146, 6), // "source"
QT_MOC_LITERAL(11, 153, 13), // "timeoutMillis"
QT_MOC_LITERAL(12, 167, 23), // "proposeUserTranslations"
QT_MOC_LITERAL(13, 191, 16) // "userTranslations"

    },
    "TranslationService\0translationSucceeded\0"
    "\0requestId\0roosha::translation::Translations\0"
    "translations\0translationFailed\0"
    "grpc::Status\0status\0translate\0source\0"
    "timeoutMillis\0proposeUserTranslations\0"
    "userTranslations"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TranslationService[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       6,    2,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    2,   44,    2, 0x0a /* Public */,
      12,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 7,    3,    8,

 // slots: parameters
    QMetaType::UInt, QMetaType::QString, QMetaType::UInt,   10,   11,
    QMetaType::Void, 0x80000000 | 4,   13,

       0        // eod
};

void TranslationService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TranslationService *_t = static_cast<TranslationService *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->translationSucceeded((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< roosha::translation::Translations(*)>(_a[2]))); break;
        case 1: _t->translationFailed((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< grpc::Status(*)>(_a[2]))); break;
        case 2: { quint32 _r = _t->translate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = _r; }  break;
        case 3: _t->proposeUserTranslations((*reinterpret_cast< roosha::translation::Translations(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TranslationService::*_t)(quint32 , roosha::translation::Translations );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TranslationService::translationSucceeded)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TranslationService::*_t)(quint32 , grpc::Status );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TranslationService::translationFailed)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TranslationService::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TranslationService.data,
      qt_meta_data_TranslationService,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TranslationService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TranslationService::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TranslationService.stringdata0))
        return static_cast<void*>(const_cast< TranslationService*>(this));
    return QObject::qt_metacast(_clname);
}

int TranslationService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TranslationService::translationSucceeded(quint32 _t1, roosha::translation::Translations _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TranslationService::translationFailed(quint32 _t1, grpc::Status _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_AsyncRpcStatusListener_t {
    QByteArrayData data[1];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AsyncRpcStatusListener_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AsyncRpcStatusListener_t qt_meta_stringdata_AsyncRpcStatusListener = {
    {
QT_MOC_LITERAL(0, 0, 22) // "AsyncRpcStatusListener"

    },
    "AsyncRpcStatusListener"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AsyncRpcStatusListener[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void AsyncRpcStatusListener::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AsyncRpcStatusListener::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_AsyncRpcStatusListener.data,
      qt_meta_data_AsyncRpcStatusListener,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AsyncRpcStatusListener::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AsyncRpcStatusListener::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AsyncRpcStatusListener.stringdata0))
        return static_cast<void*>(const_cast< AsyncRpcStatusListener*>(this));
    return QThread::qt_metacast(_clname);
}

int AsyncRpcStatusListener::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
