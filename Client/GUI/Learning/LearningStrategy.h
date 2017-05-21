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
#include <QtCore/QPointer>
#include "UserInputModels.h"
#include "CardLearningViewModels.h"
#include "CardDifficulty.h"

enum class LearningStrategyType {
    // NOTE: if any type is added to this enum, it should be added to LearningManager constructor

    SIMPLE_DIFF,
    SUPERMEMO_2
};

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
    CardLearningModel(QSharedPointer<Card> card_,
                      QPointer<CardViewModelBase> viewModel_,
                      QPointer<UserInputModelBase> inputModel_,
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
    QSharedPointer<Card> card_;
    QPointer<CardViewModelBase> viewModel_;
    QPointer<UserInputModelBase> inputModel_;
};

class LearningStrategyBase : public QObject {
 Q_OBJECT
 public:
    LearningStrategyBase(QObject *parent = Q_NULLPTR);

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

    /** Finish learning session with the last shown card not learned. If no cards have shown yet, do nothing. */
    Q_INVOKABLE virtual void cancel() = 0;

    virtual LearningStrategyType getType() = 0;

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
     */
    virtual void appendScrutinies(QList<Scrutiny> scrutinies) = 0;

    void appendScrutiny(Scrutiny scrutiny);

 protected:
    /** @throws logic_exception if lastCardShown_ is nullptr or its */
    Scrutiny currentScrutiny() throw(std::logic_error);

    QPointer<CardLearningModel> lastShownCard_;
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
    SimpleDiffStrategy(QObject *parent = Q_NULLPTR);

    CardLearningModel *firstCard() override;
    CardLearningModel *nextCard() override;
    void finish() override;
    void cancel() override;

 public slots:
    void onCardsCreated(QSet<QUuid> cardIds) override;
    void onCardsDeleted(QSet<QUuid> cardIds) override;
    void appendScrutinies(QList<Scrutiny> scrutinies) override;

    LearningStrategyType getType() override;
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

class SuperMemo2Strategy : public LearningStrategyBase {
 Q_OBJECT
 public:
    static constexpr double DIFFICULT_DEFAULT_FACTOR = 1.2;
    static constexpr double NORMAL_DEFAULT_FACTOR = 1.5;
    static constexpr double EASY_DEFAULT_FACTOR = 2.;
    static constexpr quint32 DEFAULT_INTERVAL_MAXIMUM = 60 * 24 * 60 * 60; // 60 days

    SuperMemo2Strategy(QObject *parent = Q_NULLPTR,
                       double difficultFactor = DIFFICULT_DEFAULT_FACTOR,
                       double normalFactor = NORMAL_DEFAULT_FACTOR,
                       double easyFactor = EASY_DEFAULT_FACTOR,
                       quint32 intervalMax = DEFAULT_INTERVAL_MAXIMUM);

    CardLearningModel *firstCard() override;
    CardLearningModel *nextCard() override;
    void finish() override;
    void cancel() override;
    LearningStrategyType getType() override;
    void onCardsCreated(QSet<QUuid> cardIds) override;
    void onCardsDeleted(QSet<QUuid> cardIds) override;
    void appendScrutinies(QList<Scrutiny> scrutinies) override;

 private:
    static const constexpr quint32 ONE_DAY_IN_SECONDS = 1 * 24 * 60 * 60;
    static const constexpr quint32 TEN_MINUTES_IN_SECONDS = 10 * 60;

    //@formatter:off
    struct CardInfo {
        QUuid cardId_;
        QDateTime nextScrutiny_;
        quint32 currentIntervalSeconds_;
        double currentFactor_;

        CardInfo(const QUuid &cardId_ = QUuid(),
                 const QDateTime &nextScrutiny_ = QDateTime::currentDateTime(),
                 quint32 currentIntervalSeconds = 0,
                 double currentFactor_ = 1.) :
                cardId_(cardId_),
                nextScrutiny_(nextScrutiny_),
                currentIntervalSeconds_(currentIntervalSeconds),
                currentFactor_(currentFactor_) {}
    };
    struct Comparator {bool operator()(const CardInfo &lhv, const CardInfo &rhv) {return lhv.nextScrutiny_ > rhv.nextScrutiny_;}};
    //@formatter:on

    CardInfo alteredCardInfo(const CardInfo &cardInfo, CardDifficulty::Rate scrutinyStatus);
    quint32 nextIntervalWhenSucceeded(quint32 currentInterval, double factor);
    CardLearningModel * learningModelForCard(const std::unique_ptr<CardInfo> &cardInfo);
    std::unique_ptr<CardInfo> nextCardInfo();

    double difficultFactor_;
    double normalFactor_;
    double easyFactor_;
    quint32 intervalMaximum_;

    std::unique_ptr<CardInfo> lastShownCardInfo_;
    std::priority_queue<CardInfo, std::vector<CardInfo>, Comparator> cardQueue_;
};

#endif //ROOSHA_CLIENT_LEARNINGSTRATEGY_H
