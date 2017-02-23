//
// Created by gylo on 05.02.17.
//

#ifndef ROOSHA_CLIENT_LEARNINGSTRATEGY_H
#define ROOSHA_CLIENT_LEARNINGSTRATEGY_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtQml/qqml.h>

#include <Core/dbcard.h>
#include <Core/Scrutiny.h>
#include <queue>
#include "UserInputModels.h"
#include "CardLearningViewModels.h"
#include "CardDifficulty.h"

class CardLearningModel : public QObject {
    //@formatter:off
    Q_OBJECT
 public:
    Q_PROPERTY(Card* card READ getCard)
    Q_PROPERTY(CardViewModelBase* viewModel READ getViewModel)
    Q_PROPERTY(UserInputModelBase* inputModel READ getInputModel)
    Q_PROPERTY(bool empty READ isEmpty)
    Q_PROPERTY(CardDifficulty::Rate difficultyRate MEMBER difficultyRate_)
    //@formatter:on
 public:
    /// create instance with empty flag set.
    static CardLearningModel *emptyCard();

    /// Empty model constructor
    CardLearningModel();
    CardLearningModel(Card *card_,
                      CardViewModelBase *viewModel_,
                      UserInputModelBase *inputModel_,
                      QObject *parent = Q_NULLPTR);

    /// if this flag is set, there is no cards to learn, so nothing to show
    bool isEmpty() const;
    CardDifficulty::Rate getCardDifficultyRate() const;
    Card *getCard();
    CardViewModelBase *getViewModel();
    UserInputModelBase *getInputModel();
 private:

    bool empty_;
    CardDifficulty::Rate difficultyRate_;
    Card *card_;
    CardViewModelBase *viewModel_;
    UserInputModelBase *inputModel_;
};

class LearningStrategyBase : public QObject {
 Q_OBJECT
 public:
    LearningStrategyBase(QObject *parent);

    /**
     * This function should be called to get the first card in learning session. To get subsequent cards, use
     * nextCard method.
     * @return next card to learn.
     */
    Q_INVOKABLE virtual CardLearningModel *firstCard() = 0;

    /**
     * Get the next card to learn. This method should be called after difficulty rate of the current card is set.
     * Otherwise, std::logic_error should be thrown.
     * @note this method should always call World::addScrutiny to save result of the scrutiny
     * @return next card to learn.
     */
    Q_INVOKABLE virtual CardLearningModel *nextCard() = 0;

    /** Get current card. */
    Q_INVOKABLE CardLearningModel *currentCard();

    /**
     * Finish learning session successfully by setting difficulty of the last learned card.
     * This method should be called after difficulty rate of the current card is set.
     * Otherwise, std::logic_error should be thrown.
     * If the last card returned by firstCard or nextCard methods was not learned, use cancel method.
     * @note this method should always call World::addScrutiny to save result of the scrutiny
     */
    Q_INVOKABLE virtual void finish() = 0;

    /** Finish learning session with the last shown card not learned. */
    Q_INVOKABLE virtual void cancel() = 0;

 protected:
    /** @throws logic_exception if lastCardShown_ is nullptr or its */
    Scrutiny currentScrunity() throw(std::logic_error);

    CardLearningModel *lastCardShown_;
};

class RandomCardStrategy : public LearningStrategyBase {
 public:
    RandomCardStrategy(QObject *parent = Q_NULLPTR);

    CardLearningModel *firstCard() override;
    CardLearningModel *nextCard() override;
    void finish() override;
    void cancel() override;

 private:
    DBCard *randomCard();
};

class SimpleDiffStrategy : public LearningStrategyBase {
 public:
    SimpleDiffStrategy(QObject *parent = Q_NULLPTR);

    CardLearningModel *firstCard() override;
    CardLearningModel *nextCard() override;
    void finish() override;
    void cancel() override;

 private:
    qint32 getDiffOfDifficultyRate(CardDifficulty::Rate rate);
    CardLearningModel *nextCardLearningModel();
    /** If last shown card was not emoty, add the scrutiny to learning history and update cardQueue_ */
    void processLastScrutiny();

    QHash<QUuid, qint32> diffs_;

    typedef std::pair<QUuid, qint32> CardDiff;

    struct CompareDiff { bool operator()(const CardDiff &lhs, const CardDiff &rhs) { return lhs.second > rhs.second; }};

    typedef std::priority_queue<CardDiff, std::vector<CardDiff>, CompareDiff> CardDiffQueue;
    CardDiffQueue cardQueue_;
};


#endif //ROOSHA_CLIENT_LEARNINGSTRATEGY_H
