#ifndef TRANSLATIONSERVICE_H
#define TRANSLATIONSERVICE_H

#include "Proto/translation_service.grpc.pb.h"
#include "Proto/translation_service.pb.h"

#include <memory>

#include <QString>

class TranslationService {
 public:
    TranslationService(const grpc::string &target);

    roosha::translation::Translations&& translate(QString source);

    void proposeUserTranslation(roosha::translation::Translation userTranslation);

 private:
    std::unique_ptr<roosha::translation::RooshaTranslationService::Stub> stub_;
};

#endif // TRANSLATIONSERVICE_H
