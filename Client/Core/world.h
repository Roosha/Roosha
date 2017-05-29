#ifndef WORLD_H
#define WORLD_H
#include <QMap>
#include <QUuid>
#include "dbcard.h"
#include "card.h"
#include "Scrutiny.h"
#include <QSharedPointer>

enum Action {
    DELETE,
    INSERT,
    EDIT,
    NOTHING
};

class World {
    friend class WorldTest;
    friend class DBCard;
 public:
    static World &Instance();

    void addScrutiny(Scrutiny scrutiny);

    const QMap<QUuid, QSharedPointer<DBCard>> &getCards();
    const ChangeList &getChanges();
    void setChanges(ChangeList newChanges);

    QSharedPointer<DBCard> createCard();
    void deleteCard(QUuid id);

    void setSource(QUuid cardId, QString newSource);
    void setField(QUuid cardId, const enum Field &fieldName, QStringList newField);
    void editElem(QUuid cardId, const enum Field &fieldName, const QString &newElem, qint32 pos);
    void deleteElem(QUuid cardId, const enum Field &fieldName, qint32 pos);
    void insertElem(QUuid cardId, const enum Field &fieldName, const QString &insertingElem, qint32 pos);

    void saveToDB();
    void applyChanges();

    void insertCard(QUuid key, QSharedPointer<DBCard> card);
    void removeCard(QUuid key);

    static qint8 version;

 private:
    World();
    ~World();

#ifndef TESTS_H
//    World(const World &world) = delete;
//    World &operator=(const World &) = delete;
#endif // TESTS_H
    QMap<QUuid, QSharedPointer<DBCard>> cards_;
    ChangeList changes_;
};

#endif // WORLD_H
