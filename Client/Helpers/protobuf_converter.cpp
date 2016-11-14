#include "protobuf_converter.h"

#include <QtDebug>
#include <QUuid>


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

roosha::Change createChange(roosha::Change::ChangeType type, QUuid cardId) {
    roosha::Change result;
    result.set_cardid(cardId.toString().toStdString());
    result.set_type(type);
    return result;
}

roosha::Change::StringListChange::Field __fieldToProtobuf(Field field) {
    switch (field) {
    case EXAMPLE:
        return roosha::Change_StringListChange_Field_EXAMPLE;
    case TARGET:
        return roosha::Change_StringListChange_Field_TARGET;

    }
}

roosha::Change ProtobufConverter::changeToProtobuf(const ChangeSource &change) {
    roosha::Change result = createChange(roosha::Change_ChangeType_CHANGE_SOURCE, change.getCardId());
    result.set_newsource(change.getNewSource().toStdString());

    return result;
}

roosha::Change ProtobufConverter::changeToProtobuf(const EditElem &change) {
    roosha::Change result = createChange(roosha::Change_ChangeType_EDIT_ELEM, change.getCardId());

    roosha::Change::StringListChange *stringListChange = result.mutable_stringlistchange();
    stringListChange->set_fieldname(__fieldToProtobuf(change.getFieldName()));
    stringListChange->set_index(change.getPos());
    stringListChange->set_value(change.getNewElem().toStdString());

    return result;
}

roosha::Change ProtobufConverter::changeToProtobuf(const InsertElem &change) {
    roosha::Change result = createChange(roosha::Change_ChangeType_INSERT_ELEM, change.getCardId());

    roosha::Change::StringListChange *stringListChange = result.mutable_stringlistchange();
    stringListChange->set_fieldname(__fieldToProtobuf(change.getFieldName()));
    stringListChange->set_index(change.getPos());
    stringListChange->set_value(change.getInsertingElem().toStdString());

    return result;
}

roosha::Change ProtobufConverter::changeToProtobuf(const DeleteElem &change) {
    roosha::Change result = createChange(roosha::Change_ChangeType_DELETE_ELEM, change.getCardId());

    roosha::Change::StringListChange *stringListChange = result.mutable_stringlistchange();
    stringListChange->set_fieldname(__fieldToProtobuf(change.getFieldName()));
    stringListChange->set_index(change.getPos());

    return result;
}

roosha::Change ProtobufConverter::changeToProtobuf(const CreateCard &change) {
    return createChange(roosha::Change_ChangeType_CREATE_CARD, change.getCardId());
}

roosha::Change ProtobufConverter::changeToProtobuf(const DeleteCard &change) {
    return createChange(roosha::Change_ChangeType_DELETE_CARD, change.getCardId());
}

Field __fieldFromProtobuf(roosha::Change_StringListChange_Field rawChange) {
    switch (rawChange) {
    case roosha::Change_StringListChange_Field_UNKNOWN:
        qWarning() << "unknown field type";
        break;
    case roosha::Change_StringListChange_Field_TARGET:
        return Field::TARGET;
    case roosha::Change_StringListChange_Field_EXAMPLE:
        return Field::EXAMPLE;

    case roosha::Change_StringListChange_Field_Change_StringListChange_Field_INT_MIN_SENTINEL_DO_NOT_USE_:
    case roosha::Change_StringListChange_Field_Change_StringListChange_Field_INT_MAX_SENTINEL_DO_NOT_USE_:
        qWarning() << "unexpected protobuf internal field type: " << rawChange;
        break;
    }
}

QSharedPointer<IChange> ProtobufConverter::changeFromProtobuf(const roosha::Change &rawChange) {
    QUuid cardId(QString::fromStdString(rawChange.cardid()));

    // I used pointer because I don't how standart accessor behaves when field isn't initialized.
    const roosha::Change::StringListChange* strListChange =
        const_cast<roosha::Change&>(rawChange).mutable_stringlistchange();

    switch (rawChange.type()) {
    case roosha::Change_ChangeType_UNKNOWN:
        qWarning() << "unknown change type";
        break;
    case roosha::Change_ChangeType_CHANGE_SOURCE:
        return QSharedPointer<ChangeSource>::create(cardId, QString::fromStdString(rawChange.newsource()));
    case roosha::Change_ChangeType_DELETE_CARD:
        return QSharedPointer<DeleteCard>::create(cardId);
    case roosha::Change_ChangeType_CREATE_CARD:
        return QSharedPointer<CreateCard>::create(cardId);
    case roosha::Change_ChangeType_INSERT_ELEM:
        return QSharedPointer<InsertElem>::create(
                   cardId,
                   __fieldFromProtobuf(strListChange->fieldname()),
                   QString::fromStdString(strListChange->value()),
                   strListChange->index()
               );
    case roosha::Change_ChangeType_DELETE_ELEM:
        return QSharedPointer<DeleteElem>::create(
                   cardId,
                   __fieldFromProtobuf(strListChange->fieldname()),
                   strListChange->index()
               );
    case roosha::Change_ChangeType_EDIT_ELEM:
        return QSharedPointer<EditElem>::create(
                   cardId,
                   __fieldFromProtobuf(strListChange->fieldname()),
                   QString::fromStdString(strListChange->value()),
                   strListChange->index()
               );
    case roosha::Change_ChangeType_Change_ChangeType_INT_MIN_SENTINEL_DO_NOT_USE_:
    case roosha::Change_ChangeType_Change_ChangeType_INT_MAX_SENTINEL_DO_NOT_USE_:
        qWarning() << "unexpected protobuf internal change type: " << rawChange.type();
        break;

    }
}
