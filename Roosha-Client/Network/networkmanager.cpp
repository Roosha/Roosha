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

    testData.push_back(QSharedPointer<Translation>::create(source));
    testData.push_back(QSharedPointer<Translation>::create(source));
    testData.push_back(QSharedPointer<Translation>::create(source));

    QThread::currentThread()->msleep(500);

    emit newTranslation(testData);
}
