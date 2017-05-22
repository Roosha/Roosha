//
// Created by alex on 04.02.17.
//
#define private public
#include <QtTest>
#include "Test/Core/TestLSequence.h"
#include "Test/Core/changes/worldtest.h"

int main(int argc, char** argv) {
    int status = 0;

    auto RUN_TEST = [&status, argc, argv](QObject *test) {
        status |= QTest::qExec(test, argc, argv);
        delete test;
    };

    // add new tests here
    RUN_TEST(new TestLSequence());
    RUN_TEST(new WorldTest());

    return status;
}