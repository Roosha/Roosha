#include "protobuf_converter.h"

roosha::Translations ProtobufConverter::translationsToProtobuf(const Translations& translations) {
    roosha::Translations result;
    result.set_source(translations[0]->getSource().toStdString());
    for (const auto& translation: translations) {
        roosha::Translation *reqTranslation = result.add_translation();
        reqTranslation->set_provider(roosha::TranslationProvider::UNKNOWN);
        for (auto target: translation->getTarget()) {
            reqTranslation->add_target(target.toStdString());
        }
        for (auto example: translation->getExamples()) {
            reqTranslation->add_example(example.toStdString());
        }
    }

    return result;
}

Translations ProtobufConverter::translationsFromProtobuf(const roosha::Translations& rawTranslations) {
    Translations result;
    for (int i = 0; i < rawTranslations.translation_size(); i++) {
        auto raw_translation = rawTranslations.translation(i);
        Translation *translation = new Translation;
        translation->setSource(QString::fromStdString(rawTranslations.source()));

        QStringList targets;
        for (int i = 0; i < raw_translation.target_size(); i++) {
            targets.append(QString::fromStdString(raw_translation.target(i)));
        }
        translation->setTarget(targets);

        QStringList examples;
        for (int i = 0; i < raw_translation.example_size(); i++) {
            examples.append(QString::fromStdString(raw_translation.example(i)));
        }
        translation->setExamples(examples);

        result.append(QSharedPointer<Translation>(translation));
    }
    return result;
}

RPCErrorStatus ProtobufConverter::errorStatusFromGrpc(const grpc::Status &rawStatus) {
    switch (rawStatus.error_code()) {
    case grpc::StatusCode::DEADLINE_EXCEEDED:
        return RPCErrorStatus::DEADLINE_EXCEEDED;
    case grpc::StatusCode::UNAUTHENTICATED:
        return RPCErrorStatus::NOT_AUTHENTICATED;
    default:
        return RPCErrorStatus::UNKNOWN;
    }
}
