//
// Created by gylo on 05.02.17.
//

#include <QDebug>

#include "LearningStrategy.h"

Card *CardLearningModel::getCard() {
    return card_.data();
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

CardLearningModel::CardLearningModel(QSharedPointer<Card> card_,
                                     QPointer<CardViewModelBase> viewModel_,
                                     QPointer<UserInputModelBase> inputModel_,
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
        lastShownCard_(Q_NULLPTR) {
}

CardLearningModel *LearningStrategyBase::currentCard() {
    return lastShownCard_;
}

Scrutiny LearningStrategyBase::currentScrutiny() throw(std::logic_error) {
    if (!lastShownCard_) {
        throw std::logic_error("LearningStrategyBase::currentScrutiny called while lastCardShown is null");
    }
    if (lastShownCard_->getCardDifficultyRate() == CardDifficulty::Rate::UNKNOWN) {
        throw std::logic_error("LearningStrategyBase::currentScrutiny called "
                                       "while lastCardShown.cardDifficulty is unknown");
    }

    return Scrutiny(
            lastShownCard_->getCard()->getId(),
            QDateTime::currentDateTime(),
            lastShownCard_->getInputModel()->getType(),
            lastShownCard_->getViewModel()->getType(),
            lastShownCard_->getCardDifficultyRate()
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

void LearningStrategyBase::appendScrutiny(Scrutiny scrutiny) {
    QList<Scrutiny> list{scrutiny};
    this->appendScrutinies(list);
}

// --------------------- SimpleDiffStrategy --------------------------------

SimpleDiffStrategy::SimpleDiffStrategy(QObject *parent) : LearningStrategyBase(parent) {}

CardLearningModel *SimpleDiffStrategy::firstCard() {
    if (lastShownCard_) { throw std::logic_error("SimpleDiffStrategy::firstCard called while lastCardShown is not null"); }

    return lastShownCard_ = nextCardLearningModel();
}

CardLearningModel *SimpleDiffStrategy::nextCard() {
    processLastScrutiny();

    return lastShownCard_ = nextCardLearningModel();
}

void SimpleDiffStrategy::finish() {
    processLastScrutiny();

    lastShownCard_ = Q_NULLPTR;
}

void SimpleDiffStrategy::cancel() {
    if (lastShownCard_ && !lastShownCard_->isEmpty()) {
        QUuid prevCardId = lastShownCard_->getCard()->getId();
        if (diffs_.contains(prevCardId)) { cardQueue_.emplace(prevCardId, diffs_[prevCardId]); }
    }

    lastShownCard_ = Q_NULLPTR;
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
            return lastShownCard_ = new CardLearningModel(
                    World::Instance().getCards()[cardId],
                    QPointer<CardViewModelBase>(new TargetsAndExampleViewModel()),
                    QPointer<UserInputModelBase>(new TextUserInputModel())
            );
        }
    }

    return CardLearningModel::emptyCard();
}

void SimpleDiffStrategy::processLastScrutiny() {
    if (!lastShownCard_->isEmpty()) {
        World::Instance().addScrutiny(currentScrutiny());

        QUuid prevCardId = lastShownCard_->getCard()->getId();
        diffs_[prevCardId] += getDiffOfDifficultyRate(lastShownCard_->getCardDifficultyRate());
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

SuperMemo2Strategy::SuperMemo2Strategy(QObject *parent,
                                       double difficultFactor,
                                       double normalFactor,
                                       double easyFactor,
                                       quint32 intervalMax) :
        LearningStrategyBase(parent),
        lastShownCardInfo_(Q_NULLPTR),
        difficultFactor_(difficultFactor),
        normalFactor_(normalFactor),
        easyFactor_(easyFactor),
        intervalMaximum_(intervalMax) {}

CardLearningModel *SuperMemo2Strategy::firstCard() {
    if (lastShownCard_) { throw std::logic_error("SuperMemo2Strategy::firstCard called while lastShownCard is not null"); }
    if (lastShownCardInfo_) {
        throw std::logic_error("SuperMemo2Strategy::firstCard called while lastShownCardInfo is not null");
    }

    lastShownCardInfo_ = nextCardInfo();
    return lastShownCard_ = learningModelForCard(lastShownCardInfo_);
}

CardLearningModel *SuperMemo2Strategy::nextCard() {
    if (!lastShownCard_) { throw std::logic_error("SuperMemo2Strategy::nextCard called while lastShownCard is null."); }
    if (!lastShownCard_->isEmpty()) {
        World::Instance().addScrutiny(currentScrutiny());
    }

    if (lastShownCardInfo_) {
        cardQueue_.push(alteredCardInfo(*lastShownCardInfo_, lastShownCard_->getCardDifficultyRate()));
    }

    lastShownCardInfo_ = nextCardInfo();
    return lastShownCard_ = learningModelForCard(lastShownCardInfo_);
}

void SuperMemo2Strategy::finish() {
    if (!lastShownCard_) { throw std::logic_error("SuperMemo2Strategy::finish called while lastShownCard is null."); }
    if (!lastShownCard_->isEmpty()) {
        World::Instance().addScrutiny(currentScrutiny());
    }

    if (lastShownCardInfo_) {
        auto newCardInfo = alteredCardInfo(*lastShownCardInfo_, lastShownCard_->getCardDifficultyRate());
        cardQueue_.push(alteredCardInfo(newCardInfo, lastShownCard_->getCardDifficultyRate()));
    }

    lastShownCard_ = Q_NULLPTR;
    lastShownCardInfo_ = Q_NULLPTR;
}

void SuperMemo2Strategy::cancel() {
    if (lastShownCard_ && lastShownCardInfo_) {
        cardQueue_.push(*lastShownCardInfo_);
    }

    lastShownCard_ = Q_NULLPTR;
    lastShownCardInfo_ = Q_NULLPTR;
}

LearningStrategyType SuperMemo2Strategy::getType() {
    return LearningStrategyType::SUPERMEMO_2;
}

void SuperMemo2Strategy::onCardsCreated(QSet<QUuid> cardIds) {
    auto currentTime = QDateTime::currentDateTime();
    auto cardsNum = cardIds.size();
    for (auto &&cardId : cardIds) {
        auto delta = rand() % (cardsNum + 1) - (cardsNum / 2); // to shuffle cards a little bit
        cardQueue_.emplace(cardId, currentTime.addMSecs(delta));
    }
}

void SuperMemo2Strategy::onCardsDeleted(QSet<QUuid> cardIds) {
    // do nothing since we check all cards from cardQueue
}

void SuperMemo2Strategy::appendScrutinies(QList<Scrutiny> scrutinies) {
    QMap<QUuid, QVector<Scrutiny>> map;
    for (auto currentScrutiny : scrutinies) {
        if (!map.contains(currentScrutiny.getCardId())) {
            map[currentScrutiny.getCardId()] = QVector<Scrutiny>();
        }
        map[currentScrutiny.getCardId()].append(currentScrutiny);
    }

    QVector<CardInfo> newInfos((int) cardQueue_.size());
    while (!cardQueue_.empty()) {
        CardInfo info = cardQueue_.top();
        for (auto &&scrutiny : map.value(info.cardId_, QVector<Scrutiny>())) {
            info = alteredCardInfo(info, scrutiny.getDifficultyRate());
        }
        newInfos.append(info);
        cardQueue_.pop();
    }

    cardQueue_ = std::priority_queue<CardInfo, std::vector<CardInfo>, Comparator>();
    for (auto &&newInfo : newInfos) {
        cardQueue_.push(newInfo);
    }
}

SuperMemo2Strategy::CardInfo SuperMemo2Strategy::alteredCardInfo(const SuperMemo2Strategy::CardInfo &cardInfo,
                                                                 CardDifficulty::Rate scrutinyStatus) {
    // see https://www.supermemo.com/english/ol/sm2.htm for details
    const quint32 ord = CardDifficulty::ord(scrutinyStatus);
    double factor = std::max(1.3, cardInfo.currentFactor_ + (0.1 - (5 - ord) * (0.08 + (5 - ord) * 0.02)));

    QDateTime nextScrutiny = QDateTime::currentDateTime();
    quint32 interval;
    switch (scrutinyStatus) {
        case CardDifficulty::Rate::UNKNOWN:
            throw std::logic_error("Unexpected UNKNOWN difficulty rate in SuperMemo2Strategy::alteredCardInfo");
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

CardLearningModel *SuperMemo2Strategy::learningModelForCard(const std::unique_ptr<SuperMemo2Strategy::CardInfo> &cardInfo) {
    if (cardInfo) {
        return new CardLearningModel(
                World::Instance().getCards()[cardInfo->cardId_],
                QPointer<CardViewModelBase>(new TargetsAndExampleViewModel()),
                QPointer<UserInputModelBase>(new TextUserInputModel())
        );
    } else {
        return CardLearningModel::emptyCard();
    }
}

std::unique_ptr<SuperMemo2Strategy::CardInfo> SuperMemo2Strategy::nextCardInfo() {
    QDateTime now = QDateTime::currentDateTime().addSecs(TEN_MINUTES_IN_SECONDS);
    while (!cardQueue_.empty()) {
        CardInfo info = cardQueue_.top();
        if (World::Instance().getCards().contains(info.cardId_)) {
            if (info.nextScrutiny_ <= now) {
                CardInfo *result = new CardInfo(info);
                cardQueue_.pop();
                return std::unique_ptr<SuperMemo2Strategy::CardInfo>(result);
            } else {
                return Q_NULLPTR;
            }
        } else {
            cardQueue_.pop();
        }
    }
    return Q_NULLPTR;
}

