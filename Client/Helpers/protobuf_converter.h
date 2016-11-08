#ifndef PROTOBUFCONVERTER_H
#define PROTOBUFCONVERTER_H

#include <grpc++/grpc++.h>

#include "Core/translation.h"
#include "Network/Proto/roosha_service.pb.h"
#include "Network/server_response.h"

namespace ProtobufConverter {
roosha::Translations translationsToProtobuf(const Translations& translations);
Translations translationsFromProtobuf(const roosha::Translations& rawTranslations);

RPCErrorStatus errorStatusFromGrpc(const grpc::Status &rawStatus);
};

#endif // PROTOBUFCONVERTER_H
