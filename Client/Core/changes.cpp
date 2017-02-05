#include "changes.h"
#include "Helpers/protobuf_converter.h"

#include <QDebug>

using ProtobufConverter::changeToProtobuf;

ChangeSource::ChangeSource(QUuid cardId, const QString &newSrc) : cardId(cardId), newSource(newSrc) {}

void ChangeSource::apply(World *world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    cardptr->source_ = newSource;
}

roosha::Change ChangeSource::toProtobuf() const {
    return changeToProtobuf(*this);
}

QUuid ChangeSource::getCardId() const {
    return cardId;
}

QString ChangeSource::getNewSource() const {
    return newSource;
}

EditElem::EditElem() {}

EditElem::EditElem(QUuid cardId, const enum Field &field, const QString &newEl, const QLKey &p) :
        cardId(cardId), fieldName(field), newElem(newEl), pos(p) {}

void EditElem::buildAndApply(QUuid id, const enum Field &field, const QString &newEl, const qint32 &p, World *world) {
    cardId = id;
    fieldName = field;
    newElem = newEl;

    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET: pos = cardptr->targets_.modify(p, newElem);
            break;
        case EXAMPLE: pos = cardptr->examples_.modify(p, newElem);
            break;
    }
}

void EditElem::apply(World *world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET:cardptr->targets_.modify(pos, newElem);
            break;
        case EXAMPLE:cardptr->examples_.modify(pos, newElem);
            break;
    }
}

roosha::Change EditElem::toProtobuf() const {
    return changeToProtobuf(*this);
}

QUuid EditElem::getCardId() const {
    return cardId;
}

Field EditElem::getFieldName() const {
    return fieldName;
}

QString EditElem::getNewElem() const {
    return newElem;
}

QLKey EditElem::getPos() const {
    return pos;
}

InsertElem::InsertElem() {}

InsertElem::InsertElem(QUuid cardId, const enum Field &field, const QString &insertingEl, const QLKey &p) :
        cardId(cardId), fieldName(field), insertingElem(insertingEl), pos(p) {}

void InsertElem::buildAndApply(QUuid id,
                               const enum Field &field,
                               const QString &insertingEl,
                               const qint32 &p,
                               World *world) {
    cardId = id;
    fieldName = field;
    insertingElem = insertingEl;

    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET: pos = cardptr->targets_.insertAfter(p-1, insertingElem);
            break;
        case EXAMPLE: pos = cardptr->examples_.insertAfter(p-1, insertingElem);
            break;
    }
}

void InsertElem::apply(World *world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET:cardptr->targets_.insert(pos, insertingElem);
            break;
        case EXAMPLE:cardptr->examples_.insert(pos, insertingElem);
            break;
    }
}

roosha::Change InsertElem::toProtobuf() const {
    return changeToProtobuf(*this);
}

QUuid InsertElem::getCardId() const {
    return cardId;
}

Field InsertElem::getFieldName() const {
    return fieldName;
}

QString InsertElem::getInsertingElem() const {
    return insertingElem;
}

QLKey InsertElem::getPos() const {
    return pos;
}

DeleteElem::DeleteElem() {}

DeleteElem::DeleteElem(QUuid cardId, const enum Field &field, const QLKey &p) :
        cardId(cardId), fieldName(field), pos(p) {}

void DeleteElem::buildAndApply(QUuid id, const enum Field &field, const qint32 &p, World *world) {
    cardId = id;
    fieldName = field;

    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET: pos = cardptr->targets_.remove(p);
            break;
        case EXAMPLE: pos = cardptr->examples_.remove(p);
            break;
    }
}

void DeleteElem::apply(World *world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET: cardptr->targets_.remove(pos);
            break;
        case EXAMPLE: cardptr->examples_.remove(pos);
            break;
    }
}

roosha::Change DeleteElem::toProtobuf() const {
    return changeToProtobuf(*this);
}

QUuid DeleteElem::getCardId() const {
    return cardId;
}

Field DeleteElem::getFieldName() const {
    return fieldName;
}

QLKey DeleteElem::getPos() const {
    return pos;
}

CreateCard::CreateCard(QUuid id) : cardId(id) {}

QUuid CreateCard::getCardId() const {
    return cardId;
}

void CreateCard::apply(World *world) {
    world->insertCard(cardId, QSharedPointer<DBCard>::create(cardId));
}

roosha::Change CreateCard::toProtobuf() const {
    return changeToProtobuf(*this);
}

DeleteCard::DeleteCard(QUuid id) : cardId(id) {}

void DeleteCard::apply(World *world) {
    world->removeCard(cardId);
}

roosha::Change DeleteCard::toProtobuf() const {
    return changeToProtobuf(*this);
}

QUuid DeleteCard::getCardId() const {
    return cardId;
}
