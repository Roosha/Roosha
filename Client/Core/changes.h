#ifndef CHANGES_H
#define CHANGES_H
#include "ichange.h"
#include "world.h"
#include <QSharedPointer>
#include <QUuid>
#include "dbcard.h"
#include "Core/QLSeq.h"


class CardChange : public IChange {
 public:
    CardChange(roosha::Change change);

    QUuid getCardId() const;
};


class ChangeSource : public CardChange {
 public:
    ChangeSource(QUuid id, const QString &newSrc);
    ChangeSource(roosha::Change change);

    void apply(World *world) override;

    QString getNewSource() const;
};

class EditElem : public CardChange {
 public:
    EditElem();
    EditElem(roosha::Change change);
    EditElem(QUuid cardId, const enum Field &field, const QString &newEl, const QLKey &p);

    void apply(World *world) override;
    void buildAndApply(QUuid id, const enum Field &field, const QString &newEl, const qint32 &p, World* world);

    Field getFieldName() const;
    QString getNewElem() const;
    QLKey getPos() const;

};

class InsertElem : public CardChange {
 public:
    InsertElem();
    InsertElem(roosha::Change change);
    InsertElem(QUuid id, const enum Field &field, const QString &insertingEl, const QLKey &p);

    void apply(World *world) override;
    void buildAndApply(QUuid id, const enum Field &field, const QString &insertingEl, const qint32 &p, World* world);

    Field getFieldName() const;
    QString getInsertingElem() const;
    QLKey getPos() const;

};

class DeleteElem : public CardChange {
 public:
    DeleteElem();
    DeleteElem(roosha::Change change);
    DeleteElem(QUuid id, const enum Field &field, const QLKey &p); // TODO: deprecated

    void apply(World *world) override;
    void buildAndApply(QUuid id, const enum Field &field, const qint32 &p, World* world);

    Field getFieldName() const;
    QLKey getPos() const;

};

class CreateCard : public CardChange {
 public:
    CreateCard(QUuid id);
    CreateCard(roosha::Change change);

    void apply(World *world) override;


};

class DeleteCard : public CardChange {
 public:
    DeleteCard(QUuid id);
    DeleteCard(roosha::Change change);


    void apply(World *world) override;

};

#endif // CHANGES_H
