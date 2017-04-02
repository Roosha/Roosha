//
// Created by gylo on 02.04.17.
//

#include "LearningManager.h"
#include "LearningStrategy.h"

QSharedPointer<LearningStrategyBase> LearningManager::loadStrategy(const LearningStrategyType &strategyType) {
    QSharedPointer<LearningStrategyBase> `strategy = strategies_[strategyType].toStrongRef();
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