#ifndef TESTS_H
#define TESTS_H

#include "Core/world.h"
#include <QtTest/QtTest>


class WorldTest: public QObject  {
Q_OBJECT
public slots:
    void authenticationSuccess(quint32 id);

private:
    void testEdition();
};

#endif // TESTS_H

