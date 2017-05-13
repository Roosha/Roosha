//
// Created by gylo on 04.02.17.
//

#ifndef ROOSHA_CLIENT_LEARNINGWINDOWCONTROLLER_H
#define ROOSHA_CLIENT_LEARNINGWINDOWCONTROLLER_H

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

    static LearningWindowController *getInstance() {
        if(self == nullptr) {
            self = new LearningWindowController();
        }
        return self;
    }

 private:
    LearningStrategyType strategyType_;
    QPointer<LearningManager> learningManager_;
    QSharedPointer<LearningStrategyBase> strategy_;

    QmlWidget *window_;

    static LearningWindowController *self;
};

static QObject *register_learning_window_controller(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return LearningWindowController::getInstance();
}

#endif //ROOSHA_CLIENT_LEARNINGWINDOWCONTROLLER_H
