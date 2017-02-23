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
        difficultyRate_(CardDifficulty::Rate::UNKNOWN),
        empty_(false),
        card_(card_),
        viewModel_(viewModel_),
        inputModel_(inputModel_) {
}

CardLearningModel::CardLearningModel() :
        QObject(Q_NULLPTR),
        difficultyRate_(CardDifficulty::Rate::UNKNOWN),
        empty_(true),
        card_(Q_NULLPTR),
        viewModel_(Q_NULLPTR),
        inputModel_(Q_NULLPTR) {
}

bool CardLearningModel::isEmpty() const {
    return empty_;
}

CardDifficulty::Rate CardLearningModel::getCardDifficultyRate() const {
    return difficultyRate_;
}

LearningStrategyBase::LearningStrategyBase(QObject *parent) :
        QObject(parent),
        lastCardShown_(Q_NULLPTR) {
}

CardLearningModel *LearningStrategyBase::currentCard() {
    return lastCardShown_;
}

Scrutiny LearningStrategyBase::currentScrunity() throw(std::logic_error) {
    if (!lastCardShown_) {
        throw std::logic_error("LearningStrategyBase::currentScrunity called while lastCardShown is null");
    }
    if (lastCardShown_->getCardDifficultyRate() == CardDifficulty::Rate::UNKNOWN) {
        throw std::logic_error("LearningStrategyBase::currentScrunity called "
                                       "while lastCardShown.cardDifficulty is unknown");
    }

    return Scrutiny(
            lastCardShown_->getCard()->getId(),
            QDateTime::currentDateTime(),
            lastCardShown_->getInputModel()->getType(),
            lastCardShown_->getViewModel()->getType(),
            lastCardShown_->getCardDifficultyRate()
    );
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

CardLearningModel *RandomCardStrategy::nextCard() {
    Scrutiny lastScrutiny = currentScrunity();
    World::Instance().addScrutiny(lastScrutiny);

    delete lastCardShown_;

    auto card = randomCard();
    return (!card) ?
            lastCardShown_ = CardLearningModel::emptyCard() :
            lastCardShown_ = new CardLearningModel(card, new TargetsAndExampleViewModel, new TextUserInputModel);
}

void RandomCardStrategy::finish() {
    Scrutiny lastScrutiny = currentScrunity();
    World::Instance().addScrutiny(lastScrutiny);

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

// ---------------------SimpleDiffStrategy

SimpleDiffStrategy::SimpleDiffStrategy(QObject *parent) :
        LearningStrategyBase(parent) {
    for (auto &&cardId : World::Instance().getCards().keys()) {
        diffs_[cardId] = 0;
    }

    auto history = World::Instance().getLearningHistory();
    for (auto &&scrutiny : history) {
        diffs_[scrutiny.getCardId()] += getDiffOfDifficultyRate(scrutiny.getDifficultyRate());
    }
    for (auto &&id : diffs_.keys()) {
        cardQueue_.emplace(id, diffs_[id]);
    }
}

CardLearningModel *SimpleDiffStrategy::firstCard() {
    if (lastCardShown_) { throw std::logic_error("SimpleDiffStrategy::firstCard called while lastCardShown is not null"); }

    return lastCardShown_ = nextCardLearningModel();
}

CardLearningModel *SimpleDiffStrategy::nextCard() {
    processLastScrutiny();

    return lastCardShown_ = nextCardLearningModel();
}

void SimpleDiffStrategy::finish() {
    processLastScrutiny();

    lastCardShown_ = Q_NULLPTR;
}

void SimpleDiffStrategy::cancel() {
    if (lastCardShown_->isEmpty()) { return; }

    QUuid prevCardId = lastCardShown_->getCard()->getId();
    cardQueue_.emplace(prevCardId, diffs_[prevCardId]);

    lastCardShown_ = Q_NULLPTR;
}

qint32 SimpleDiffStrategy::getDiffOfDifficultyRate(CardDifficulty::Rate rate) {
    switch (rate) {
        case CardDifficulty::Rate::UNKNOWN:
            throw std::logic_error("SimpleDiffStrategy::getDiffOfDifficultyRate: unexpected rate UNKNOWN");
        case CardDifficulty::Rate::SKIPPED: return -1;
        case CardDifficulty::Rate::FAILED: return -2;
        case CardDifficulty::Rate::EASY: return +3;
        case CardDifficulty::Rate::NORMAL: return +2;
        case CardDifficulty::Rate::DIFFICULT: return +1;
    }
}

CardLearningModel *SimpleDiffStrategy::nextCardLearningModel() {
    if (cardQueue_.empty()) { return CardLearningModel::emptyCard(); }
    QUuid cardId = cardQueue_.top().first;
    cardQueue_.pop();
    return lastCardShown_ = new CardLearningModel(
            World::Instance().getCards()[cardId].data(),
            new TargetsAndExampleViewModel(),
            new TextUserInputModel()
    );
}

void SimpleDiffStrategy::processLastScrutiny() {
    if (!lastCardShown_->isEmpty()) {
        World::Instance().addScrutiny(currentScrunity());

        QUuid prevCardId = lastCardShown_->getCard()->getId();
        diffs_[prevCardId] += getDiffOfDifficultyRate(lastCardShown_->getCardDifficultyRate());
        cardQueue_.emplace(prevCardId, diffs_[prevCardId]);
    }
}
