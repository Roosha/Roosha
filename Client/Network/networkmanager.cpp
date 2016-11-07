#include "Network/networkmanager.h"
#include <QList>
#include <QSharedPointer>
#include <QMessageBox>
#include <QThread>

TestNetworkManager::TestNetworkManager(QObject *parent) : QObject(parent) {
}

TestNetworkManager::~TestNetworkManager() {
}

void TestNetworkManager::requestTranslations(QString source) {
    // TODO: here must be a real request to server
    TestTranslations testData;

    QSharedPointer<Translation> a(new Translation);
    a->setSource(source);
    a->setExamples(QStringList({"Fish", source, "Words", "A"}));
    a->setTarget(QStringList({"I love fosh text", source, "And I hope, that fish text love me"}));

    QSharedPointer<Translation> b(new Translation);
    b->setSource(source);
    b->setExamples(QStringList({"Fish", source, "Words", "B"}));
    b->setTarget(QStringList({"I love fosh target", "And I hope, that fish text love me"}));

    QSharedPointer<Translation> c(new Translation);
    c->setSource(source);
    c->setExamples(QStringList({"Fish", source, "Words", "C"}));
    c->setTarget(QStringList({"I love fosh text", "And I hope, that fish text love me"}));

    testData.push_back(a);
    testData.push_back(b);
    testData.push_back(c);

    QThread::currentThread()->msleep(500);

    emit newTranslation(testData);
}
