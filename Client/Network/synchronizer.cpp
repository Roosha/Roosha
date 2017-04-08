//
// Created by anna on 04.04.17.
//

#include "synchronizer.h"

Synchronizer::Synchronizer(QObject *parent, NetworkManager * nm) :
        QObject(parent), networkManager_(nm), synchronized_prefix_length(0) {
    qDebug("Synchronizer created");
    connect(networkManager_, &NetworkManager::successLoadChanges,
           this, &Synchronizer::receivedChanges,
           Qt::QueuedConnection);
    connect(networkManager_, &NetworkManager::successSaveChanges,
           this, &Synchronizer::pullSucceeded,
           Qt::QueuedConnection);
    connect(networkManager_, &NetworkManager::failSaveChanges,
           this, &Synchronizer::pullFailed,
           Qt::QueuedConnection);

}
Synchronizer::~Synchronizer() {
    qDebug("Synchronizer destroyed");
}
void Synchronizer::pullSucceeded(qint32 requestId) {
    synchronizing.append(suffix);
    synchronized_prefix_length = synchronizing.length();
    suffix.clear();
    emit finishSynchronization(synchronizing);
}
void Synchronizer::pullFailed(qint32 requestId, RPCErrorStatus status) {
    synchronize(synchronizing);
}
void Synchronizer::receivedChanges(qint32 requestId, ChangeList recivedChanges) {
    suffix.clear();
    for (int i = synchronized_prefix_length; i < synchronizing.length(); i++) {
        for (int j = 0; j < recivedChanges.length(); j++) {
            CMP changes_are = recivedChanges[j]->compare(synchronizing[i]);
            if (changes_are == DIFFER) {
                suffix.append(recivedChanges[j]);
            } else if (changes_are == CONFLICT) {
                // TODO: alert window for choosing the one. Now server's option will be chosen(writen after conflicting client)
                suffix.append(recivedChanges[j]);
            }
        }
    }
    networkManager_->saveChanges(suffix, synchronized_prefix_length);
}

qint32 Synchronizer::synchronize(ChangeList fullClientHistory) {
    qDebug("Synchronization started");
    synchronizing = fullClientHistory;
    networkManager_->loadChanges(synchronized_prefix_length);
}