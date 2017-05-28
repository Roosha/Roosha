#include <iostream>
#include <Helpers/configuremanager.h>
#include "worldtest.h"

WorldTest::WorldTest(ConfigureManager *cm1, ConfigureManager *cm2): cm1(cm1), cm2(cm2) {

    nm1 = cm1->getNetworkManager();
    nm2 = cm2->getNetworkManager();

    sync1 = cm1->getSynchronizer();
    sync2 = cm2->getSynchronizer();

    connect(nm1, &NetworkManager::successAuthorize, this, &WorldTest::authenticationSuccess);
    connect(nm1, &NetworkManager::failAuthorize, this, [&](qint32 id) { std::cout << "send auth" << std::endl;
        nm1->authorize(QString("test"), QString("test"));}, Qt::ConnectionType::QueuedConnection);

    connect(nm1, &NetworkManager::successRegistrate, this, &WorldTest::authenticationSuccess);
    connect(nm1, &NetworkManager::failRegistrate, this, [&](qint32 id) { std::cout << "send auth" << std::endl;
        nm1->authorize(QString("test"), QString("test"));}, Qt::ConnectionType::QueuedConnection);
    connect(sync1, &Synchronizer::finishSynchronization, this, &WorldTest::updateWorld);
// connecting other CM
    connect(nm2, &NetworkManager::successAuthorize, this, &WorldTest::testSync);
    connect(nm2, &NetworkManager::failAuthorize, this, [&](qint32 id) { std::cout << "send auth" << std::endl;
        nm2->authorize(QString("test"), QString("test"));}, Qt::ConnectionType::QueuedConnection);

    connect(nm2, &NetworkManager::successRegistrate, this, &WorldTest::authenticationSuccess);
    connect(nm2, &NetworkManager::failRegistrate, this, [&](qint32 id) { std::cout << "send auth" << std::endl;
        nm2->authorize(QString("test"), QString("test"));}, Qt::ConnectionType::QueuedConnection);
    connect(sync2, &Synchronizer::finishSynchronization, this, &WorldTest::updateWorld);
}

void WorldTest::authenticationSuccess(quint32 id){
    World::version = 1;
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
    testEdition();
}

void WorldTest::testEdition() {
    World::version = 2;
    World &world = World::Instance();
    QSharedPointer<DBCard> card = world.createCard();
    QUuid id1 = card->getId();
    card->setSource(QString("src1"));
    QStringList newEx;
    QStringList newTar;
    newEx << "a" << "b" << "c";
    newTar << "x" << "y" << "z";
    card->setField(EXAMPLE, newEx);
    card->setField(TARGET, newTar);
    QCOMPARE(world.changes_.size(), 8);
    newEx.clear();
    newTar.clear();
    newEx << "a" << "c";
    newTar << "y" << "q";

    card->setField(EXAMPLE, newEx);
    card->setField(TARGET, newTar);
    QCOMPARE(world.changes_.size(), 11);
    newEx.clear();
    newTar.clear();
    newEx << "a" << "d" << "c";
    newTar << "b" << "y" ;

    card->setField(EXAMPLE, newEx);
    card->setField(TARGET, newTar);
    QCOMPARE(world.changes_.size(), 14);

    std::cout << "end TE" << std::endl;
    emit endTestEdition();
}

void WorldTest::updateWorld(ChangeList changes) {
    std::cout << "smth" << std::endl;
    World::Instance().setChanges(changes);
}

void WorldTest::testSync() {
    World::version = 1;
    World &world1 = World::Instance();
    world1.cards_.clear();
    world1.changes_.clear();

    QSharedPointer<DBCard> card = world1.createCard();

    sync1->synchronize(world1.getChanges());
    QSignalSpy spy1(sync1, &Synchronizer::finishSynchronization);
    QVERIFY(spy1.wait(3000));
    World::version = 2;
    World &world2 = World::Instance();
    world2.cards_.clear();
    world2.changes_.clear();

    card = world2.createCard();

    sync2->synchronize(world2.getChanges());
    QSignalSpy spy2(sync2, &Synchronizer::finishSynchronization);
    QVERIFY(spy2.wait(3000));
    std::cout << "HERE" << std::endl;

}

