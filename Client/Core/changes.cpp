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

EditElem::EditElem(QUuid cardId, const enum Field &field, const QString &newEl, const qint32 &p) :
        cardId(cardId), fieldName(field), newElem(newEl), pos(p) {}

void EditElem::apply(World *world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET:cardptr->targets_[pos] = newElem;
            break;
        case EXAMPLE:cardptr->examples_[pos] = newElem;
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

qint32 EditElem::getPos() const {
    return pos;
}

InsertElem::InsertElem(QUuid cardId, const enum Field &field, const QString &insertingEl, const qint32 &p) :
        cardId(cardId), fieldName(field), insertingElem(insertingEl), pos(p) {}

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

qint32 InsertElem::getPos() const {
    return pos;
}

DeleteElem::DeleteElem(QUuid cardId, const enum Field &field, const qint32 &p) :
        cardId(cardId), fieldName(field), pos(p) {}

void DeleteElem::apply(World *world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET:cardptr->targets_.erase(cardptr->targets_.begin() + pos);
            break;
        case EXAMPLE:cardptr->examples_.erase(cardptr->examples_.begin() + pos);
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

qint32 DeleteElem::getPos() const {
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
