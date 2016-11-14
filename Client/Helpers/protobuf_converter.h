#ifndef PROTOBUFCONVERTER_H
#define PROTOBUFCONVERTER_H

#include <grpc++/grpc++.h>

#include "Core/translation.h"
#include "Core/changes.h"
#include "Network/Proto/roosha_service.pb.h"
#include "Network/server_response.h"

namespace ProtobufConverter {
roosha::Translations translationsToProtobuf(const Translations& translations);
Translations translationsFromProtobuf(const roosha::Translations& rawTranslations);

roosha::Change changeToProtobuf(const ChangeSource& change);
roosha::Change changeToProtobuf(const EditElem& change);
roosha::Change changeToProtobuf(const InsertElem& change);
roosha::Change changeToProtobuf(const DeleteElem& change);
roosha::Change changeToProtobuf(const CreateCard& change);
roosha::Change changeToProtobuf(const DeleteCard& change);
QSharedPointer<IChange> changeFromProtobuf(const roosha::Change& rawChange);


RPCErrorStatus errorStatusFromGrpc(const grpc::Status &rawStatus);
};

#endif // PROTOBUFCONVERTER_H
