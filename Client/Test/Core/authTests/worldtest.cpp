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

    connect(nm1, &NetworkManager::failAuthorize, this, [&](qint32 id) { std::cout << "send auth" << std::endl;
        nm1->authorize(QString("test"), QString("test"));}, Qt::ConnectionType::QueuedConnection);

    connect(nm1, &NetworkManager::failRegistrate, this, [&](qint32 id) { std::cout << "send auth" << std::endl;
        nm1->authorize(QString("test"), QString("test"));}, Qt::ConnectionType::QueuedConnection);

    connect(sync1, &Synchronizer::finishSynchronization, this, &WorldTest::updateWorld);

// connecting other CM

    connect(nm2, &NetworkManager::failAuthorize, this, [&](qint32 id) { std::cout << "send auth" << std::endl;
        nm2->authorize(QString("test"), QString("test"));}, Qt::ConnectionType::QueuedConnection);

    connect(nm2, &NetworkManager::failRegistrate, this, [&](qint32 id) { std::cout << "send auth" << std::endl;
        nm2->authorize(QString("test"), QString("test"));}, Qt::ConnectionType::QueuedConnection);

    connect(sync2, &Synchronizer::finishSynchronization, this, &WorldTest::updateWorld);
}

bool WorldTest::resetHistory() {
    World::version = 1;
    World &world1 = World::Instance();
    world1.cards_.clear();
    world1.changes_.clear();
    World::version = 2;
    World &world2 = World::Instance();
    world2.cards_.clear();
    world2.changes_.clear();
    sync1->resetSyncPrefix();
    sync2->resetSyncPrefix();

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

void WorldTest::testSimpleOps() {
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
}

void WorldTest::updateWorld(ChangeList changes) {
    std::cout << "world was updated" << std::endl;
    World::Instance().setChanges(changes);
}
// creates one or two cards: one from first device, send it to server, then if two create another card from second device else just sync second device
void WorldTest::setup(bool two) {
    while (!resetHistory()) { std::cout << "smth went wrong"; }

    World::version = 1;
    World &world1 = World::Instance();
    // two changes always generate when user creates new card: creation of the card and setting empty source, let's do the same in our tests
    QSharedPointer<DBCard> card = world1.createCard();
    card->setSource("");
    ids[0] = card->getId();

    QSignalSpy spy1(sync1, &Synchronizer::finishSynchronization);
    sync1->synchronize(world1.getChanges());
    QVERIFY(spy1.wait(1000));

    World::version = 2;
    World &world2 = World::Instance();
    if (two) {
        card = world2.createCard();
        card->setSource("");

        ids[1] = card->getId();
    }
    QSignalSpy spy2(sync2, &Synchronizer::finishSynchronization);
    sync2->synchronize(world2.getChanges());
    QVERIFY(spy2.wait(1000));
    // now we send 1 card create change from "first" device of client "test/test" (and source setter)
    // and after get it from "second" device and send its card create change
}

void WorldTest::testSetupSync() {
    setup(true);
    World::version = 1;
    World &world1 = World::Instance();
    World::version = 2;
    World &world2 = World::Instance();


    QCOMPARE(world1.cards_.size(), 1);
    QCOMPARE(world2.cards_.size(), 2);
    QCOMPARE(world1.changes_.size(), 2);
    QCOMPARE(world2.changes_.size(), 4);
    QSet<QUuid> Id;
    Id << ids[0] << ids[1];
    QCOMPARE(QSet<QUuid>::fromList(world2.cards_.keys()), Id);

    std::cout << "test setup OK" << std::endl;
    emit endTestCase();
}

void WorldTest::testCardDeletionSync() {
    setup(true);
    World::version = 1;
    World &world1 = World::Instance();
    QSignalSpy spy1(sync1, &Synchronizer::finishSynchronization);
    sync1->synchronize(world1.getChanges());
    QVERIFY(spy1.wait(1000));

    // two cards on each devices

    World::version = 2;
    World &world2 = World::Instance();
    // before synchronization will delete second card
    world2.deleteCard(ids[1]);

    QSignalSpy spy2(sync2, &Synchronizer::finishSynchronization);
    sync2->synchronize(world2.getChanges());
    QVERIFY(spy2.wait(1000));

    // then if we try edit it from other device and try to synchronize, it should be deleted from other device anyway
    // let's try edit card that already deleted on server
    World::version = 1;

    QSharedPointer<DBCard> card = world1.getCards()[ids[1]];
    card->setSource("fegkh"); // conflicting change of source field
    QStringList newTar;
    QStringList newEx;
    newTar  << "jsan" << "sdbl";
    newEx << "fajn" << "bhsadl";
    card->setField(TARGET, newTar);
    card->setField(EXAMPLE, newEx);

    sync1->synchronize(world1.getChanges());
    QVERIFY(spy1.wait(1000));

    QCOMPARE(world1.cards_.size(), 1);
    QCOMPARE(world1.changes_.size(), 5);
    std::cout << "test card deletion OK" << std::endl;
    emit endTestCase();
}

void WorldTest::testConflictSync() {
    setup(false);

    // emulate actions from first device
    World::version = 1;
    World &world1 = World::Instance();

    QSharedPointer<DBCard> card = world1.getCards()[ids[0]];
    card->setSource("a");
    QStringList newTar;
    QStringList newEx;
    newTar  << "б" << "в";
    newEx << "asdf";
    card->setField(TARGET, newTar);
    card->setField(EXAMPLE, newEx);
    // now it should generate 4 new changes on first device, let's synchronize it
    QSignalSpy spy1(sync1, &Synchronizer::finishSynchronization);
    sync1->synchronize(world1.getChanges());
    QVERIFY(spy1.wait(1000));
    // now we have 1 card and 4 changes + 1 create card change + 1 initial setter, they also exists on the server
    QCOMPARE(world1.cards_.size(), 1);
    QCOMPARE(world1.changes_.size(), 6);


    // emulate actions from second device

    World::version = 2;
    World &world2 = World::Instance();

    card = world2.getCards()[ids[0]];
    card->setSource("g"); // conflicting change of source field
    newTar.clear();
    newEx.clear();
    newTar  << "г";
    newEx << "fdsa";
    card->setField(TARGET, newTar);
    card->setField(EXAMPLE, newEx);
    // now it should generate 3 new changes on second device, one of them conflicting, choose server from conflicting pair when we will syncronize
    // server change now is setting "a" as a source field, it is already synchronized from first device
    // server change will be chosen when WorldTest::testing is odd, client change will be chosen when WorldTest::testing is even
    // we increment it each time when making choice, so we know which behavior we expect
    WorldTest::testing = 1;

    World::version = 2;
    QSignalSpy spy2(sync2, &Synchronizer::finishSynchronization);
    sync2->synchronize(world2.getChanges());
    QVERIFY(spy2.wait(1000));

    QCOMPARE(world2.cards_[ids[0]]->getSource(), QString("a"));

    World::version = 1;

    sync1->synchronize(world1.getChanges());
    QVERIFY(spy1.wait(1000));

    testEqualsHistory(world1.getChanges(), world2.getChanges());
    std::cout << "test conflict OK" << std::endl;
    emit endTestCase();
}

void WorldTest::testSync() {

    // we don't know ordering of elements, inserting from differ devices in lseq
    // more testcases when we don't manually check the result but make some changes and sync both devices with comparing result

    setup(false);

    World::version = 1;
    World &world1 = World::Instance();

    World::version = 1;
    QSharedPointer<DBCard> card = world1.getCards()[ids[0]];
    QStringList newTar;
    QStringList newEx;
    newTar  << "sahsdv" << "aldfhl" << "gyasbdl";
    newEx << "gadd" << "adshh" << "uasdhf";
    card->setField(TARGET, newTar);
    card->setField(EXAMPLE, newEx);

    QSignalSpy spy1(sync1, &Synchronizer::finishSynchronization);
    sync1->synchronize(world1.getChanges()); // send changes to server
    QVERIFY(spy1.wait(1000));

    World::version = 2;
    World &world2 = World::Instance();

    QSignalSpy spy2(sync2, &Synchronizer::finishSynchronization);
    sync2->synchronize(world2.getChanges()); // get changes from another device
    QVERIFY(spy2.wait(1000));

    testEqualsHistory(world1.changes_, world2.changes_);

    card = world2.getCards()[ids[0]];
    newTar.clear();
    newEx.clear();
    newTar  << "sahdv"; // this will add 2 change deleteElems to server
    newEx << "gad"; // the same
    card->setSource("adh");

    card->setField(TARGET, newTar);
    card->setField(EXAMPLE, newEx);

    sync2->synchronize(world2.getChanges()); // send changes to server from second device
    QVERIFY(spy2.wait(1000));

    World::version = 1;
    card = world1.getCards()[ids[0]];
    newTar.clear();
    newEx.clear();
    newTar  << "sfas" << "aldfsl" << "abdl"; //conflicting edit of first elem, and edition elems that already deleted in server state (should be deleted anyway)
    newEx << "gd" << "uahf";
    card->setField(TARGET, newTar);
    card->setField(EXAMPLE, newEx);


    sync1->synchronize(world1.getChanges()); // send changes to server
    QVERIFY(spy1.wait(1000));

    // card should be:
    // source: adh
    // targets: sfas
    // examples: gadd

    card = world1.getCards()[ids[0]];
    QCOMPARE(card->getSource(), QString("adh"));
    QCOMPARE(card->getExamples().size(), 1);
    QCOMPARE(card->getTargets()[0], QString("sfas"));
    QCOMPARE(card->getTargets().size(), 1);
    QCOMPARE(card->getExamples()[0], QString("gad"));
    // finally, get current server state from second device

    World::version = 2;
    sync2->synchronize(world2.getChanges());
    QVERIFY(spy2.wait(1000));

    QVERIFY(WorldTest::testing > 3); // we encountered several conflicts during tests

    testEqualsHistory(world1.changes_, world2.changes_);

    std::cout << "END" << std::endl;
}