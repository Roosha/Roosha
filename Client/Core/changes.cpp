#include "changes.h"
#include <QDebug>
#include "world.h"
#include <QSharedPointer>


ChangeSource::ChangeSource(QUuid cardId, const QString & newSrc) : cardId(cardId), newSource(newSrc) {}

void ChangeSource::apply(World * world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    cardptr->source = newSource;
}

EditElem::EditElem(QUuid cardId, const enum Field & field, const QString & newEl, const qint32 & p) :
                    cardId(cardId), fieldName(field), newElem(newEl), pos(p) {}

void EditElem::apply(World * world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET:
            cardptr->targets[pos] = newElem;
            break;
        case EXAMPLE:
            cardptr->examples[pos] = newElem;
            break;
    }
}

InsertElem::InsertElem(QUuid cardId, const enum Field & field, const QString & insertingEl, const qint32 & p) :
                        cardId(cardId), fieldName(field), insertingElem(insertingEl), pos(p) {}

void InsertElem::apply(World * world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET:
            cardptr->targets.insert(pos, insertingElem);
            break;
        case EXAMPLE:
            cardptr->examples.insert(pos, insertingElem);
            break;
    }
}


DeleteElem::DeleteElem(QUuid cardId, const enum Field & field, const qint32 & p) :
                        cardId(cardId), fieldName(field), pos(p) {}

void DeleteElem::apply(World * world) {
    const QSharedPointer<DBCard> cardptr = world->getCards().value(this->cardId);
    switch (fieldName) {
        case TARGET:
            cardptr->targets.erase(cardptr->targets.begin() + pos);
            break;
        case EXAMPLE:
            cardptr->examples.erase(cardptr->examples.begin() + pos);
            break;
    }
}

CreateCard::CreateCard(QUuid id) : cardId(id) { }
QUuid CreateCard::getId() {
    return cardId;
}

void CreateCard::apply(World * world) {
//    if (cardId.isNull())
//        cardId = QUuid::createUuid();
    world->getCards().insert(cardId, QSharedPointer<DBCard>::create(cardId));
}

DeleteCard::DeleteCard(QUuid id) : cardId(id) {}

void DeleteCard::apply(World * world) {
    world->getCards().remove(cardId);
}
