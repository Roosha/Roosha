#ifndef PROTOBUFCONVERTER_H
#define PROTOBUFCONVERTER_H

#include <grpc++/grpc++.h>
#include <Network/Proto/learning.pb.h>
#include <GUI/Learning/LearningStrategy.h>

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

roosha::ScrutinyInput learningInputToProtobuf(const LearningInputType &type);
LearningInputType learningInputFromProtobuf(const roosha::ScrutinyInput &type);
roosha::ScrutinyView learningViewToProtobuf(const LearningViewType &type);
LearningViewType learningViewFromProtobuf(const roosha::ScrutinyView &type);
roosha::ScrutinyStatus cardDifficultyRateToProtobuf(const CardDifficulty::Rate &status);
CardDifficulty::Rate cardDifficultyRateFromProtobuf(const roosha::ScrutinyStatus &status);
};

#endif // PROTOBUFCONVERTER_H
