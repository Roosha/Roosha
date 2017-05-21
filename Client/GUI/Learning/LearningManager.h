//
// Created by gylo on 02.04.17.
//

#ifndef ROOSHA_CLIENT_LEARNINGMANAGER_H
#define ROOSHA_CLIENT_LEARNINGMANAGER_H

#include <QObject>
#include <QMap>
#include <QWeakPointer>

class LearningStrategyBase;
enum class LearningStrategyType;

class LearningManager : public QObject {
    Q_OBJECT
 public:
    LearningManager(QObject *parent = Q_NULLPTR);

    QSharedPointer<LearningStrategyBase> loadStrategy(const LearningStrategyType &strategyType);
    void saveStrategy(QSharedPointer<LearningStrategyBase> strategy);
};

#endif //ROOSHA_CLIENT_LEARNINGMANAGER_H
