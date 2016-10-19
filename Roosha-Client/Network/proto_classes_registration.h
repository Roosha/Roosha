#ifndef PROTO_CLASSES_REGISTRATION_H
#define PROTO_CLASSES_REGISTRATION_H

#include <QMetaType>

#include "Proto/translation_service.grpc.pb.h"
#include "Proto/translation_service.pb.h"
#include "Proto/commons.pb.h"

/**
  * This header file registers grpc and protobuf classes in QMetaType class,
  * so it's required to be included in any file where grpc or protobuf clsses are used in QVariant.
  * @see http://doc.qt.io/qt-5/qmetatype.html#Q_DECLARE_METATYPE
  */


Q_DECLARE_METATYPE(roosha::translation::Translations)
Q_DECLARE_METATYPE(grpc::Status)

#endif // PROTO_CLASSES_REGISTRATION_H
