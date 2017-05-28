#ifndef TESTS_H
#define TESTS_H

#include "Core/world.h"
#include <QtTest/QtTest>
#include "Network/synchronizer.h"
#include "Helpers/configuremanager.h"


class WorldTest: public QObject  {
Q_OBJECT
public:
    WorldTest(ConfigureManager *cm1, ConfigureManager *cm2);
public slots:
    void authenticationSuccess(quint32 id);
    void updateWorld(ChangeList changes);

signals:
    void endTestEdition();

private:
    void testEdition();
    void testSync();
    ConfigureManager *cm1;
    ConfigureManager *cm2;
    NetworkManager *nm1;
    NetworkManager *nm2;
    Synchronizer *sync1;
    Synchronizer *sync2;

};

#endif // TESTS_H

