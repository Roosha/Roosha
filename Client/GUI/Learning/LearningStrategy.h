//
// Created by gylo on 05.02.17.
//

#ifndef ROOSHA_CLIENT_LEARNINGSTRATEGY_H
#define ROOSHA_CLIENT_LEARNINGSTRATEGY_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include "CardLearningModel.h"

class LearningStrategy : public QObject {
    Q_OBJECT
 public:
    LearningStrategy(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE QObject *nextCard();
};

#endif //ROOSHA_CLIENT_LEARNINGSTRATEGY_H
