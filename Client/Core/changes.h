#ifndef CHANGES_H
#define CHANGES_H
#include "ichange.h"
#include "world.h"
#include <QSharedPointer>
#include <QUuid>
#include "dbcard.h"

class ChangeSource : public IChange {
 public:

    ChangeSource();
    ChangeSource(QUuid id, const QString & newSrc);

    void apply(World * world);

 private:
    QUuid cardId;
    QString newSource;
};

class EditElem : public IChange {
 public:

    EditElem();
    EditElem(QUuid cardId, const enum Field & field, const QString & newEl, const qint32 & p);

    void apply(World * world);

 private:
    QUuid cardId;
    enum Field fieldName;
    QString newElem;
    qint32 pos;
};

class InsertElem : public IChange {
 public:

    InsertElem();
    InsertElem(QUuid id, const enum Field & field, const QString & insertingEl, const qint32 & p);

    void apply(World * world);

 private:
    QUuid cardId;
    enum Field fieldName;
    QString insertingElem;
    qint32 pos;
};

class DeleteElem : public IChange {
 public:

    DeleteElem();
    DeleteElem(QUuid id, const enum Field & field, const qint32 & p);

    void apply(World * world);

 private:
    QUuid cardId;
    enum Field fieldName;
    qint32 pos;
};

class CreateCard : public IChange {
 public:

    CreateCard();

    void apply(World * world);
 private:
    friend class World;
    QUuid cardId;
};

class DeleteCard : public IChange {
 public:

    DeleteCard(QUuid id);

    void apply(World * world);
 private:
    QUuid cardId;
};

#endif // CHANGES_H
