//
// Created by gylo on 05.02.17.
//

#include <QDebug>

#include "LearningStrategy.h"

Card * CardLearningModel::getCard() {
    return card_;
}

CardViewModelBase * CardLearningModel::getViewModel() {
    return viewModel_;
}

UserInputModelBase * CardLearningModel::getInputModel() {
    return inputModel_;
}
CardLearningModel::CardLearningModel(Card *card_,
                                     CardViewModelBase *viewModel_,
                                     UserInputModelBase *inputModel_,
                                     QObject *parent) :
        QObject(parent),
        card_(card_),
        viewModel_(viewModel_),
        inputModel_(inputModel_) {
}

ILearningStrategy::ILearningStrategy(QObject *parent) :
        QObject(parent) {
}

TestStrategy::TestStrategy(QObject *parent) :
        ILearningStrategy(parent) {
}

CardLearningModel * TestStrategy::firstCard() {
    qDebug() << "TestStrategy::firstCard() called";

    auto cards = World::Instance().getCards().values();
    if (cards.size() == 0) {
        qDebug() << "No cards, nothing to return";
        return nullptr;
    }
    DBCard *card = cards[qrand() % cards.size()].data();
    CardViewModelBase *view = new CardViewModelBase("front", "back");
    UserInputModelBase *input = new UserInputModelBase("input");
    return new CardLearningModel(card, view, input);
}

CardLearningModel * TestStrategy::nextCard(CardDifficulty::Rate previousRate) {
    qDebug() << "TestStrategy::nextCard(" << previousRate << ") called";

    auto cards = World::Instance().getCards().values();
    if (cards.size() == 0) {
        qDebug() << "No cards, nothing to return";
        return nullptr;
    }
    DBCard *card = cards[qrand() % cards.size()].data();
    CardViewModelBase *view = new CardViewModelBase("front", "back");
    UserInputModelBase *input = new UserInputModelBase("input");
    return new CardLearningModel(card, view, input);
}

void TestStrategy::finish(CardDifficulty::Rate previousRate) {
    qDebug() << "TestStrategy::finish(" << previousRate << ") called";
}

void TestStrategy::cancel() {
    qDebug() << "TestStrategy::cancel() called";
}
