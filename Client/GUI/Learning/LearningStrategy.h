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

    /** @see LearningStrategyBase::scrutiniesNumber_ */
    quint32 getScrutiniesNumber();

 public slots:
    /** @see onCardsCreated */
    void onCardCreated(QUuid cardId);

    /**
     * Notify the strategy on creation of cards.
     *
     * <b>NOTE:</b> recurrent call of this method with the same cardId may make following behaviour of the strategy
     * undefined.
     * @param cardIds ids of created cards
     */
    virtual void onCardsCreated(QSet<QUuid> cardIds) = 0;

    /** @see onCardsDeleted */
    void onCardDeleted(QUuid cardId);

    /**
     * Notify the strategy on deletion of cards.
     * @param cardIds ids of deleted cards
     */
    virtual void onCardsDeleted(QSet<QUuid> cardIds) = 0;

    /**
     * Adjust strategy taking into account specified scrutinies.
     *
     * <b>NOTE:</b> the strategy should be familiar with existence of all cards specified in scrutinies. Otherwise,
     * these scrutinies will be ignored.
     * @see LearningStrategyBase::scrutiniesNumber_
     */
    virtual void appendScrutinies(QList<Scrutiny> scrutinies) = 0;

 protected:
    /** @throws logic_exception if lastCardShown_ is nullptr or its */
    Scrutiny currentScrutiny() throw(std::logic_error);

    /**
     * This counter stores length of learning history taken into account by the strategy. It should be incremented
     * whenever strategy receives new scrutiny, whether from QML or LearningStrategyBase::appendScrutinies method.
     */
    quint32 scrutiniesNumber_;

    CardLearningModel *lastCardShown_;
};

class SimpleDiffStrategy : public LearningStrategyBase {
 Q_OBJECT
 public:
    /**
     * Create simple diff strategy for specified card list and learning history.
     *
     * <b>NOTE:</b> cardIds should contain all ids specified in scrutinies. Otherwise, behaviour is undefined.
     * @param cardIds list of card ids for learning
     * @param scrutinies learning history
     * @param parent parent
     */
    SimpleDiffStrategy(QList<QUuid> cardIds = QList<QUuid>(),
                       QList<Scrutiny> scrutinies = QList<Scrutiny>(),
                       QObject *parent = Q_NULLPTR);

    CardLearningModel *firstCard() override;
    CardLearningModel *nextCard() override;
    void finish() override;
    void cancel() override;

 public slots:
    void onCardsCreated(QSet<QUuid> cardIds) override;
    void onCardsDeleted(QSet<QUuid> cardIds) override;
    void appendScrutinies(QList<Scrutiny> scrutinies) override;
 private:
    qint32 getDiffOfDifficultyRate(CardDifficulty::Rate rate);
    CardLearningModel *nextCardLearningModel();
    /** If last shown card was not empty, add the scrutiny to learning history and update cardQueue_ */
    void processLastScrutiny();

    /** priorities of all existing cards. */
    QHash<QUuid, qint32> diffs_;

    //@formatter:off
    typedef std::pair<QUuid, qint32> CardDiff;
    struct CompareDiff { bool operator()(const CardDiff &lhs, const CardDiff &rhs) { return lhs.second > rhs.second; }};
    typedef std::priority_queue<CardDiff, std::vector<CardDiff>, CompareDiff> CardDiffQueue;
    /** may contain non-existing card. Whenever take element of this queue, check if it's cardId is contained in diffs_ */
    CardDiffQueue cardQueue_;
    //@formatter:on
};

#endif //ROOSHA_CLIENT_LEARNINGSTRATEGY_H
