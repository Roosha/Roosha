#ifndef QMLCONVERTATION_H
#define QMLCONVERTATION_H

#include <QVariant>

class QmlConvertation
{
public:
    QmlConvertation();

    template<class T>
    static QVariant prepareToQml(QList<QSharedPointer<T> > data) {
        QList<QObject *> raw;
        raw.reserve(data.size());
        for(auto pointer : data) {
            raw.push_back(pointer.data());
        }
        return QVariant::fromValue(raw);
    }
};

#endif // QMLCONVERTATION_H
