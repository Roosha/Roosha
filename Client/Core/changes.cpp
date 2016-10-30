#include "changes.h"
#include <QDebug>


ChangeSource::ChangeSource() { }

ChangeSource::ChangeSource(const QString & newSrc) : newSource(newSrc) {}

void ChangeSource::apply(DBCard * cardptr) {
    cardptr->source = newSource;
}

EditElem::EditElem() { }

EditElem::EditElem(const enum Field & field, const QString & newEl, const qint32 & p) :
        fieldName(field), newElem(newEl), pos(p) {}

void EditElem::apply(DBCard * cardptr) {
    switch (fieldName) {
        case TARGET:
            cardptr->targets[pos] = newElem;
            break;
        case EXAMPLE:
            cardptr->examples[pos] = newElem;
            break;
    }
}

InsertElem::InsertElem() { }

InsertElem::InsertElem(const enum Field & field, const QString & insertingEl, const qint32 & p) :
        fieldName(field), insertingElem(insertingEl), pos(p) {}

void InsertElem::apply(DBCard * cardptr) {
    switch (fieldName) {
        case TARGET:
            cardptr->targets.insert(pos, insertingElem);
            break;
        case EXAMPLE:
            cardptr->examples.insert(pos, insertingElem);
            break;
    }
}

DeleteElem::DeleteElem() { }

DeleteElem::DeleteElem(const enum Field & field, const qint32 & p) : fieldName(field), pos(p) {}

void DeleteElem::apply(DBCard * cardptr) {
    switch (fieldName) {
        case TARGET:
            cardptr->targets.erase(cardptr->targets.begin() + pos);
            break;
        case EXAMPLE:
            cardptr->examples.erase(cardptr->examples.begin() + pos);
            break;
    }
}

