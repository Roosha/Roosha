#include "changes.h"
#include "Helpers/protobuf_converter.h"

#include <QDebug>
#include <stdexcept>

inline roosha::CardChange::Field __toProtoField(Field field) {
    switch (field) {
        case EXAMPLE: return roosha::CardChange::Field::CardChange_Field_EXAMPLE;
        case TARGET: return roosha::CardChange::Field::CardChange_Field_TARGET;
    }
    throw std::logic_error(std::logic_error("Unexpected card field type: " + static_cast<int>(field)));
}

Field __fieldFromProto(roosha::CardChange::Field rawField) {
    switch (rawField) {
        case roosha::CardChange::Field::CardChange_Field_EXAMPLE: return Field::EXAMPLE;
        case roosha::CardChange::Field::CardChange_Field_TARGET: return Field::TARGET;

        case roosha::CardChange::Field::CardChange_Field_UNKNOWN:
        case roosha::CardChange::Field::CardChange_Field_CardChange_Field_INT_MAX_SENTINEL_DO_NOT_USE_:
        case roosha::CardChange::Field::CardChange_Field_CardChange_Field_INT_MIN_SENTINEL_DO_NOT_USE_:
            qFatal("__fieldFromProto: unexpected raw field.L %d",
                   rawField);
    }
    throw std::logic_error("Unexpected card field: " + static_cast<int>(rawField));
}

CardChange::CardChange(roosha::Change change) : IChange(change) {
    if (change.change_case() == roosha::Change::CHANGE_NOT_SET)  {
        rawChange.mutable_cardchange();
    } else if (change.change_case() != roosha::Change::kCardChange) {
        throw std::invalid_argument("wrong change type in CardChange::CardChange(roosha::Change change); got "
                                            + std::to_string(change.change_case()));
    }
}

QUuid CardChange::getCardId() const {
    return QUuid(QString::fromStdString(rawChange.cardchange().cardid()));
}

ChangeSource::ChangeSource(QUuid cardId, const QString &newSrc) : CardChange(roosha::Change()) {
    rawChange.mutable_cardchange()->set_cardid(cardId.toString().toStdString());
    rawChange.mutable_cardchange()->mutable_changesource()->set_newsource(newSrc.toStdString());
}

ChangeSource::ChangeSource(roosha::Change change) : CardChange(change) {
    if (change.cardchange().change_case() != roosha::CardChange::kChangeSource) {
        throw std::invalid_argument("wrong change type in ChangeSource::ChangeSource(roosha::Change change); got "
                                            + std::to_string(change.cardchange().change_case()));
    }
}

void ChangeSource::apply(World *world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(getCardId());
    cardptr->source_ = getNewSource();
}

QString ChangeSource::getNewSource() const {
    return QString::fromStdString(rawChange.cardchange().changesource().newsource());
}

EditElem::EditElem(QUuid cardId,
                   const enum Field &field,
                   const QString &newEl,
                   const QLKey &p) : CardChange(roosha::Change()) {
    rawChange.mutable_cardchange()->set_cardid((cardId.toString().toStdString()));
    rawChange.mutable_cardchange()->mutable_editelem()->set_field(__toProtoField(field));
    rawChange.mutable_cardchange()->mutable_editelem()->set_value(newEl.toStdString());
    rawChange.mutable_cardchange()->mutable_editelem()->set_position(p.toByteArray().constData());
}

EditElem::EditElem() : CardChange(roosha::Change()) {
    rawChange.mutable_cardchange()->mutable_editelem();
}

EditElem::EditElem(roosha::Change change) : CardChange(change) {
    if (change.cardchange().change_case() != roosha::CardChange::kEditElem) {
        throw std::invalid_argument("wrong change type in EditElem::EditElem(roosha::Change change); got "
                                            + std::to_string(change.cardchange().change_case()));
    }
}

