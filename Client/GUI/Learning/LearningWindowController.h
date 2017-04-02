//
// Created by gylo on 04.02.17.
//

#ifndef ROOSHA_CLIENT_LEARNINGMANAGER_H
#define ROOSHA_CLIENT_LEARNINGMANAGER_H

#include <QObject>
#include <GUI/QmlWidget.h>
#include <QtCore/QPointer>

#define LEARNING_QML_URI "roosha.learning"

class LearningManager;
class LearningStrategyBase;
enum class LearningStrategyType;

class LearningWindowController : public QObject {
 Q_OBJECT
 public:
    static void registerQmlTypes();

    LearningWindowController(QObject *parent = Q_NULLPTR);

    void showLearningWindow();
    Q_INVOKABLE void closeLearningWindow();
 private:
    LearningStrategyType strategyType_;
    QPointer<LearningManager> learningManager_;
    QSharedPointer<LearningStrategyBase> strategy_;

    QmlWidget *window_;
};


// TODO: this class should be moved to a separate file, but, unfortunately, all my attempts to do this failed due to
// some crappy compilation errors.
class LearningManager : public QObject {
 Q_OBJECT
 public:
    LearningManager();

    QSharedPointer<LearningStrategyBase> loadStrategy(const LearningStrategyType &strategyType);
    void saveStrategy(QSharedPointer<LearningStrategyBase> strategy);

 private:
    LearningStrategyBase *loadStrategyForType(const LearningStrategyType &strategyType);

    /// This map contains elements for each defined strategy type. Even so, it may contain null pointer since some
    /// strategies may be not loaded at the moment.
    QMap<LearningStrategyType, QWeakPointer<LearningStrategyBase>> strategies_;
};


#endif //ROOSHA_CLIENT_LEARNINGMANAGER_H
