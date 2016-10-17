#include "Core/dbcard.h"
#include <QDebug>

DBCard::DBCard() {

}

void DBCard::setSource(QString newSource) {
    QSharedPointer<ChangeSource> change = QSharedPointer<ChangeSource>::create(newSource);
    changes.append(change);
    change->apply(this);
}

//void DBCard::setExamples(QVector<QString> newExamples) {

//}

//void DBCard::setTarget(QVector<QString> newTarget) {

//}

void DBCard::saveToDB() {

}

void DBCard::applyChanges() {
    for (auto change : changes) {
           change->apply(this);
           qInfo() << "change applyed, result: ";
           qInfo() << source;
       }
}

