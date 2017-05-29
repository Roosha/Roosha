#include <iostream>
#include <Helpers/configuremanager.h>
#include "worldtest.h"

qint8 WorldTest::testing = 0;

WorldTest::WorldTest(ConfigureManager *cm1, ConfigureManager *cm2): cm1(cm1), cm2(cm2) {

    nm1 = cm1->getNetworkManager();
    nm2 = cm2->getNetworkManager();

    sync1 = cm1->getSynchronizer();
    sync2 = cm2->getSynchronizer();


    auto channel = grpc::CreateChannel(ROOSHA_SERVER_ADDRESS, grpc::InsecureChannelCredentials());
    stub = roosha::RooshaService::NewStub(channel);

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
    while (!resetHistory()) { std::cout << "smth went wrong"; }
    World::version = 1;
    World &world1 = World::Instance();
    world1.cards_.clear();
    world1.changes_.clear();

    // two changes always generate when user creates new card: creation of the card and setting empty source, let's do the same in our tests
    QSharedPointer<DBCard> card = world1.createCard();
    card->setSource("");
    QUuid id1 = card->getId();

    sync1->synchronize(world1.getChanges());
    QSignalSpy spy1(sync1, &Synchronizer::finishSynchronization);
    QVERIFY(spy1.wait(1000));

    World::version = 2;
    World &world2 = World::Instance();
    world2.cards_.clear();
    world2.changes_.clear();

    card = world2.createCard();
    card->setSource("");

    QUuid id2 = card->getId();

    sync2->synchronize(world2.getChanges());
    QSignalSpy spy2(sync2, &Synchronizer::finishSynchronization);
    QVERIFY(spy2.wait(1000));
    // now we send 1 card create change from "first" device of client "test/test" (and source setter)
    // and after get it from "second" device and send its card create change
    QCOMPARE(world1.cards_.size(), 1);
    QCOMPARE(world2.cards_.size(), 2);
    QCOMPARE(world1.changes_.size(), 2);
    QCOMPARE(world2.changes_.size(), 4);
    QSet<QUuid> ids;
    ids << id1 << id2;
    QCOMPARE(QSet<QUuid>::fromList(world2.cards_.keys()), ids);

    // emulate actions from first device

    World::version = 1;
    QSharedPointer<DBCard> card1 = world1.getCards()[id1];
    card1->setSource("a");
    QStringList newTar1;
    QStringList newEx1;
    newTar1  << "б" << "в";
    newEx1 << "asdf";
    card1->setField(TARGET, newTar1);
    card1->setField(EXAMPLE, newEx1);
    // now it should generate 4 new changes on first device, let's synchronize it
    sync1->synchronize(world1.getChanges());
    QVERIFY(spy1.wait(1000));
    // now we have 2 cards and 4 changes + 2 create card changes + 2 initial setters, they also exists on the server
    QCOMPARE(world1.cards_.size(), 2);
    QCOMPARE(world1.changes_.size(), 8);


    // emulate actions from second device

    World::version = 2;
    card1 = world2.getCards()[id1];
    card1->setSource("g"); // conflicting change of source field
    newTar1.clear();
    newEx1.clear();
    newTar1  << "г";
    newEx1 << "fdsa";
    card1->setField(TARGET, newTar1);
    card1->setField(EXAMPLE, newEx1);
    // now it should generate 3 new changes on second device, one of them conflicting, choose server from conflicting pair when we will syncronize
    // server change now is setting "a" as a source field, it is already synchronized from first device
    // server change will be chosen when WorldTest::testing is odd, client change will be chosen when WorldTest::testing is even
    // we increment it each time when making choice, so we know which behavior we expect
    WorldTest::testing = 1;
    // before synchronization also will delete second card
    // then if we try edit it from other device and try to synchronize, it should be deleted from other device anyway
    world2.deleteCard(id2);

    sync2->synchronize(world2.getChanges());
    QVERIFY(spy2.wait(1000));
    QCOMPARE(world2.cards_.size(), 1);
    QCOMPARE(world2.changes_.size(), 11);
    QCOMPARE(world2.cards_[id1]->getSource(), QString("a"));
    // we don't know ordering of elements, inserting from differ devices in lseq
    // thus one more testcase when we manually check the result and after will make some changes and sync both devices with comparing result


    // let's try edit card that already deleted on server
    World::version = 1;

    QSharedPointer<DBCard> card2 = world1.getCards()[id2];
    card2->setSource("fegkh"); // conflicting change of source field
    QStringList newTar2;
    QStringList newEx2;
    newTar2  << "jsan" << "sdbl";
    newEx2 << "fajn" << "bhsadl";
    card2->setField(TARGET, newTar1);
    card2->setField(EXAMPLE, newEx1);

    sync1->synchronize(world1.getChanges());
    QVERIFY(spy1.wait(1000));


    QCOMPARE(world1.changes_.size(), 11);


    // now state must be equal on both devices

    testEqualsHistory(world1.changes_, world2.changes_);

    World::version = 1;
    card = world1.getCards()[id1];
    newTar1.clear();
    newEx1.clear();
    newTar1  << "sahsdv";
    newEx1 << "gadd";
    card->setSource("adh");
    card->setField(TARGET, newTar1);
    card->setField(EXAMPLE, newEx1);

    sync1->synchronize(world1.getChanges());
    QVERIFY(spy1.wait(1000));

    World::version = 2;
    card = world2.getCards()[id1];
    newTar1.clear();
    newEx1.clear();
    newTar1  << "sahsdv" << "aldfhl" << "gyasbdl";
    newEx1 << "gadd" << "adshh" << "uasdhf";
    card->setSource("adh");
    card->setField(TARGET, newTar1);
    card->setField(EXAMPLE, newEx1);

    sync2->synchronize(world2.getChanges());
    QVERIFY(spy2.wait(1000));

    World::version = 1;

    sync1->synchronize(world1.getChanges());
    QVERIFY(spy1.wait(1000));

    testEqualsHistory(world1.changes_, world2.changes_);

    std::cout << "END" << std::endl;
    qApp->exit(0);
}

bool WorldTest::resetHistory() {
    roosha::Void request;
    roosha::Void response;
    grpc::ClientContext context;
    context.AddMetadata(TOKEN_METADATA_KEY, ConfigureManager::Instance().getToken().toStdString());
    grpc::Status status = stub->resetHistory(&context, request, &response);
    return status.ok();
}

void WorldTest::testEqualsHistory(ChangeList history1, ChangeList history2) {
    QCOMPARE(history1.size(), history2.size());
    for (int i = 0; i < history1.size(); i++) {
        QCOMPARE(history1[i]->compare(history2[i]), EQUAL);
    }
}

