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

#ifndef DECLARE_CARD_CHANGE_RESULT
#define DECLARE_CARD_CHANGE_RESULT \
    roosha::Change result; \
    result.mutable_cardchange()->set_cardid(change.getCardId().toString().toStdString());
#else
#error DECLARE_CARD_CHANGE_RESULT macro already defined
#endif

inline roosha::CardChange::Field __toProtoField(Field field) {
    switch (field) {
        case EXAMPLE: return roosha::CardChange::Field::CardChange_Field_EXAMPLE;
        case TARGET: return roosha::CardChange::Field::CardChange_Field_TARGET;
    }
    throw std::logic_error(std::logic_error("Unexpected card field type: " + static_cast<int>(field)));
}

roosha::Change ProtobufConverter::changeToProtobuf(const DeleteCard &change) {
    DECLARE_CARD_CHANGE_RESULT;
    result.mutable_cardchange()->mutable_deletecard();
    return result;
}

roosha::Change ProtobufConverter::changeToProtobuf(const ChangeSource &change) {
    DECLARE_CARD_CHANGE_RESULT;
    result.mutable_cardchange()->mutable_changesource()->set_newsource(change.getNewSource().toStdString());
    return result;
}

roosha::Change ProtobufConverter::changeToProtobuf(const CreateCard &change) {
    DECLARE_CARD_CHANGE_RESULT;
    result.mutable_cardchange()->mutable_createcard();
    return result;
}

roosha::Change ProtobufConverter::changeToProtobuf(const EditElem &change) {
    DECLARE_CARD_CHANGE_RESULT;
    auto editElem = result.mutable_cardchange()->mutable_editelem();
    editElem->set_field(__toProtoField(change.getFieldName()));
    editElem->set_position(change.getPos().toByteArray().constData());
    editElem->set_value(change.getNewElem().toStdString());
    return result;
}

roosha::Change ProtobufConverter::changeToProtobuf(const InsertElem &change) {
    DECLARE_CARD_CHANGE_RESULT;
    auto insertElem = result.mutable_cardchange()->mutable_insertelem();
    insertElem->set_field(__toProtoField(change.getFieldName()));
    insertElem->set_index(change.getPos().toByteArray().constData());
    insertElem->set_value(change.getInsertingElem().toStdString());
    return result;
}

roosha::Change ProtobufConverter::changeToProtobuf(const DeleteElem &change) {
    DECLARE_CARD_CHANGE_RESULT;
    auto deleteElem = result.mutable_cardchange()->mutable_deleteelem();
    deleteElem->set_field(__toProtoField(change.getFieldName()));
    deleteElem->set_index(change.getPos().toByteArray().constData());
    return result;
}

#ifdef DECLARE_CARD_CHANGE_RESULT
#undef DECLARE_CARD_CHANGE_RESULT
#else
#error DECLARE_CARD_CHANGE_RESULT macro is not defined
#endif

Field __fielfFromProto(roosha::CardChange::Field rawField) {
    switch (rawField) {
        case roosha::CardChange::Field::CardChange_Field_EXAMPLE: return Field::EXAMPLE;
        case roosha::CardChange::Field::CardChange_Field_TARGET: return Field::TARGET;

        case roosha::CardChange::Field::CardChange_Field_UNKNOWN:
        case roosha::CardChange::Field::CardChange_Field_CardChange_Field_INT_MAX_SENTINEL_DO_NOT_USE_:
        case roosha::CardChange::Field::CardChange_Field_CardChange_Field_INT_MIN_SENTINEL_DO_NOT_USE_:
            qFatal("__fielfFromProto: unexpected raw field.L %d",
                   rawField);
    }
    throw std::logic_error("Unexpected card field: " + static_cast<int>(rawField));
}

ChangePtr __cardChangeFromProto(const roosha::CardChange &rawChange) {
    QUuid cardId = QUuid(QString::fromStdString(rawChange.cardid()));

    switch (rawChange.change_case()) {
        case roosha::CardChange::kCreateCard: return QSharedPointer<CreateCard>::create(cardId);
        case roosha::CardChange::kDeleteCard: return QSharedPointer<DeleteCard>::create(cardId);
        case roosha::CardChange::kChangeSource:
            return QSharedPointer<ChangeSource>::create(
                    cardId,
                    QString::fromStdString(rawChange.changesource().newsource())
            );
        case roosha::CardChange::kInsertElem:
            return QSharedPointer<InsertElem>::create(
                    cardId,
                    __fielfFromProto(rawChange.insertelem().field()),
                    QString::fromStdString(rawChange.insertelem().value()),
                    QLKey(QByteArray::fromStdString(rawChange.insertelem().index()))
            );
        case roosha::CardChange::kDeleteElem:
            return QSharedPointer<DeleteElem>::create(
                    cardId,
                    __fielfFromProto(rawChange.deleteelem().field()),
                    QLKey(QByteArray::fromStdString(rawChange.deleteelem().index()))
            );
        case roosha::CardChange::kEditElem:
            return QSharedPointer<EditElem>::create(
                    cardId,
                    __fielfFromProto(rawChange.editelem().field()),
                    QString::fromStdString(rawChange.editelem().value()),
                    QLKey(QByteArray::fromStdString(rawChange.editelem().position()))
            );
        case roosha::CardChange::CHANGE_NOT_SET:
            qWarning("__cardChangeFromProto: empty card change passed. Return nullptr");
            return ChangePtr(nullptr);

    }
    throw std::logic_error("Unexpected change case: " + rawChange.change_case());
}

ChangePtr ProtobufConverter::changeFromProtobuf(const roosha::Change &rawChange) {
    switch (rawChange.change_case()) {
        case roosha::Change::kCardChange: return __cardChangeFromProto(rawChange.cardchange());
        case roosha::Change::CHANGE_NOT_SET:
            qWarning("ProtobufConverter::changeFromProtobuf: empty Change passed. Return nullptr");
            return ChangePtr(nullptr);

    }
    throw std::logic_error("Unexpected change case: " + rawChange.change_case());
}

