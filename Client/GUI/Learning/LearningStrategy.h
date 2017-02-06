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
    Q_PROPERTY(QObject* card READ getCard)
    Q_PROPERTY(QObject* viewModel READ getViewModel)
    Q_PROPERTY(QObject* inputModel READ getInputModel)
    //@formatter:on
 public:
    CardLearningModel(DBCard *card_,
                      CardViewModelBase *viewModel_,
                      UserInputModelBase *inputModel_,
                      QObject *parent = Q_NULLPTR);

    QObject *getCard();
    QObject *getViewModel();
    QObject *getInputModel();
 private:
    DBCard *card_;
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

    static void registerInQml() {
        static bool registered = false;
        if (!registered) {
            qmlRegisterType<CardDifficulty>("roosha.learning", 1, 0, "Difficulty");
            qRegisterMetaType<Rate>();
            registered = true;
        }
    }
};

class ILearningStrategy : public QObject {
    Q_OBJECT
 public:
    ILearningStrategy(QObject *parent);

    /**
     * This function should be called to get the first card in learning session. To get subsequent cards, use
     * nextCard method.
     * @return a pointer to CardLearningModel instance for next card to learn.
     */
    Q_INVOKABLE virtual QObject *firstCard() = 0;

    /**
     * Get the next card to learn.
     * @param previousRate difficulty of previous learned card.
     * @return a pointer to CardLeaningModel instance for next card to learn.
     */
    Q_INVOKABLE virtual QObject *nextCard(CardDifficulty::Rate previousRate) = 0;

    /**
     * Finish learning session successfully by setting difficulty of the last learned card.
     * If the last card returned by firstCard or nextCard methods was not learned, use cancel method.
     * @param previousRate
     */
    Q_INVOKABLE virtual void finish(CardDifficulty::Rate previousRate) = 0;

    /**
     * Finish learning session with the last shown card not learned.
     */
    Q_INVOKABLE virtual void cancel() = 0;
};

class TestStrategy : public ILearningStrategy {
 public:
    TestStrategy(QObject *parent = Q_NULLPTR);

    QObject *firstCard() override;
    QObject *nextCard(CardDifficulty::Rate previousRate) override;
    void finish(CardDifficulty::Rate previousRate) override;
    void cancel() override;
};

#endif //ROOSHA_CLIENT_LEARNINGSTRATEGY_H
