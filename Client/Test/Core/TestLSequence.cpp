;//
// Created by alex on 04.02.17.
//

#include "TestLSequence.h"
#include <random>

void TestLSequence::creation() {
    QLSeq s;

    QCOMPARE(s.size(), 0);
}

void TestLSequence::insertion() {
    QLSeq s;

    s.insertAfter(-1, "Hello");

    QCOMPARE(s.size(), 1);
    QCOMPARE(s.at(0), QString("Hello"));
    QVERIFY_EXCEPTION_THROWN(s.at(1), std::out_of_range);

    s.insertAfter(s.lastKey(), "World");

    QCOMPARE(s.size(), 2);
    QCOMPARE(s.at(0), QString("Hello"));
    QCOMPARE(s.at(1), QString("World"));
    QVERIFY_EXCEPTION_THROWN(s.at(2), std::out_of_range);

    QCOMPARE(s.toList(), QStringList({"Hello", "World"}));

    s.remove("Hello");
    s.remove("World");

    QCOMPARE(s.size(), 0);

    s.insert(QLKey({10}), "A");
    s.insert(QLKey({11}), "D");

    s.insertAfter(QLKey{10}, "C");
    s.insertAfter(0, "B");

    QCOMPARE(s.toList(), QStringList({"A", "B", "C", "D"}));


    QLSeq s2;
    s2.insert(QLKey{10, 3}, "A");
    s2.insert(QLKey{15}, "B");
    s2.insert(QLKey{10, 20, 70}, "D");
    s2.insert(QLKey{10, 2}, "C");

    s2.insertAfter(2, "CD");

    QCOMPARE(s2.toList(), QStringList({"C", "A", "D", "CD", "B"}));

}

void TestLSequence::removing() {
    QLSeq s;

    s.insertAfter(-1, "Hello");
    s.insertAfter(0, "Yo");

    s.remove("Hello");
    QCOMPARE(s.size(), 1);
    QCOMPARE(s.at(0), QString("Yo"));
    QCOMPARE(s.toList(), QStringList({"Yo"}));
}

void TestLSequence::keyMapping() {
    QLSeq s;

    s.insertAfter(s.lastKey(), "1");
    s.insertAfter(s.lastKey(), "2");
    s.insertAfter(s.lastKey(), "3");

    QCOMPARE(s.at(s.firstKey()), QString("1"));
    QCOMPARE(s.at(s.lastKey()), QString("3"));
}

void TestLSequence::highload() {
    QLSeq s;
    QStringList l;

    for(int i = 0; i < 127*3; i++) {
        s.insertAfter(s.lastKey(), "Q");
        l.push_back("Q");
    }
    QCOMPARE(s.toList(), l);

    int max = 0;
    int count = 0;
    for(auto it = s.cbegin(); it!=s.cend(); it++) {
        max = qMax(max, it.key().data.size());
        count++;
    }

    QCOMPARE(count, 127*3);
    qDebug() << "Right insert strategy: LSequence tree height is" << max;
    QVERIFY(max <= 15);

    QLSeq s2;

    max = 0;
    for(int i = 0; i < 127*3; i++) {
        s2.insertAfter(-1, "Q");
    }

    for(auto it = s2.cbegin(); it!=s2.cend(); it++) {
        max = qMax(max, it.key().data.size());
    }
    qDebug() << "Left insert strategy: LSequence tree height is" << max;
    QVERIFY(max <= 15);

    QLSeq s3;

    max = 0;
    std::default_random_engine generator;
    for(int i = 0; i < 127*3; i++) {
        std::uniform_int_distribution<int> distribution(-1, s3.size()-1);
        s3.insertAfter(distribution(generator), "Q");
    }

    for(auto it = s2.cbegin(); it!=s2.cend(); it++) {
        max = qMax(max, it.key().data.size());
    }
    qDebug() << "Random insert strategy: LSequence tree height is" << max;
    QVERIFY(max <= 15);

}

void TestLSequence::nullTerminatedSymbol() {
    QLSeq s;

    for(int i = 0; i < 127*3; i++) {
        s.insertAfter(s.lastKey(), "Q");
    }
    QCOMPARE(s.size(), 127*3);

    for(auto i = s.cbegin(); i != s.cend(); i++) {
        QVERIFY(!i.key().data.contains(0));
    }
}