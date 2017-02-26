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
        scrutiniesNumber_(0),
        lastCardShown_(Q_NULLPTR) {
}

CardLearningModel *LearningStrategyBase::currentCard() {
    return lastCardShown_;
}

Scrutiny LearningStrategyBase::currentScrutiny() throw(std::logic_error) {
    if (!lastCardShown_) {
        throw std::logic_error("LearningStrategyBase::currentScrutiny called while lastCardShown is null");
    }
    if (lastCardShown_->getCardDifficultyRate() == CardDifficulty::Rate::UNKNOWN) {
        throw std::logic_error("LearningStrategyBase::currentScrutiny called "
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

void LearningStrategyBase::onCardCreated(QUuid cardId) {
    QSet<QUuid> set{cardId};
    this->onCardsCreated(set);
}

void LearningStrategyBase::onCardDeleted(QUuid cardId) {
    QSet<QUuid> set{cardId};
    this->onCardsDeleted(set);
}

// ---------------------SimpleDiffStrategy

SimpleDiffStrategy::SimpleDiffStrategy(QList<QUuid> cardIds, QList<Scrutiny> scrutinies, QObject *parent) :
        LearningStrategyBase(parent) {
    this->onCardsCreated(QSet<QUuid>::fromList(cardIds));
    this->appendScrutinies(scrutinies);
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
    if (!lastCardShown_) { throw std::logic_error("SimpleDiffStrategy::cancel called while lastCardShown is null"); }

    if (!lastCardShown_->isEmpty()) {
        QUuid prevCardId = lastCardShown_->getCard()->getId();
        if (diffs_.contains(prevCardId)) { cardQueue_.emplace(prevCardId, diffs_[prevCardId]); }
    }

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
    while (!cardQueue_.empty()) {
        QUuid cardId = cardQueue_.top().first;
        cardQueue_.pop();
        if (diffs_.contains(cardId)) {
            return lastCardShown_ = new CardLearningModel(
                    World::Instance().getCards()[cardId].data(),
                    new TargetsAndExampleViewModel(),
                    new TextUserInputModel()
            );
        }
    }

    return CardLearningModel::emptyCard();
}

void SimpleDiffStrategy::processLastScrutiny() {
    if (!lastCardShown_->isEmpty()) {
        World::Instance().addScrutiny(currentScrutiny());

        QUuid prevCardId = lastCardShown_->getCard()->getId();
        diffs_[prevCardId] += getDiffOfDifficultyRate(lastCardShown_->getCardDifficultyRate());
        if (diffs_.contains(prevCardId)) { cardQueue_.emplace(prevCardId, diffs_[prevCardId]); }
    }
}

void SimpleDiffStrategy::onCardsCreated(QSet<QUuid> cardIds) {
    for (auto &&cardId : cardIds) {
        diffs_[cardId] = 0;
        cardQueue_.emplace(cardId, 0);
    }
}

void SimpleDiffStrategy::onCardsDeleted(QSet<QUuid> cardIds) {
    for (auto &&cardId : cardIds) {
        diffs_.remove(cardId);
    }
}

void SimpleDiffStrategy::appendScrutinies(QList<Scrutiny> scrutinies) {
    scrutiniesNumber_ += scrutinies.size();
    for (auto &&scrutiny : scrutinies) {
        diffs_[scrutiny.getCardId()] += getDiffOfDifficultyRate(scrutiny.getDifficultyRate());
    }
}
