#include "changes.h"
#include <QDebug>


ChangeSource::ChangeSource() { }

ChangeSource::ChangeSource(const QString & newSrc) : newSource(newSrc) {}

void ChangeSource::apply(DBCard * cardptr) {
    cardptr->source = newSource;
}

EditExampleElem::EditExampleElem() { }

EditExampleElem::EditExampleElem(const QString & newEl, const qint32 & p) : newElem(newEl), pos(p) {}

void EditExampleElem::apply(DBCard * cardptr) {
    cardptr->examples[pos] = newElem;
}

InsertExampleElem::InsertExampleElem() { }

InsertExampleElem::InsertExampleElem(const QString & insertingEl, const qint32 & p) : insertingElem(insertingEl), pos(p) {}

void InsertExampleElem::apply(DBCard * cardptr) {
    cardptr->examples.insert(pos, insertingElem);
}

DeleteExampleElem::DeleteExampleElem() { }

DeleteExampleElem::DeleteExampleElem(const qint32 & p) : pos(p) {}

void DeleteExampleElem::apply(DBCard * cardptr) {
    cardptr->examples.erase(cardptr->examples.begin() + pos);
}

EditTargetElem::EditTargetElem() { }

EditTargetElem::EditTargetElem(const QString & newEl, const qint32 & p) : newElem(newEl), pos(p) {}

void EditTargetElem::apply(DBCard * cardptr) {
    cardptr->targets[pos] = newElem;
}

InsertTargetElem::InsertTargetElem() { }

InsertTargetElem::InsertTargetElem(const QString & insertingEl, const qint32 & p) : insertingElem(insertingEl), pos(p) {}

void InsertTargetElem::apply(DBCard * cardptr) {
    cardptr->targets.insert(pos, insertingElem);
}

DeleteTargetElem::DeleteTargetElem() { }

DeleteTargetElem::DeleteTargetElem(const qint32 & p) : pos(p) {}

void DeleteTargetElem::apply(DBCard * cardptr) {
    cardptr->targets.erase(cardptr->targets.begin() + pos);
}
