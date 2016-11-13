#ifndef IDIFF_H
#define IDIFF_H
#include <QUuid>

class DBCard;
class World;

enum Field {
    EXAMPLE,
    TARGET
};

class IChange {
 public:
    ~IChange() {}
    virtual void apply(World * world) = 0;

};

#endif // IDIFF_H
