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

class IChange {
 public:
    ~IChange() {}
    virtual void apply(World *world) = 0;
    virtual roosha::Change toProtobuf() const = 0;
};

typedef QSharedPointer<IChange> ChangePtr;
typedef QVector<ChangePtr> ChangeList;

#endif // IDIFF_H
