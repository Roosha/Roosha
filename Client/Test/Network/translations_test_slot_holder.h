#ifndef TRANSLATIONSTESTSLOTHOLDER_H
#define TRANSLATIONSTESTSLOTHOLDER_H

#include <QObject>

#include <grpc++/grpc++.h>

#include "Network/Proto/roosha_service.pb.h"

using roosha::translation::Translations;

class TranslationsTestSlotHolder : public QObject {
    Q_OBJECT

public:
    TranslationsTestSlotHolder();
public slots:
    void translationSucceededSlot(quint32 id, Translations translations);
    void translationFailedSlot(quint32 id, grpc::Status status);
};

#endif // TRANSLATIONSTESTSLOTHOLDER_H
