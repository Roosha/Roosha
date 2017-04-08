#include "Helpers/configuremanager.h"

ConfigureManager::ConfigureManager() {
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
