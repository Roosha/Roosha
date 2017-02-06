//
// Created by alex on 04.02.17.
//

#include <QtTest/QtTest>
#include <Core/QLSeq.h>

class TestLSequence: public QObject
{
Q_OBJECT
private slots:
    void creation();
    void insertion();
    void removing();
    void keyMapping();
    void highload();
    void nullTerminatedSymbol();
};