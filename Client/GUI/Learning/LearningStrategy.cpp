//
// Created by gylo on 05.02.17.
//

#include <QDebug>

#include "LearningStrategy.h"

Card *CardLearningModel::getCard() {
    return card_;
}

CardLearningModel *CardLearningModel::emptyCard() {
    return new CardLearningModel();
}

CardViewModelBase *CardLearningModel::getViewModel() {
    return viewModel_;
}

UserInputModelBase *CardLearningModel::getInputModel() {
    return inputModel_;
}

CardLearningModel::CardLearningModel(Card *card_,
                                     CardViewModelBase *viewModel_,
                                     UserInputModelBase *inputModel_,
                                     QObject *parent) :
        QObject(parent),
        empty_(false),
        card_(card_),
        viewModel_(viewModel_),
        inputModel_(inputModel_) {
}

CardLearningModel::CardLearningModel() :
        QObject(Q_NULLPTR),
        empty_(true),
        card_(Q_NULLPTR),
        viewModel_(Q_NULLPTR),
        inputModel_(Q_NULLPTR) {
}

bool CardLearningModel::isEmpty() {
    return empty_;
}

LearningStrategyBase::LearningStrategyBase(QObject *parent) :
        QObject(parent),
        lastCardShown_(Q_NULLPTR) {
}

CardLearningModel *LearningStrategyBase::currentCard() {
    return lastCardShown_;
}

// ---------------------Random card strategy

RandomCardStrategy::RandomCardStrategy(QObject *parent) :
        LearningStrategyBase(parent) {
}

CardLearningModel *RandomCardStrategy::firstCard() {
    if (lastCardShown_) { throw std::logic_error("RandomCardStrategy::firstCard called while lastCardShown is not null"); }

    auto card = randomCard();
    return (!card) ?
            lastCardShown_ = CardLearningModel::emptyCard() :
            lastCardShown_ = new CardLearningModel(card, new TargetsAndExampleViewModel, new TextUserInputModel);
}

CardLearningModel *RandomCardStrategy::nextCard(CardDifficulty::Rate previousRate) {
    if (!lastCardShown_) { throw std::logic_error("RandomCardStrategy::nextCard called while lastCardShown is null"); }

    delete lastCardShown_;

    auto card = randomCard();
    return (!card) ?
            lastCardShown_ = CardLearningModel::emptyCard() :
            lastCardShown_ = new CardLearningModel(card, new TargetsAndExampleViewModel, new TextUserInputModel);
}

void RandomCardStrategy::finish(CardDifficulty::Rate previousRate) {
    if (!lastCardShown_) { throw std::logic_error("RandomCardStrategy::nextCard called while lastCardShown is null"); }

    delete lastCardShown_;
    lastCardShown_ = Q_NULLPTR;
}

void RandomCardStrategy::cancel() {
    if (lastCardShown_) { delete lastCardShown_; }
    lastCardShown_ = Q_NULLPTR;
}

DBCard *RandomCardStrategy::randomCard() {
    auto cards = World::Instance().getCards().values();
    if (cards.size() == 0) { return Q_NULLPTR; }
    return cards[qrand() % cards.size()].data();
}

// ---------------------/RandomCardStrategy
