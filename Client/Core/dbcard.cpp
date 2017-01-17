#include "Core/dbcard.h"
#include <QDebug>

DBCard::DBCard(QUuid newId) : world_(World::Instance()) {
    setId(newId);
}

void DBCard::setId(QUuid newId) {
    id_ = newId;
}

void DBCard::setSource(QString newSource) {
    world_.setSource(id_, newSource);
}

void DBCard::setField(const enum Field &fieldName, QStringList newField) {
    world_.setField(id_, fieldName, newField);
}

void DBCard::editElem(const enum Field &fieldName, const QString &newElem, qint32 pos) {
    world_.editElem(id_, fieldName, newElem, pos);
}

void DBCard::insertElem(const enum Field &fieldName, const QString &insertingElem, qint32 pos) {
    world_.insertElem(id_, fieldName, insertingElem, pos);
}

void DBCard::deleteElem(const enum Field &fieldName, qint32 pos) {
    world_.deleteElem(id_, fieldName, pos);
}
