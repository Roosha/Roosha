#include "world.h"
#include "card.h"
#include "dbcard.h"
#include "changes.h"
#include <QUuid>

World::World() { }

World::~World() { }

World& World::Instance() {
    static World world;
    return world;
}

const QMap<QUuid, QSharedPointer<DBCard>> & World::getCards() {
    return cards;
}

const QVector<QSharedPointer<IChange>> & World::getChanges() {
    return changes;
}

QSharedPointer<DBCard> World::createCard() {
    QSharedPointer<CreateCard> change = QSharedPointer<CreateCard>::create(QUuid::createUuid());
    this->changes.append(change);
    change->apply(this);
    return cards.value(change->getId());
}

void World::deleteCard(QUuid id) {
    QSharedPointer<DeleteCard> change = QSharedPointer<DeleteCard>::create(id);
    this->changes.append(change);
    change->apply(this);
}

void World::setSource(QUuid cardId, QString newSource) {
    QSharedPointer<ChangeSource> change = QSharedPointer<ChangeSource>::create(cardId, newSource);
    changes.append(change);
    change->apply(this);
}

void World::editElem(QUuid cardId, const enum Field & fieldName, const QString & newElem, qint32 pos) {
    QSharedPointer<EditElem> change = QSharedPointer<EditElem>::create(cardId, fieldName, newElem, pos);
    changes.append(change);
    change->apply(this);
}

void World::insertElem(QUuid cardId, const enum Field & fieldName, const QString & insertingElem, qint32 pos) {
    QSharedPointer<InsertElem> change = QSharedPointer<InsertElem>::create(cardId, fieldName, insertingElem, pos);
    changes.append(change);
    change->apply(this);
}

void World::deleteElem(QUuid cardId, const enum Field & fieldName, qint32 pos) {
    QSharedPointer<DeleteElem> change = QSharedPointer<DeleteElem>::create(cardId, fieldName, pos);
    changes.append(change);
    change->apply(this);
}

void World::saveToDB() {
//TODO
}

void World::applyChanges() {
    cards.clear();
    for (auto change : changes) {
        change->apply(this);
    }
}


void World::insertCard(QUuid key, QSharedPointer<DBCard> card) {
    cards.insert(key, card);
}

void World::removeCard(QUuid key){
    cards.remove(key);
}
