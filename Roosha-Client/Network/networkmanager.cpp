#include "Network/networkmanager.h"
#include <QList>
#include <QSharedPointer>
#include <QMessageBox>
#include <QThread>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
}

NetworkManager::~NetworkManager() {
}

void NetworkManager::requestTranslations(QString source) {
    // TODO: here must be a real request to server
    Translations testData;

    QSharedPointer<Translation> a(new Translation);
    a->setSource(source);
    a->setExamples(QStringList({"Fish", source, "Words"}));
    a->setTarget(QStringList({"I love fosh text", "And I hope, that fish text love me"}));

    QSharedPointer<Translation> b(new Translation);
    b->setSource(source);
    b->setExamples(QStringList({"Fish", source, "Words"}));
    b->setTarget(QStringList({"I love fosh text", "And I hope, that fish text love me"}));

    QSharedPointer<Translation> c(new Translation);
    c->setSource(source);
    c->setExamples(QStringList({"Fish", source, "Words"}));
    c->setTarget(QStringList({"I love fosh text", "And I hope, that fish text love me"}));

    testData.push_back(a);
    testData.push_back(b);
    testData.push_back(c);

    QThread::currentThread()->msleep(500);

    emit newTranslation(testData);
}
