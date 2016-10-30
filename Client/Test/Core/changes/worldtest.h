#ifndef WORLDTEST_H
#define WORLDTEST_H
#include "Core/world.h"

class WorldTest {
 public:
    WorldTest();
    void run();
    void printCards(World * world);
    void printCard(QSharedPointer<DBCard> card);
};

#endif // WORLDTEST_H
