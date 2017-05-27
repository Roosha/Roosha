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
    QWidget* w = new QWidget();
    ConfigureManager *cm = &ConfigureManager::Instance();
    NetworkManager *nm = new NetworkManager(nullptr);
    Synchronizer *sync = new Synchronizer(nullptr, nm);


    WorldTest * testAuth = new WorldTest();
    connect(nm, &NetworkManager::successAuthorize,
            testAuth, &WorldTest::authenticationSuccess);
    connect(nm, &NetworkManager::failAuthorize,
            this, [&](qint32 id) {
                std::cout << "send auth" << std::endl;
                nm->authorize(QString("test"), QString("test"));}, Qt::ConnectionType::QueuedConnection);

    connect(nm, &NetworkManager::successRegistrate,
            testAuth, &WorldTest::authenticationSuccess);
    connect(nm, &NetworkManager::failRegistrate,this, [&](qint32 id) {
        std::cout << "send auth" << std::endl;
        nm->authorize(QString("test"), QString("test"));}, Qt::ConnectionType::QueuedConnection);
//    w->setWindowIconText(QString("test"));
//    w->show();
    nm->registrate(QString("test"), QString("test"));
//    testAuth->sendRegistrateRequest(QString("test"), QString("test"));
    test.exec();
}
