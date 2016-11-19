#include "protobuf_converter.h"

#include <QtDebug>
#include <QUuid>
#include <QMessageLogger>

#include <iostream>


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

#ifndef DECLARE_CARD_CHANGE_RESULT
#define DECLARE_CARD_CHANGE_RESULT \
    roosha::Change result; \
    result.mutable_cardchange()->set_cardid(change.getCardId().toString().toStdString());
#else
#error DECLARE_CARD_CHANGE_RESULT macro already defined
#endif


inline roosha::CardChange::Field __toProtoField(Field field) {
    switch (field) {
    case EXAMPLE:
        return roosha::CardChange::Field::CardChange_Field_EXAMPLE;
    case TARGET:
        return roosha::CardChange::Field::CardChange_Field_TARGET;

    }
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
    editElem->set_position(change.getPos());
    editElem->set_value(change.getNewElem().toStdString());
    return result;
}

roosha::Change ProtobufConverter::changeToProtobuf(const InsertElem &change) {
    DECLARE_CARD_CHANGE_RESULT;
    auto insertElem = result.mutable_cardchange()->mutable_insertelem();
    insertElem->set_field(__toProtoField(change.getFieldName()));
    insertElem->set_index(change.getPos());
    insertElem->set_value(change.getInsertingElem().toStdString());
    return result;
}


roosha::Change ProtobufConverter::changeToProtobuf(const DeleteElem &change) {
    DECLARE_CARD_CHANGE_RESULT;
    auto deleteElem = result.mutable_cardchange()->mutable_deleteelem();
    deleteElem->set_field(__toProtoField(change.getFieldName()));
    deleteElem->set_index(change.getPos());
    return result;
}


#ifdef DECLARE_CARD_CHANGE_RESULT
#undef DECLARE_CARD_CHANGE_RESULT
#else
#error DECLARE_CARD_CHANGE_RESULT macro is not defined
#endif


Field __fielfFromProto(roosha::CardChange::Field rawField) {
    switch (rawField) {
    case roosha::CardChange::Field::CardChange_Field_EXAMPLE:
        return Field::EXAMPLE;
    case roosha::CardChange::Field::CardChange_Field_TARGET:
        return Field::TARGET;
    case roosha::CardChange::Field::CardChange_Field_UNKNOWN:
    case roosha::CardChange::Field::CardChange_Field_CardChange_Field_INT_MAX_SENTINEL_DO_NOT_USE_:
    case roosha::CardChange::Field::CardChange_Field_CardChange_Field_INT_MIN_SENTINEL_DO_NOT_USE_:
        qFatal("__fielfFromProto: unexpected raw field.L %d", rawField);
    }
}

ChangePtr __cardChangeFromProto(const roosha::CardChange& rawChange) {
    QUuid cardId = QUuid(QString::fromStdString(rawChange.cardid()));

    switch(rawChange.change_case()) {
    case roosha::CardChange::kCreateCard:
        return QSharedPointer<CreateCard>::create(cardId);
    case roosha::CardChange::kDeleteCard:
        return QSharedPointer<DeleteCard>::create(cardId);
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
                   rawChange.insertelem().index()
               );
    case roosha::CardChange::kDeleteElem:
        return QSharedPointer<DeleteElem>::create(
                   cardId,
                   __fielfFromProto(rawChange.deleteelem().field()),
                   rawChange.deleteelem().index()
               );
    case roosha::CardChange::kEditElem:
        return QSharedPointer<EditElem>::create(
                   cardId,
                   __fielfFromProto(rawChange.editelem().field()),
                   QString::fromStdString(rawChange.editelem().value()),
                   rawChange.editelem().position()
               );
    case roosha::CardChange::CHANGE_NOT_SET:
        qWarning("__cardChangeFromProto: empty card change passed. Return nullptr");
        return ChangePtr(nullptr);

    }
}

ChangePtr ProtobufConverter::changeFromProtobuf(const roosha::Change &rawChange) {
    switch (rawChange.change_case()) {
    case roosha::Change::kCardChange:
        return __cardChangeFromProto(rawChange.cardchange());
    case roosha::Change::CHANGE_NOT_SET:
        qWarning("ProtobufConverter::changeFromProtobuf: empty Change passed. Return nullptr");
        return ChangePtr(nullptr);

    }
}
