#include "worldtest.h"

void WorldTest::run() {
    World &world = World::Instance();
    QSharedPointer<DBCard> card = world.createCard();
    QUuid id1 = card->getId();
    card->insertElem(EXAMPLE, "ex1", 0);
    card->setSource("src1");
    card->insertElem(EXAMPLE, "ex2", 0);
    card->deleteElem(EXAMPLE, 1);
    card->insertElem(EXAMPLE, "ex3", 0);
    card->editElem(EXAMPLE, "ex4", 0);
    card->insertElem(TARGET, "tar", 0);

    QCOMPARE(world.getCards()[id1]->getSource(), "src1");
    QCOMPARE(world.getCards()[id1]->getTargets()[0], "tar");
    QCOMPARE(world.getCards()[id1]->getExamples()[0], "ex4");
    QCOMPARE(world.getCards()[id1]->getExamples()[1], "ex2");

    card = world.createCard();
    QUuid id2 = card->getId();

    card->setSource("src");
    card->insertElem(EXAMPLE, "ex", 0);
    card->deleteElem(EXAMPLE, 0);
    card->insertElem(EXAMPLE, "ex3", 0);
    card->editElem(EXAMPLE, "ex4", 0);
    card->insertElem(TARGET, "t", 0);

    QCOMPARE(world.getCards()[id2]->getSource(), "src");
    QCOMPARE(world.getCards()[id2]->getTargets()[0], "t");
    QCOMPARE(world.getCards()[id2]->getExamples()[0], "ex4");

    card = world.createCard();
    QUuid id3 = card->getId();
    card->setSource("src");
    card->insertElem(EXAMPLE, "ex", 0);
    card->deleteElem(EXAMPLE, 0);
    card->insertElem(EXAMPLE, "ex", 0);
    card->editElem(EXAMPLE, "e", 0);
    card->insertElem(TARGET, "tr", 0);


    QCOMPARE(world.getCards()[id3]->getSource(), "src");
    QCOMPARE(world.getCards()[id3]->getTargets()[0], "tr");
    QCOMPARE(world.getCards()[id3]->getExamples()[0], "e");

    world.removeCard(id2);
//    qInfo() << "after remove \n";

    QCOMPARE(world.getCards()[id1]->getSource(), "src1");
    QCOMPARE(world.getCards()[id1]->getTargets()[0], "tar");
    QCOMPARE(world.getCards()[id1]->getExamples()[0], "ex4");
    QCOMPARE(world.getCards()[id1]->getExamples()[1], "ex2");

    QCOMPARE(world.getCards()[id3]->getSource(), "src");
    QCOMPARE(world.getCards()[id3]->getTargets()[0], "tr");
    QCOMPARE(world.getCards()[id3]->getExamples()[0], "e");
    world.applyChanges();
//    qInfo() << "after reapplying in world\n";

    QCOMPARE(world.getCards()[id1]->getSource(), "src1");
    QCOMPARE(world.getCards()[id1]->getTargets()[0], "tar");
    QCOMPARE(world.getCards()[id1]->getExamples()[0], "ex4");
    QCOMPARE(world.getCards()[id1]->getExamples()[1], "ex2");

    QCOMPARE(world.getCards()[id3]->getSource(), "src");
    QCOMPARE(world.getCards()[id3]->getTargets()[0], "tr");
    QCOMPARE(world.getCards()[id3]->getExamples()[0], "e");
}

