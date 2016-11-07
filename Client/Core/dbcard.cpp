#include "Core/dbcard.h"
#include <QDebug>

DBCard::DBCard(QUuid newId) : world(World::Instance()),  id(newId) { }

QUuid DBCard::getId() {
    return id;
}

void DBCard::setSource(QString newSource) {
    world.setSource(id, newSource);
}

void DBCard::setExamples(QStringList newExamples) {
//TODO: create elemenary changes instead calling simplified world methods:
//      compare newExample and current example and generate changes
}

void DBCard::setTargets(QStringList newTarget) {
//TODO: create elemenary changes instead calling simplified world methods:
//      compare newTarget and current target and generate changes
}


//temp
void DBCard::editElem(const enum Field & fieldName, const QString & newElem, qint32 pos) {
    world.editElem(id, fieldName, newElem, pos);
}

void DBCard::insertElem(const enum Field & fieldName, const QString & insertingElem, qint32 pos) {
    world.insertElem(id, fieldName, insertingElem, pos);
}

void DBCard::deleteElem(const enum Field & fieldName, qint32 pos) {
    world.deleteElem(id, fieldName, pos);
}
