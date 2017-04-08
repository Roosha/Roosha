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

class IChange {
 public:
    ~IChange();
    IChange(roosha::Change change);

    virtual void apply(World *world) = 0;
    roosha::Change toProtobuf() const {
        return rawChange;
    }
    CMP compare(IChange* otherChange);
 protected:
    roosha::Change rawChange;
};

typedef QSharedPointer<IChange> ChangePtr;
typedef QVector<ChangePtr> ChangeList;

#endif // IDIFF_H
