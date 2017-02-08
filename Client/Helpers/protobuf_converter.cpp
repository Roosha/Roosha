#include "protobuf_converter.h"

#include <QtDebug>

using ProtobufConverter::grpcStatusCodeToCString;

roosha::Translations ProtobufConverter::translationsToProtobuf(const Translations &translations) {
    roosha::Translations result;
    result.set_source(translations[0]->getSource().toStdString());
    for (const auto &translation: translations) {
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

Translations ProtobufConverter::translationsFromProtobuf(const roosha::Translations &rawTranslations) {
    Translations result;
    for (int i = 0; i < rawTranslations.translation_size(); i++) {
        auto raw_translation = rawTranslations.translation(i);
        Translation *translation = new Translation;
        translation->setSource(QString::fromStdString(rawTranslations.source()));

        QStringList targets;
        for (int tar_count = 0; tar_count < raw_translation.target_size(); tar_count++) {
            targets.append(QString::fromStdString(raw_translation.target(tar_count)));
        }
        translation->setTarget(targets);

        QStringList examples;
        for (int ex_count = 0; ex_count < raw_translation.example_size(); ex_count++) {
            examples.append(QString::fromStdString(raw_translation.example(ex_count)));
        }
        translation->setExamples(examples);

        result.append(QSharedPointer<Translation>(translation));
    }
    return result;
}

RPCErrorStatus ProtobufConverter::errorStatusFromGrpc(const grpc::Status &rawStatus) {
    switch (rawStatus.error_code()) {
        case grpc::StatusCode::DEADLINE_EXCEEDED: return RPCErrorStatus::DEADLINE_EXCEEDED;
        case grpc::StatusCode::UNAUTHENTICATED: return RPCErrorStatus::NOT_AUTHENTICATED;
        case grpc::StatusCode::UNAVAILABLE: return RPCErrorStatus::NO_CONNECTION;
        default:
            qWarning("Unknown grpc statusCode: %s. Message: '%s'",
                                       grpcStatusCodeToCString(rawStatus.error_code()),
                                       rawStatus.error_message().c_str());
            return RPCErrorStatus::UNKNOWN;
    }
}

const char *ProtobufConverter::grpcStatusCodeToCString(const grpc::StatusCode &rawStatus) {
    switch (rawStatus) {
        case grpc::OK: return "OK";
        case grpc::CANCELLED: return "CANCELLED";
        case grpc::UNKNOWN: return "UNKNOWN";
        case grpc::INVALID_ARGUMENT: return "INVALID_ARGUMENT";
        case grpc::DEADLINE_EXCEEDED: return "DEADLINE_EXCEEDED";
        case grpc::NOT_FOUND: return "NOT_FOUND";
        case grpc::ALREADY_EXISTS: return "ALREADY_EXISTS";
        case grpc::PERMISSION_DENIED: return "PERMISSION_DENIED";
        case grpc::UNAUTHENTICATED: return "UNAUTHENTICATED";
        case grpc::RESOURCE_EXHAUSTED: return "RESOURCE_EXHAUSTED";
        case grpc::FAILED_PRECONDITION: return "FAILED_PRECONDITION";
        case grpc::ABORTED: return "ABORTED";
        case grpc::OUT_OF_RANGE: return "OUT_OF_RANGE";
        case grpc::UNIMPLEMENTED: return "UNIMPLEMENTED";
        case grpc::INTERNAL: return "INTERNAL";
        case grpc::UNAVAILABLE: return "UNAVAILABLE";
        case grpc::DATA_LOSS: return "DATA_LOSS";
        case grpc::DO_NOT_USE: return "DO_NOT_USE";
    }
    throw std::logic_error(QString::asprintf("Unhandled grpc::StatusCode value: %d", rawStatus).toStdString());
}

ChangePtr ProtobufConverter::changeFromProtobuf(const roosha::Change &rawChange) {
    switch (rawChange.change_case()) {
        case roosha::Change::kCardChange:
            switch (rawChange.cardchange().change_case()) {
                case roosha::CardChange::kCreateCard: return QSharedPointer<CreateCard>::create(rawChange);
                case roosha::CardChange::kDeleteCard: return QSharedPointer<DeleteCard>::create(rawChange);
                case roosha::CardChange::kChangeSource: return QSharedPointer<ChangeSource>::create(rawChange);
                case roosha::CardChange::kInsertElem: return QSharedPointer<InsertElem>::create(rawChange);
                case roosha::CardChange::kDeleteElem: return QSharedPointer<DeleteElem>::create(rawChange);
                case roosha::CardChange::kEditElem: return QSharedPointer<EditElem>::create(rawChange);
                case roosha::CardChange::CHANGE_NOT_SET:
                    qWarning("ChangeFromProto: empty card change passed. Return nullptr");
                    return ChangePtr(nullptr);

            }
        case roosha::Change::CHANGE_NOT_SET:
            qWarning("ProtobufConverter::changeFromProtobuf: empty Change passed. Return nullptr");
            return ChangePtr(nullptr);

    }
}

