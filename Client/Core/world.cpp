#include "world.h"
#include <QtDebug>

#define MAX_WORLDS 3

qint8 World::version = 0;

World::World() {}

World::~World() {}

World &World::Instance() {
    static World world[MAX_WORLDS];
    return world[World::version];
}

void World::addScrutiny(Scrutiny scrutiny) {
    changes_.append(QSharedPointer<Scrutiny>::create(scrutiny.toProtobuf()));
}

const QMap<QUuid, QSharedPointer<DBCard>> &World::getCards() {
    return cards_;
}

const ChangeList &World::getChanges() {
    return changes_;
}

void World::setChanges(ChangeList newChanges) {
    cards_.clear();
    changes_ = newChanges;
    applyChanges();
}

QSharedPointer<DBCard> World::createCard() {
    QSharedPointer<CreateCard> change = QSharedPointer<CreateCard>::create(QUuid::createUuid());
    this->changes_.append(change);
    change->apply(this);
    return cards_.value(change->getCardId());
}

void World::deleteCard(QUuid id) {
    QSharedPointer<DeleteCard> change = QSharedPointer<DeleteCard>::create(id);
    this->changes_.append(change);
    change->apply(this);
}

void World::setSource(QUuid cardId, QString newSource) {
    QSharedPointer<ChangeSource> change = QSharedPointer<ChangeSource>::create(cardId, newSource);
    changes_.append(change);
    change->apply(this);
}

void World::editElem(QUuid cardId, const enum Field &fieldName, const QString &newElem, qint32 pos) {
    QSharedPointer<EditElem> change = QSharedPointer<EditElem>::create();
    change->buildAndApply(cardId, fieldName, newElem, pos, this);
    changes_.append(change);
//    change->apply(this);
}

void World::insertElem(QUuid cardId, const enum Field &fieldName, const QString &insertingElem, qint32 pos) {
    QSharedPointer<InsertElem> change = QSharedPointer<InsertElem>::create();
    change->buildAndApply(cardId, fieldName, insertingElem, pos, this);
    changes_.append(change);
//    change->apply(this);
}

void World::deleteElem(QUuid cardId, const enum Field &fieldName, qint32 pos) {
    QSharedPointer<DeleteElem> change = QSharedPointer<DeleteElem>::create();
    change->buildAndApply(cardId, fieldName, pos, this);
    changes_.append(change);
//    change->apply(this);
}

void World::saveToDB() {
//TODO
}

void World::applyChanges() {
    cards_.clear();
    for (auto change : changes_) {
        change->apply(this);
    }
}

void World::insertCard(QUuid key, QSharedPointer<DBCard> card) {
    cards_.insert(key, card);
}

void World::removeCard(QUuid key) {
    cards_.remove(key);
}

void World::setField(QUuid cardId, const enum Field &fieldName, QStringList newField) {
    QStringList field;
    switch (fieldName) {
        case TARGET:field = cards_.value(cardId)->getTargets();
            break;
        case EXAMPLE:field = cards_.value(cardId)->getExamples();
            break;
    }
    qint32 srcSize = field.size();
    qint32 dstSize = newField.size();
    QVector<QVector<qint32> > numActions(srcSize + 1);
    QVector<QVector<Action> > actions(srcSize + 1);
    for (qint32 i = 0; i <= srcSize; i++) {
        numActions[i].resize(dstSize + 1);
        actions[i].resize(dstSize + 1);
        actions[i][0] = DELETE;
        numActions[i][0] = i;
    }
    for (qint32 i = 1; i <= dstSize; i++) {
        numActions[0][i] = i;
        actions[0][i] = INSERT;
    }
    for (qint32 i = 1; i <= srcSize; i++) {
        for (qint32 j = 1; j <= dstSize; j++) {
            qint32 cost = field[i - 1] == newField[j - 1] ? 0 : 1;
            if (numActions[i][j - 1] < numActions[i - 1][j] && numActions[i][j - 1] < numActions[i - 1][j - 1] + cost) {
                numActions[i][j] = numActions[i][j - 1] + 1;
                actions[i][j] = INSERT;
            } else if (numActions[i - 1][j] < numActions[i - 1][j - 1] + cost) {
                numActions[i][j] = numActions[i - 1][j] + 1;
                actions[i][j] = DELETE;
            } else {
                numActions[i][j] = numActions[i - 1][j - 1] + cost;
                actions[i][j] = cost == 0 ? NOTHING : EDIT;
            }
        }
    }
    QList<Action> seqActionsReversed;
    qint32 i = srcSize;
    qint32 j = dstSize;
    while (i > 0 || j > 0) {
        Action action = actions[i][j];
        seqActionsReversed.append(action);
        switch (action) {
            case DELETE:i--;
                break;
            case INSERT:j--;
                break;
            default:i--;
                j--;
        }
    }
    QList<Action>::const_reverse_iterator rit = seqActionsReversed.rbegin();
    qint32 curNew = 0;
    while (rit != seqActionsReversed.rend()) {
        Action action = *rit;
        switch (action) {
            case DELETE:this->deleteElem(cardId, fieldName, curNew);
                break;
            case INSERT:this->insertElem(cardId, fieldName, newField[curNew], curNew);
                curNew++;
                break;
            case EDIT:this->editElem(cardId, fieldName, newField[curNew], curNew);
                curNew++;
                break;
            case NOTHING:curNew++;
        }
        rit++;
    }
}
