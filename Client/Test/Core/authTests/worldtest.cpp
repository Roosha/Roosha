#include <iostream>
#include "worldtest.h"

void WorldTest::authenticationSuccess(quint32 id){
    World &world = World::Instance();
    QSharedPointer<DBCard> card = world.createCard();
    QUuid id1 = card->getId();
    card->insertElem(EXAMPLE, QString("ex1"), 0);
    card->setSource(QString("src1"));
    card->insertElem(EXAMPLE, QString("ex2"), 0);
    card->deleteElem(EXAMPLE, 1);
    card->insertElem(EXAMPLE, QString("ex3"), 0);
    card->editElem(EXAMPLE, QString("ex4"), 0);
    card->insertElem(TARGET, QString("tar"), 0);

    QCOMPARE(world.getCards()[id1]->getSource(), QString("src1"));
    QCOMPARE(world.getCards()[id1]->getTargets()[0], QString("tar"));
    QCOMPARE(world.getCards()[id1]->getExamples()[0], QString("ex4"));
    QCOMPARE(world.getCards()[id1]->getExamples()[1], QString("ex2"));

    card = world.createCard();
    QUuid id2 = card->getId();

    card->setSource(QString("src"));
    card->insertElem(EXAMPLE, QString("ex"), 0);
    card->deleteElem(EXAMPLE, 0);
    card->insertElem(EXAMPLE, QString("ex3"), 0);
    card->editElem(EXAMPLE, QString("ex4"), 0);
    card->insertElem(TARGET, QString("t"), 0);

    QCOMPARE(world.getCards()[id2]->getSource(), QString("src"));
    QCOMPARE(world.getCards()[id2]->getTargets()[0], QString("t"));
    QCOMPARE(world.getCards()[id2]->getExamples()[0], QString("ex4"));

    card = world.createCard();
    QUuid id3 = card->getId();
    card->setSource(QString("src"));
    card->insertElem(EXAMPLE, QString("ex"), 0);
    card->deleteElem(EXAMPLE, 0);
    card->insertElem(EXAMPLE, QString("ex"), 0);
    card->editElem(EXAMPLE, QString("e"), 0);
    card->insertElem(TARGET, QString("tr"), 0);


    QCOMPARE(world.getCards()[id3]->getSource(), QString("src"));
    QCOMPARE(world.getCards()[id3]->getTargets()[0], QString("tr"));
    QCOMPARE(world.getCards()[id3]->getExamples()[0], QString("e"));

    world.removeCard(id2);
    qInfo() << "after remove \n";

    QCOMPARE(world.getCards()[id1]->getSource(), QString("src1"));
    QCOMPARE(world.getCards()[id1]->getTargets()[0], QString("tar"));
    QCOMPARE(world.getCards()[id1]->getExamples()[0], QString("ex4"));
    QCOMPARE(world.getCards()[id1]->getExamples()[1], QString("ex2"));

    QCOMPARE(world.getCards()[id3]->getSource(), QString("src"));
    QCOMPARE(world.getCards()[id3]->getTargets()[0], QString("tr"));
    QCOMPARE(world.getCards()[id3]->getExamples()[0], QString("e"));
    world.applyChanges();
    qInfo() << "after reapplying in world\n";

    QCOMPARE(world.getCards()[id1]->getSource(), QString("src1"));
    QCOMPARE(world.getCards()[id1]->getTargets()[0], QString("tar"));
    QCOMPARE(world.getCards()[id1]->getExamples()[0], QString("ex4"));
    QCOMPARE(world.getCards()[id1]->getExamples()[1], QString("ex2"));

    QCOMPARE(world.getCards()[id3]->getSource(), QString("src"));
    QCOMPARE(world.getCards()[id3]->getTargets()[0], QString("tr"));
    QCOMPARE(world.getCards()[id3]->getExamples()[0], QString("e"));
    std::cout << "end WT" << std::endl;
    qApp->exit(0);
}

