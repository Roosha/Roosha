//
// Created by anna on 04.04.17.
//

#ifndef ROOSHA_CLIENT_SYNCHRONIZER_H
#define ROOSHA_CLIENT_SYNCHRONIZER_H


#include <QtCore/QObject>
#include <Core/ichange.h>
#include "error_status.h"
#include "network_manager.h"

class Synchronizer: public QObject {
 Q_OBJECT

 public:
    qint32 synchronize(ChangeList fullClientHistory);
    Synchronizer(QObject *parent = Q_NULLPTR, NetworkManager * nm =Q_NULLPTR);
    ~Synchronizer();

public slots:
    void receivedChanges(qint32 requestId, ChangeList serverChanges);
    void pullSucceeded(qint32 requestId);
    void syncFailed(qint32 requestId, RPCErrorStatus status);
 signals:
    void finishSynchronization(ChangeList changes);
private:
    ChangeList clientChanges;
    ChangeList suffix;
    NetworkManager *networkManager_;
    qint32 synchronized_prefix_length;
};


#endif //ROOSHA_CLIENT_SYNCHRONIZER_H
