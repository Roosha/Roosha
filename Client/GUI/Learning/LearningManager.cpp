//
// Created by gylo on 02.04.17.
//

#include <iostream>
#include "LearningManager.h"
#include "LearningStrategy.h"


LearningManager::LearningManager(QObject *parent) : QObject(parent) {}

QSharedPointer<LearningStrategyBase> LearningManager::loadStrategy(const LearningStrategyType &strategyType) {
    QSharedPointer<LearningStrategyBase> strategy;
    switch (strategyType) {
        case LearningStrategyType::SIMPLE_DIFF:
            strategy = QSharedPointer<SimpleDiffStrategy>::create();
            break;
        case LearningStrategyType::SUPERMEMO_2:
            strategy = QSharedPointer<SuperMemo2Strategy>::create();
            break;
    }

    for (auto &&change :World::Instance().getChanges()) {
        roosha::Change rawChange = change->toProtobuf();
        if (change->toProtobuf().change_case() == roosha::Change::kCardChange) {
            CardChange *cardChange = (CardChange *)change.data();
            roosha::CardChange rawCardChange = rawChange.cardchange();
            if (rawCardChange.change_case() == roosha::CardChange::kCreateCard) {
                std::cout << "Create card" << rawCardChange.cardid() << std::endl;
                strategy->onCardCreated(cardChange->getCardId());
            } else if (rawCardChange.change_case() == roosha::CardChange::kDeleteCard) {
                std::cout << "Delete card" << rawCardChange.cardid() << std::endl;
                strategy->onCardDeleted(cardChange->getCardId());
            }
        } else if (change->toProtobuf().change_case() == roosha::Change::kScrutiny) {
            std::cout << "Append scrutiny" << ((Scrutiny*) change.data())->getCardId().toString().toStdString() << std::endl;
            strategy->appendScrutiny(*((Scrutiny*) change.data()));
        }
    }
    return strategy;
}

void LearningManager::saveStrategy(QSharedPointer<LearningStrategyBase> strategy) {
    // nop
}
