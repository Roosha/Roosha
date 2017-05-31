//
// Created by anna on 27.05.17.
//

#ifndef ROOSHA_CLIENT_TESTNET_H
#define ROOSHA_CLIENT_TESTNET_H


#include <QtCore/QObject>

class TestNet: public QObject {
Q_OBJECT
private slots:
    void exec_my_tests();
};


#endif //ROOSHA_CLIENT_TESTNET_H
