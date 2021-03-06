#include "Helpers/configuremanager.h"
#include <Test/Core/authTests/worldtest.h>


qint8 ConfigureManager::version = 0;


ConfigureManager::ConfigureManager() {
    testMID = 0;
}

ConfigureManager::~ConfigureManager() {
}

NetworkManager *ConfigureManager::getNetworkManager() {
    return networkManager_;
}

void ConfigureManager::setNetworkManager(NetworkManager *manager) {
    networkManager_ = manager;
}
Synchronizer *ConfigureManager::getSynchronizer() {
    return synchronizer_;
}

void ConfigureManager::setSynchronizer(Synchronizer *synchronizer){
    synchronizer_ = synchronizer;
}

QString ConfigureManager::getLogin() {
    return settings_.value("auth/login", "").toString();
}

QString ConfigureManager::getPasswordHash() {
    return settings_.value("auth/password", "").toString();
}

QString ConfigureManager::getToken() {
    return settings_.value("auth/token", "").toString();
}

void ConfigureManager::setLogin(QString login) {
    settings_.setValue("auth/login", QVariant::fromValue(login));
}

void ConfigureManager::setPasswordHash(QString password) {
    settings_.setValue("auth/password", QVariant::fromValue(password));
}

void ConfigureManager::setToken(QString token) {
    settings_.setValue("auth/token", QVariant::fromValue(token));
}

qint8 ConfigureManager::getMachineId() {
    if (WorldTest::testing > 0) {
        return testMID;
    }
    return static_cast<qint8>(settings_.value("auth/machineId", QVariant::fromValue(0)).toInt());
}

void ConfigureManager::setMachineId(qint8 machineId) {
    if (WorldTest::testing > 0 && testMID == 0) {
        testMID = machineId;
        return;
    }
    settings_.setValue("auth/machineId", QVariant::fromValue(machineId));
}

qint32 ConfigureManager::getSyncLength() {
    return settings_.value("sync/len", 0).toInt();
}

void ConfigureManager::setSyncLength(qint32 len) {
    settings_.setValue("sync/len", QVariant::fromValue(len));
}
