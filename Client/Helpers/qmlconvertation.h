#ifndef QMLCONVERTATION_H
#define QMLCONVERTATION_H

#include <QVariant>
#include <QMap>

#include <iostream>

class QmlConvertation {
 public:
    QmlConvertation();

    template<class T>
    static QVariant prepareToQml(QList<QSharedPointer<T> > data) {
        QList<QObject *> raw;
        raw.reserve(data.size());
        for (auto pointer : data) {
            raw.push_back(pointer.data());
        }
        return QVariant::fromValue(raw);
    }

    template<class T>
    static QVariant prepareToQml(QMap<QUuid, QSharedPointer<T> > data) {
        QMapIterator<QUuid, QSharedPointer<T>> i(data);
        QList<QObject *> raw;
        raw.reserve(data.size());

        while (i.hasNext()) {
            i.next();
            raw.push_back(i.value().data());
        }
        return QVariant::fromValue(raw);
    }
};

#endif // QMLCONVERTATION_H
