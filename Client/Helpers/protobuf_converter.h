#ifndef PROTOBUFCONVERTER_H
#define PROTOBUFCONVERTER_H

#include <grpc++/grpc++.h>

#include "Core/translation.h"
#include "Core/changes.h"
#include "Network/Proto/roosha_service.pb.h"
#include "Network/server_response.h"

namespace ProtobufConverter {
roosha::Translations translationsToProtobuf(const Translations &translations);
Translations translationsFromProtobuf(const roosha::Translations &rawTranslations);

ChangePtr changeFromProtobuf(const roosha::Change &rawChange);

RPCErrorStatus errorStatusFromGrpc(const grpc::Status &rawStatus);
const char* grpcStatusCodeToCString(const grpc::StatusCode &rawStatus);
QString rooshaCardChangeToString(const roosha::CardChange &rawChange);
QString rooshaFieldToString(const roosha::CardChange::Field &rawField);
};

#endif // PROTOBUFCONVERTER_H
