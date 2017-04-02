//
// Created by gylo on 04.02.17.
//

#include <QQmlContext>
#include <Core/world.h>

#include "LearningStrategy.h"
#include "LearningWindowController.h"

LearningWindowController::LearningWindowController(QObject *parent) :
        QObject(parent),
        strategyType_(LearningStrategyType::SIMPLE_DIFF_STRATEGY),
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

LearningManager::LearningManager() :
        QObject(Q_NULLPTR) {
    strategies_[LearningStrategyType::SIMPLE_DIFF_STRATEGY] = QWeakPointer<SimpleDiffStrategy>();
}

QSharedPointer<LearningStrategyBase> LearningManager::loadStrategy(const LearningStrategyType &strategyType) {
    QSharedPointer<LearningStrategyBase> strategy = strategies_[strategyType].toStrongRef();
    if (strategy.isNull()) {
        strategy = QSharedPointer<LearningStrategyBase>(loadStrategyForType(strategyType));
    }

    auto changes = World::Instance().getChanges();
    for (int i = strategy->getChangesNumber(); i < changes.size(); i++) {
        const auto protoChange = changes[i]->toProtobuf();
        if (protoChange.change_case() == protoChange.kCardChange) {
            const auto cardChange = protoChange.cardchange();
            if (cardChange.change_case() == cardChange.kCreateCard) {
                strategy->onCardCreated(((CreateCard *) changes[i].data())->getCardId());
            } else if (cardChange.change_case() == cardChange.kDeleteCard) {
                strategy->onCardDeleted(((DeleteCard *) changes[i].data())->getCardId());
            }
        }
    }
    strategy->setChangesNumber((quint32) changes.size());

    const auto scrutinies = World::Instance().getLearningHistory();
    strategy->appendScrutinies(scrutinies.mid(strategy->getScrutiniesNumber()).toList());

    return strategies_[strategyType] = strategy;
}

void LearningManager::saveStrategy(QSharedPointer<LearningStrategyBase> strategy) {
    // TODO: implement
}

LearningStrategyBase *LearningManager::loadStrategyForType(const LearningStrategyType &strategyType) {
    switch (strategyType) {
        case LearningStrategyType::SIMPLE_DIFF_STRATEGY:
            return new SimpleDiffStrategy(
                    World::Instance().getCards().keys(),
                    World::Instance().getLearningHistory().toList()
            );
    }
}
