//
// Created by gylo on 05.02.17.
//

#include "LearningStrategy.h"


LearningStrategy::LearningStrategy(QObject *parent) :
        QObject(parent) {
}


QObject *LearningStrategy::nextCard() {
    auto cards = World::Instance().getCards();
    auto cardId = cards.keys()[qrand() % cards.size()];

    return new CardLearningModel(cards[cardId].data(), CardLearningModel::PLAIN, CardLearningModel::TEXT_INPUT);
}
