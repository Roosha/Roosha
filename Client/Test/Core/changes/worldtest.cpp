#include "worldtest.h"
#include "Core/world.h"
#include <QUuid>
#include <QDebug>

WorldTest::WorldTest() { }

void WorldTest::printCard(QSharedPointer<DBCard> card) {
    qInfo() << "id: " << card->getId();
    qInfo() << "src: " << card->getSource();
    qInfo() << "ex: ";
    for (auto ex: card->getExamples()) {
        qInfo() << ex;
    }
    qInfo() << "tar: ";
    for (auto tar: card->getTargets()) {
        qInfo() << tar;
    }
}

void WorldTest::printCards(World * world) {
    QMap<QUuid, QSharedPointer<DBCard>>::const_iterator i = world->cards.constBegin();
    while (i != world->cards.constEnd()) {
        qInfo() << i.key() << ": ";
        printCard(i.value());
        ++i;
    }
}

void WorldTest::run() {
    QUuid id;
    World world;
    id = world.createCard();
    world.insertElem(id, EXAMPLE,"ex1", 0);
    world.setSource(id, "src1");
    world.insertElem(id, EXAMPLE,"ex2", 0);
    world.deleteElem(id, EXAMPLE, 1);
    world.insertElem(id, EXAMPLE,"ex3", 0);
    world.editElem(id, EXAMPLE, "ex4", 0);
    world.insertElem(id, TARGET, "tar", 0);
    QUuid id_to_del = world.createCard();
    world.setSource(id_to_del, "src");
    world.insertElem(id_to_del, EXAMPLE,"ex", 0);
    world.deleteElem(id_to_del, EXAMPLE, 0);
    world.insertElem(id_to_del, EXAMPLE,"ex3", 0);
    world.editElem(id_to_del, EXAMPLE, "ex4", 0);
    world.insertElem(id_to_del, TARGET, "t", 0);
    id = world.createCard();
    world.setSource(id, "src");
    world.insertElem(id, EXAMPLE,"ex", 0);
    world.deleteElem(id, EXAMPLE, 0);
    world.insertElem(id, EXAMPLE,"ex", 0);
    world.editElem(id, EXAMPLE, "e", 0);
    world.insertElem(id, TARGET, "tr", 0);
    printCards(&world);
    qInfo() << "after remove \n";
    world.deleteCard(id_to_del);
    printCards(&world);
    world.applyChanges();
    qInfo() << "after reapplying in world\n";
    printCards(&world);
    World anotherWorld;
    anotherWorld.changes = world.changes;
    anotherWorld.applyChanges();

    qInfo() << "after applying in another world\n";
    printCards(&anotherWorld);
}

