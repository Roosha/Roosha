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
    LearningManager();

    QSharedPointer<LearningStrategyBase> loadStrategy(const LearningStrategyType &strategyType);
    void saveStrategy(QSharedPointer<LearningStrategyBase> strategy);

 private:
    LearningStrategyBase *loadStrategyForType(const LearningStrategyType &strategyType);

    QMap<LearningStrategyType, QWeakPointer<LearningStrategyBase>> strategies_;
};

#endif //ROOSHA_CLIENT_LEARNINGMANAGER_H