void EditElem::buildAndApply(QUuid id,
                             const enum Field &field,
                             const QString &newEl,
                             const qint32 &p,
                             World *world) {
    rawChange.mutable_cardchange()->set_cardid((id.toString().toStdString()));
    rawChange.mutable_cardchange()->mutable_editelem()->set_field(__toProtoField(field));
    rawChange.mutable_cardchange()->mutable_editelem()->set_value(newEl.toStdString());

    const QSharedPointer<DBCard> cardptr = world->getCards().value(id);
    switch (field) {
        case TARGET:
            rawChange.mutable_cardchange()->mutable_editelem()->set_position(
                    cardptr->targets_.modify(p, newEl).toByteArray().constData());
            break;
        case EXAMPLE:
            rawChange.mutable_cardchange()->mutable_editelem()->set_position(
                    cardptr->examples_.modify(p, newEl).toByteArray().constData());
            break;
    }
}

void EditElem::apply(World *world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(getCardId());
    switch (getFieldName()) {
        case TARGET:
            cardptr->targets_.modify(getPos(), getNewElem());
            break;
        case EXAMPLE:
            cardptr->examples_.modify(getPos(), getNewElem());
            break;
    }
}

Field EditElem::getFieldName() const {
    return __fieldFromProto(rawChange.cardchange().editelem().field());
}

QString EditElem::getNewElem() const {
    return QString::fromStdString(rawChange.cardchange().editelem().value());
}

QLKey EditElem::getPos() const {
    return QLKey(QByteArray::fromStdString(rawChange.cardchange().editelem().position()));
}

InsertElem::InsertElem() : CardChange(roosha::Change()) {
    rawChange.mutable_cardchange()->mutable_insertelem();
}

InsertElem::InsertElem(roosha::Change change) : CardChange(change) {
    if (change.cardchange().change_case() != roosha::CardChange::kInsertElem) {
        throw std::invalid_argument("wrong change type in InsertElem::InsertElem(roosha::Change change); got "
                                            + std::to_string(change.cardchange().change_case()));
    }
}

InsertElem::InsertElem(QUuid cardId,
                       const enum Field &field,
                       const QString &insertingEl,
                       const QLKey &p) : CardChange(roosha::Change()) {
    rawChange.mutable_cardchange()->set_cardid((cardId.toString().toStdString()));
    rawChange.mutable_cardchange()->mutable_insertelem()->set_field(__toProtoField(field));
    rawChange.mutable_cardchange()->mutable_insertelem()->set_value(insertingEl.toStdString());
    rawChange.mutable_cardchange()->mutable_insertelem()->set_index(p.toByteArray().constData());
}

void InsertElem::buildAndApply(QUuid id,
                               const enum Field &field,
                               const QString &insertingEl,
                               const qint32 &p,
                               World *world) {
    rawChange.mutable_cardchange()->set_cardid((id.toString().toStdString()));
    rawChange.mutable_cardchange()->mutable_insertelem()->set_field(__toProtoField(field));
    rawChange.mutable_cardchange()->mutable_insertelem()->set_value(insertingEl.toStdString());

    const QSharedPointer<DBCard> cardptr = world->getCards().value(id);
    switch (field) {
        case TARGET:
            rawChange.mutable_cardchange()->mutable_insertelem()->set_index(
                    cardptr->targets_.insertAfter(p - 1, insertingEl).toByteArray().constData());
            break;
        case EXAMPLE:
            rawChange.mutable_cardchange()->mutable_insertelem()->set_index(
                    cardptr->examples_.insertAfter(p - 1, insertingEl).toByteArray().constData());
            break;
    }
}

void InsertElem::apply(World *world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(getCardId());
    switch (getFieldName()) {
        case TARGET: cardptr->targets_.insert(getPos(), getInsertingElem());
            break;
        case EXAMPLE: cardptr->examples_.insert(getPos(), getInsertingElem());
            break;
    }
}

Field InsertElem::getFieldName() const {
    return __fieldFromProto(rawChange.cardchange().insertelem().field());
}

