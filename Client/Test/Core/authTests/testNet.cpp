//
// Created by anna on 27.05.17.
//
#include <QObject>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <Core/centralcontroller.h>
#include <Network/network_manager.h>
#include "Helpers/configuremanager.h"
#include "Network/synchronizer.h"
#include "testNet.h"
#include "worldtest.h"

void TestNet::exec_my_tests() {
    int * argc = new int(1);
    char ** argv = new char*;
    *argv = new char;
    (*argv)[0] = '!';
    QApplication test(*argc, argv);
    WorldTest::testing = 1;

    ConfigureManager::version = 1;
    ConfigureManager *cm1 = &ConfigureManager::Instance();
    NetworkManager *nm1 = new NetworkManager(nullptr);
    Synchronizer *sync1 = new Synchronizer(nullptr, nm1);
    cm1->setNetworkManager(nm1);
    cm1->setSynchronizer(sync1);
    cm1->setMachineId(1);

    ConfigureManager::version = 2;
    ConfigureManager *cm2 = &ConfigureManager::Instance();
    NetworkManager *nm2 = new NetworkManager(nullptr);
    Synchronizer *sync2 = new Synchronizer(nullptr, nm2);
    cm2->setNetworkManager(nm2);
    cm2->setSynchronizer(sync2);
    cm2->setMachineId(2);

    qRegisterMetaType<ChangeList>("ChangeList");

    WorldTest * wt = new WorldTest(cm1, cm2);

    wt->testSimpleOps();
    wt->testEdition();

    QSignalSpy spyAuth1(nm1, &NetworkManager::successAuthorize);
    nm1->registrate(QString("test"), QString("test"));
    QVERIFY(spyAuth1.wait(5000));

    QSignalSpy spyAuth2(nm2, &NetworkManager::successAuthorize);
    nm2->registrate(QString("test"), QString("test"));
    QVERIFY(spyAuth2.wait(1000));

    wt->testSetupSync();
    wt->testCardDeletionSync();
    wt->testConflictSync();
    wt->testSync();
}
