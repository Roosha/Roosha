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

    static qint8 testing;
public slots:
    void authenticationSuccess(quint32 id);
    void updateWorld(ChangeList changes);

signals:
    void endTestEdition();

private:
    void testEdition();
    void testSync();
    void testEqualsHistory(ChangeList history1, ChangeList history2);
    bool resetHistory();
    ConfigureManager *cm1;
    ConfigureManager *cm2;
    NetworkManager *nm1;
    NetworkManager *nm2;
    Synchronizer *sync1;
    Synchronizer *sync2;
    std::unique_ptr<roosha::RooshaService::Stub> stub;

};

#endif // TESTS_H

