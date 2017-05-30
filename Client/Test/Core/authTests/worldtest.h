#ifndef TESTS_H
#define TESTS_H

#include "Core/world.h"
#include <QtTest/QtTest>
#include "Network/synchronizer.h"
#include "Helpers/configuremanager.h"
#include <Network/Proto/roosha_service.grpc.pb.h>



class WorldTest: public QObject  {
Q_OBJECT
public:
    WorldTest(ConfigureManager *cm1, ConfigureManager *cm2);

    void testSimpleOps();
    void testEdition();
    void testSetupSync();
    void testSync();
    void testConflictSync();
    void testCardDeletionSync();


    static qint8 testing;
public slots:
    void updateWorld(ChangeList changes);
signals:
    void endTestCase();


private:
    void testEqualsHistory(ChangeList history1, ChangeList history2);
    bool resetHistory();
    void setup(bool two);
    QUuid ids[2];
    ConfigureManager *cm1;
    ConfigureManager *cm2;
    NetworkManager *nm1;
    NetworkManager *nm2;
    Synchronizer *sync1;
    Synchronizer *sync2;

    std::unique_ptr<roosha::RooshaService::Stub> stub;

};

#endif // TESTS_H

