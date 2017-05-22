//
// Created by gylo on 04.02.17.
//

#include <QQmlContext>
#include <Core/world.h>

#include "LearningWindowController.h"
#include "LearningStrategy.h"
#include "LearningManager.h"

LearningWindowController::LearningWindowController(QObject *parent) :
        QObject(parent),
        strategyType_(LearningStrategyType::SUPERMEMO_2),
        learningManager_(QPointer<LearningManager>(new LearningManager())),
        window_(Q_NULLPTR) {
    strategy_ = learningManager_->loadStrategy(strategyType_);
}

void LearningWindowController::registerQmlTypes() {
    static bool registered = false;
    if (!registered) {
        qmlRegisterType<CardDifficulty>(LEARNING_QML_URI, 1, 0, "Difficulty");
        qRegisterMetaType<CardDifficulty::Rate>();

        qmlRegisterType<CardLearningModel>(LEARNING_QML_URI, 1, 0, "CardLearningModel");

        registered = true;
    }

}

void LearningWindowController::showLearningWindow() {
    if (window_) { closeLearningWindow(); }

    strategy_ = learningManager_->loadStrategy(strategyType_);

    window_ = new QmlWidget();
    connect(window_, &QmlWidget::closed, this, [this] {
        window_ = Q_NULLPTR;
        closeLearningWindow();
    });

    window_->rootContext()->setContextProperty("controller", this);
    window_->rootContext()->setContextProperty("strategy", strategy_.data());
    window_->setSource(QStringLiteral("qrc:/learning/LearningWindow.qml"));

    window_->setResizeMode(QQuickWidget::SizeRootObjectToView);
    window_->setAttribute(Qt::WA_DeleteOnClose);
    window_->show();
}

void LearningWindowController::closeLearningWindow() {
    qDebug() << "closeLearningWindow called";
    if (window_) {
        window_->close();
    }
    window_ = Q_NULLPTR;

    strategy_->cancel();
    learningManager_->saveStrategy(strategy_);
}

