#ifndef CHANGES_H
#define CHANGES_H
#include "ichange.h"
#include "world.h"
#include <QSharedPointer>
#include <QUuid>
#include "dbcard.h"
#include "Core/QLSeq.h"

class ChangeSource : public IChange {
 public:
    ChangeSource(QUuid id, const QString &newSrc);

    void apply(World *world) override;
    roosha::Change toProtobuf() const override;

    QUuid getCardId() const;
    QString getNewSource() const;

 private:
    const QUuid cardId;
    const QString newSource;
};

class EditElem : public IChange {
 public:
    EditElem();
    EditElem(QUuid cardId, const enum Field &field, const QString &newEl, const QLKey &p);

    void apply(World *world) override;
    void buildAndApply(QUuid id, const enum Field &field, const QString &newEl, const qint32 &p, World* world);
    roosha::Change toProtobuf() const override;

    QUuid getCardId() const;
    Field getFieldName() const;
    QString getNewElem() const;
    QLKey getPos() const;

 private:
    QUuid cardId;
    enum Field fieldName;
    QString newElem;
    QLKey pos;
};

class InsertElem : public IChange {
 public:
    InsertElem();
    InsertElem(QUuid id, const enum Field &field, const QString &insertingEl, const QLKey &p);

    void apply(World *world) override;
    void buildAndApply(QUuid id, const enum Field &field, const QString &insertingEl, const qint32 &p, World* world);
    roosha::Change toProtobuf() const override;

    QUuid getCardId() const;
    Field getFieldName() const;
    QString getInsertingElem() const;
    QLKey getPos() const;

 private:
    QUuid cardId;
    enum Field fieldName;
    QString insertingElem;
    QLKey pos;
};

class DeleteElem : public IChange {
 public:
    DeleteElem();
    DeleteElem(QUuid id, const enum Field &field, const QLKey &p); // TODO: deprecated

    void apply(World *world) override;
    void buildAndApply(QUuid id, const enum Field &field, const qint32 &p, World* world);
    roosha::Change toProtobuf() const override;

    QUuid getCardId() const;
    Field getFieldName() const;
    QLKey getPos() const;

 private:
    QUuid cardId;
    enum Field fieldName;
    QLKey pos;
};

class CreateCard : public IChange {
 public:
    CreateCard(QUuid id);

    void apply(World *world) override;
    roosha::Change toProtobuf() const override;

    QUuid getCardId() const;

 private:
    const QUuid cardId;
};

class DeleteCard : public IChange {
 public:
    DeleteCard(QUuid id);

    void apply(World *world) override;
    roosha::Change toProtobuf() const override;

    QUuid getCardId() const;

 private:
    const QUuid cardId;
};

#endif // CHANGES_H
