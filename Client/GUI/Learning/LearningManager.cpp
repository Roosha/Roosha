//
// Created by gylo on 02.04.17.
//

#include "LearningManager.h"
#include "LearningStrategy.h"

LearningManager::LearningManager() :
        QObject(Q_NULLPTR) {
    strategies_[LearningStrategyType::SIMPLE_DIFF] = QWeakPointer<SimpleDiffStrategy>();
    strategies_[LearningStrategyType::SUPERMEMO_2] = QWeakPointer<SuperMemo2Strategy>();
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
    // save to disk
}

LearningStrategyBase *LearningManager::loadStrategyForType(const LearningStrategyType &strategyType) {
    switch (strategyType) {
        case LearningStrategyType::SIMPLE_DIFF:
            return new SimpleDiffStrategy(
                    (quint32) World::Instance().getChanges().size(),
                    World::Instance().getCards().keys(),
                    World::Instance().getLearningHistory().toList()
            );
        case LearningStrategyType::SUPERMEMO_2:
            return new SuperMemo2Strategy(
                    (quint32) World::Instance().getChanges().size(),
                    World::Instance().getCards().keys(),
                    World::Instance().getLearningHistory().toList()
            );
    }
}