QString InsertElem::getInsertingElem() const {
    return QString::fromStdString(rawChange.cardchange().insertelem().value());
}

QLKey InsertElem::getPos() const {
    return QLKey(QByteArray::fromStdString(rawChange.cardchange().insertelem().index()));;
}

DeleteElem::DeleteElem() : CardChange(roosha::Change()) {
    rawChange.mutable_cardchange()->mutable_deleteelem();
}
DeleteElem::DeleteElem(roosha::Change change) : CardChange(change) {
    if (change.cardchange().change_case() != roosha::CardChange::kDeleteElem) {
        throw std::invalid_argument("wrong change type in DeleteElem::DeleteElem(roosha::Change change) ; got "
                                            + std::to_string(change.cardchange().change_case()));
    }
}

DeleteElem::DeleteElem(QUuid cardId,
                       const enum Field &field,
                       const QLKey &p) : CardChange(roosha::Change()) {

    rawChange.mutable_cardchange()->set_cardid((cardId.toString().toStdString()));
    rawChange.mutable_cardchange()->mutable_deleteelem()->set_field(__toProtoField(field));
    rawChange.mutable_cardchange()->mutable_deleteelem()->set_index(p.toByteArray().constData());
}

void DeleteElem::buildAndApply(QUuid id,
                               const enum Field &field,
                               const qint32 &p,
                               World *world) {
    rawChange.mutable_cardchange()->set_cardid((id.toString().toStdString()));
    rawChange.mutable_cardchange()->mutable_deleteelem()->set_field(__toProtoField(field));

    const QSharedPointer<DBCard> cardptr = world->getCards().value(id);
    switch (field) {
        case TARGET:
            rawChange.mutable_cardchange()->mutable_deleteelem()->set_index(
                    cardptr->targets_.remove(p).toByteArray().constData());
            break;
        case EXAMPLE:
            rawChange.mutable_cardchange()->mutable_deleteelem()->set_index(
                    cardptr->examples_.remove(p).toByteArray().constData());
            break;
    }
}

void DeleteElem::apply(World *world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(getCardId());
    switch (getFieldName()) {
        case TARGET: cardptr->targets_.remove(getPos());
            break;
        case EXAMPLE: cardptr->examples_.remove(getPos());
            break;
    }
}

Field DeleteElem::getFieldName() const {
    return __fieldFromProto(rawChange.cardchange().deleteelem().field());
}

QLKey DeleteElem::getPos() const {
    return QLKey(QByteArray::fromStdString(rawChange.cardchange().deleteelem().index()));
}

CreateCard::CreateCard(QUuid id) : CardChange(roosha::Change()) {
    rawChange.mutable_cardchange()->set_cardid(id.toString().toStdString());
    rawChange.mutable_cardchange()->mutable_createcard();
}

CreateCard::CreateCard(roosha::Change change) : CardChange(change) {

    if (change.cardchange().change_case() != roosha::CardChange::kCreateCard) {
        throw std::invalid_argument("wrong change type in CreateCard::CreateCard(roosha::Change change) ; got "
                                            + std::to_string(change.cardchange().change_case()));
    }
}

void CreateCard::apply(World *world) {
    world->insertCard(getCardId(), QSharedPointer<DBCard>::create(getCardId()));
}

DeleteCard::DeleteCard(QUuid id) : CardChange(roosha::Change()) {
    rawChange.mutable_cardchange()->set_cardid(id.toString().toStdString());
    rawChange.mutable_cardchange()->mutable_deletecard();
}

DeleteCard::DeleteCard(roosha::Change change) : CardChange(change) {
    if (change.cardchange().change_case() != roosha::CardChange::kDeleteCard) {
        throw std::invalid_argument("wrong change type in DeleteCard::DeleteCard(roosha::Change change) ; got "
                                            + std::to_string(change.cardchange().change_case()));
    }
}

void DeleteCard::apply(World *world) {
    world->removeCard(getCardId());
}