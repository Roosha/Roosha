#ifndef CHANGES_H
#define CHANGES_H
#include "ichange.h"
#include "world.h"
#include <QSharedPointer>
#include <QUuid>
#include "dbcard.h"

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
    EditElem(QUuid cardId, const enum Field &field, const QString &newEl, const qint32 &p);

    void apply(World *world) override;
    roosha::Change toProtobuf() const override;

    QUuid getCardId() const;
    Field getFieldName() const;
    QString getNewElem() const;
    qint32 getPos() const;

 private:
    const QUuid cardId;
    const enum Field fieldName;
    const QString newElem;
    const qint32 pos;
};

class InsertElem : public IChange {
 public:
    InsertElem(QUuid id, const enum Field &field, const QString &insertingEl, const qint32 &p);

    void apply(World *world) override;
    roosha::Change toProtobuf() const override;

    QUuid getCardId() const;
    Field getFieldName() const;
    QString getInsertingElem() const;
    qint32 getPos() const;

 private:
    const QUuid cardId;
    const enum Field fieldName;
    const QString insertingElem;
    const qint32 pos;
};

class DeleteElem : public IChange {
 public:
    DeleteElem(QUuid id, const enum Field &field, const qint32 &p);

    void apply(World *world) override;
    roosha::Change toProtobuf() const override;

    QUuid getCardId() const;
    Field getFieldName() const;
    qint32 getPos() const;

 private:
    const QUuid cardId;
    const enum Field fieldName;
    const qint32 pos;
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
