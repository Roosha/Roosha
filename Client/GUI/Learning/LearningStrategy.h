//
// Created by gylo on 05.02.17.
//

#ifndef ROOSHA_CLIENT_LEARNINGSTRATEGY_H
#define ROOSHA_CLIENT_LEARNINGSTRATEGY_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <Core/dbcard.h>
#include "UserInputModels.h"
#include "CardLearningViewModels.h"
#include <QtQml/qqml.h>

class CardLearningModel : public QObject {
    //@formatter:off
    Q_OBJECT
 public:
    Q_PROPERTY(Card* card READ getCard)
    Q_PROPERTY(CardViewModelBase* viewModel READ getViewModel)
    Q_PROPERTY(UserInputModelBase* inputModel READ getInputModel)
    Q_PROPERTY(bool empty READ isEmpty)
    //@formatter:on
 public:
    /// create instance with empty flag set.
    static CardLearningModel *emptyCard();

    CardLearningModel(Card *card_,
                      CardViewModelBase *viewModel_,
                      UserInputModelBase *inputModel_,
                      QObject *parent = Q_NULLPTR);

    /// if this flag is set, there is no cards to learn, so nothing to show
    bool isEmpty();
    Card *getCard();
    CardViewModelBase *getViewModel();
    UserInputModelBase *getInputModel();
 private:
    /// Empty model constructor
    CardLearningModel();

    bool empty_;
    Card *card_;
    CardViewModelBase *viewModel_;
    UserInputModelBase *inputModel_;
};

class CardDifficulty : public QObject {
 Q_OBJECT
 public:
    enum Rate {
        FAILED,
        EASY,
        NORMAL,
        DIFFICULT
    };

    Q_ENUM(Rate)
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
     * Get the next card to learn.
     * @param previousRate difficulty of previous learned card.
     * @return next card to learn.
     */
    Q_INVOKABLE virtual CardLearningModel *nextCard(CardDifficulty::Rate previousRate) = 0;

    /** Get current card. */
    Q_INVOKABLE CardLearningModel *currentCard();

    /**
     * Finish learning session successfully by setting difficulty of the last learned card.
     * If the last card returned by firstCard or nextCard methods was not learned, use cancel method.
     * @param previousRate
     */
    Q_INVOKABLE virtual void finish(CardDifficulty::Rate previousRate) = 0;

    /** Finish learning session with the last shown card not learned. */
    Q_INVOKABLE virtual void cancel() = 0;

 protected:
    CardLearningModel *lastCardShown_;
};

class RandomCardStrategy : public LearningStrategyBase {
 public:
    RandomCardStrategy(QObject *parent = Q_NULLPTR);

    CardLearningModel *firstCard() override;
    CardLearningModel *nextCard(CardDifficulty::Rate previousRate) override;
    void finish(CardDifficulty::Rate previousRate) override;
    void cancel() override;

 private:
    DBCard *randomCard();
};

#endif //ROOSHA_CLIENT_LEARNINGSTRATEGY_H
