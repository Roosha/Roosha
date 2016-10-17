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

void DBCard::editExampleElem(QString newElem, qint32 pos) {
    QSharedPointer<EditExampleElem> change = QSharedPointer<EditExampleElem>::create(newElem, pos);
    changes.append(change);
    change->apply(this);
}

void DBCard::editTargetElem(QString newElem, qint32 pos) {
    QSharedPointer<EditTargetElem> change = QSharedPointer<EditTargetElem>::create(newElem, pos);
    changes.append(change);
    change->apply(this);
}


void DBCard::deleteExampleElem(qint32 pos) {
    QSharedPointer<DeleteExampleElem> change = QSharedPointer<DeleteExampleElem>::create(pos);
    changes.append(change);
    change->apply(this);
}

void DBCard::deleteTargetElem(qint32 pos) {
    QSharedPointer<DeleteTargetElem> change = QSharedPointer<DeleteTargetElem>::create(pos);
    changes.append(change);
    change->apply(this);
}

void DBCard::insertExampleElem(QString insertingElem, qint32 pos) {
    QSharedPointer<InsertExampleElem> change = QSharedPointer<InsertExampleElem>::create(insertingElem, pos);
    changes.append(change);
    change->apply(this);
}

void DBCard::insertTargetElem(QString insertingElem, qint32 pos){
    QSharedPointer<InsertTargetElem> change = QSharedPointer<InsertTargetElem>::create(insertingElem, pos);
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

