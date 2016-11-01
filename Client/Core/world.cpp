#include "world.h"
#include "card.h"
#include "changes.h"
#include <QUuid>

World::World() { }

World::~World() { }

World& World::Instance() {
    static World world;
    return world;
}

QMap<QUuid, QSharedPointer<DBCard>> & World::getCards() {
    return cards;
}

QVector<QSharedPointer<IChange>> & World::getChanges() {
    return changes;
}

QUuid World::createCard() {
    QSharedPointer<CreateCard> change = QSharedPointer<CreateCard>::create();
    this->changes.append(change);
    change->apply(this);
    return change->cardId;
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

void World::setExamples(QUuid cardId, QStringList newExamples) {
//TODO: create elemenary changes instead calling simplified card methods:
//      compare newExample and current example and generate changes
}

void World::setTarget(QUuid cardId, QStringList newTarget) {
//TODO: create elemenary changes instead calling simplified card methods:
//      compare newTarget and current target and generate changes
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

