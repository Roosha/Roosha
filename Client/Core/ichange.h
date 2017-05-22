#ifndef IDIFF_H
#define IDIFF_H
#include <QUuid>
#include <QSharedPointer>
#include <QVector>

#include "Network/Proto/changes.pb.h"

class DBCard;
class World;

enum Field {
    EXAMPLE,
    TARGET
};
enum CMP {
    EQUAL,
    DIFFER,
    CONFLICT
};

enum Deletion {
    THIS_DELETES_OTHER,
    OTHER_DELETES_THIS,
    NONE
};

class IChange {
 public:
    ~IChange();
    IChange(roosha::Change change);

    virtual void apply(World *world) = 0;
    roosha::Change toProtobuf() const {
        return rawChange;
    }
    CMP compare(QSharedPointer<IChange> otherChange);
    Deletion checkForDeletion(QSharedPointer<IChange> otherChange);
 protected:
    roosha::Change rawChange;

 private:
    static bool isFirstDeletedBySecond(const roosha::Change &first, const roosha::Change &second);
};

typedef QSharedPointer<IChange> ChangePtr;
typedef QVector<ChangePtr> ChangeList;

#endif // IDIFF_H
