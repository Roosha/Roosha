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

LearningStrategyBase::LearningStrategyBase(quint32 changesNumber, QObject *parent) :
        QObject(parent),
        scrutiniesNumber_(0),
        changesNumber_(changesNumber),
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

quint32 LearningStrategyBase::getScrutiniesNumber() {
    return scrutiniesNumber_;
}

quint32 LearningStrategyBase::getChangesNumber() const {
    return changesNumber_;
}

void LearningStrategyBase::setChangesNumber(quint32 changesNumber_) {
    LearningStrategyBase::changesNumber_ = changesNumber_;
}

// --------------------- SimpleDiffStrategy --------------------------------

SimpleDiffStrategy::SimpleDiffStrategy(quint32 changesNumber,
                                       QList<QUuid> cardIds,
                                       QList<Scrutiny> scrutinies,
                                       QObject *parent) :
        LearningStrategyBase(changesNumber, parent) {
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
    if (lastCardShown_ && !lastCardShown_->isEmpty()) {
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
        if (diffs_.contains(scrutiny.getCardId())) {
            diffs_[scrutiny.getCardId()] += getDiffOfDifficultyRate(scrutiny.getDifficultyRate());
        }
    }
}

LearningStrategyType SimpleDiffStrategy::getType() {
    return LearningStrategyType::SIMPLE_DIFF;
}

// ---------------------- SuperMemo2 Strategy ------------------------------------

SuperMemo2Strategy::SuperMemo2Strategy(quint32 changesNumber,
                                       QList<QUuid> cardIds,
                                       QList<Scrutiny> scrutinies,
                                       QObject *parent,
                                       double difficultFactor,
                                       double normalFactor,
                                       double easyFactor,
                                       quint32 intervalMax) :
        LearningStrategyBase(changesNumber, parent),
        difficultFactor_(difficultFactor),
        normalFactor_(normalFactor),
        easyFactor_(easyFactor),
        intervalMaximum_(intervalMax) {
    auto currentTime = QDateTime::currentDateTime();
    for (auto &&cardId : cardIds) {
        auto delta = rand() % (changesNumber + 1) - (changesNumber / 2); // to shuffle cards a little bit
        cardQueue_.emplace(cardId, currentTime.addMSecs(delta));
    }

    appendScrutinies(scrutinies);
}

CardLearningModel *SuperMemo2Strategy::firstCard() {
    // TODO: implement
}

CardLearningModel *SuperMemo2Strategy::nextCard() {
    // TODO: implement

}

void SuperMemo2Strategy::finish() {
    // TODO: implement

}

void SuperMemo2Strategy::cancel() {
    // TODO: implement
}

LearningStrategyType SuperMemo2Strategy::getType() {
    return LearningStrategyType::SUPERMEMO_2;
}

void SuperMemo2Strategy::onCardsCreated(QSet<QUuid> cardIds) {

}

void SuperMemo2Strategy::onCardsDeleted(QSet<QUuid> cardIds) {
    // TODO: implement

}

void SuperMemo2Strategy::appendScrutinies(QList<Scrutiny> scrutinies) {
    // TODO: implement

}

SuperMemo2Strategy::CardInfo SuperMemo2Strategy::nextCardInfo(const SuperMemo2Strategy::CardInfo &cardInfo,
                                                              CardDifficulty::Rate scrutinyStatus)  {
    // see https://www.supermemo.com/english/ol/sm2.htm for details
    const quint32 ord = CardDifficulty::ord(scrutinyStatus);
    double factor = std::max(1.3, cardInfo.currentFactor_ + (0.1 - (5 - ord) * (0.08 + (5 - ord) * 0.02)));

    QDateTime nextScrutiny = QDateTime::currentDateTime();
    quint32 interval;
    switch (scrutinyStatus) {
        case CardDifficulty::Rate::UNKNOWN:
            throw std::logic_error("Unexpected UNKNOWN difficulty rate in SuperMemo2Strategy::nextCardInfo");
        case CardDifficulty::Rate::SKIPPED:interval = 0;
            nextScrutiny = nextScrutiny.addSecs(TEN_MINUTES_IN_SECONDS);
            break;
        case CardDifficulty::Rate::FAILED:
            interval = std::max(1 * ONE_DAY_IN_SECONDS,
                                cardInfo.currentIntervalSeconds_ / 2);
            nextScrutiny = nextScrutiny.addSecs(TEN_MINUTES_IN_SECONDS);
            break;
        case CardDifficulty::Rate::DIFFICULT:
            interval = std::min(intervalMaximum_,
                                nextIntervalWhenSucceeded(cardInfo.currentIntervalSeconds_, factor));
            nextScrutiny = nextScrutiny.addSecs(interval);
            break;
        case CardDifficulty::Rate::NORMAL:
            interval = std::min(intervalMaximum_,
                                nextIntervalWhenSucceeded(cardInfo.currentIntervalSeconds_, factor));
            nextScrutiny = nextScrutiny.addSecs(interval);
            break;
        case CardDifficulty::Rate::EASY:
            interval = std::min(intervalMaximum_,
                                nextIntervalWhenSucceeded(cardInfo.currentIntervalSeconds_, factor));
            nextScrutiny = nextScrutiny.addSecs(interval);
            break;
    }

    return SuperMemo2Strategy::CardInfo(
            cardInfo.cardId_,
            nextScrutiny,
            interval,
            factor
    );
}

quint32 SuperMemo2Strategy::nextIntervalWhenSucceeded(quint32 currentInterval, double factor) {
    if (currentInterval < TEN_MINUTES_IN_SECONDS) {
        return TEN_MINUTES_IN_SECONDS;
    } else if (currentInterval <= TEN_MINUTES_IN_SECONDS && currentInterval < ONE_DAY_IN_SECONDS) {
        return ONE_DAY_IN_SECONDS;
    } else {
        return (quint32) (currentInterval * factor);
    }
}

