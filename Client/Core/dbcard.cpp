#include "Core/dbcard.h"
#include <QDebug>

DBCard::DBCard() {

}

void DBCard::setSource(QString newSource) {
    QSharedPointer<ChangeSource> change = QSharedPointer<ChangeSource>::create(newSource);
    changes.append(change);
    change->apply(this);
}

void DBCard::setExamples(QVector<QString> newExamples) {
//TODO: create elemenary changes instead calling simplified card methods:
//      compare newExample and current example and generate changes
}

void DBCard::setTarget(QVector<QString> newTarget) {
//TODO: create elemenary changes instead calling simplified card methods:
//      compare newTarget and current target and generate changes
}

void DBCard::editElem(const enum Field & fieldName, const QString & newElem, qint32 pos) {
    QSharedPointer<EditElem> change = QSharedPointer<EditElem>::create(fieldName, newElem, pos);
    changes.append(change);
    change->apply(this);
}

void DBCard::insertElem(const enum Field & fieldName, const QString & insertingElem, qint32 pos) {
    QSharedPointer<InsertElem> change = QSharedPointer<InsertElem>::create(fieldName, insertingElem, pos);
    changes.append(change);
    change->apply(this);
}

void DBCard::deleteElem(const enum Field & fieldName, qint32 pos) {
    QSharedPointer<DeleteElem> change = QSharedPointer<DeleteElem>::create(fieldName, pos);
    changes.append(change);
    change->apply(this);
}

void DBCard::saveToDB() {
//TODO
}

void DBCard::applyChanges() {
    targets.clear();
    examples.clear();
    source = "";
    for (auto change : changes) {
        change->apply(this);
    }
}

