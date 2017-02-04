//
// Created by gylo on 05.02.17.
//

#include "CardLearningModel.h"

CardLearningModel::CardLearningModel(DBCard *card, CardViewMode viewMode, UserInputMode inputMode) :
        QObject(Q_NULLPTR),
        card_(card),
        viewMode_(viewMode),
        inputMode_(inputMode) {
}

QObject *CardLearningModel::getCard() const {
    return card_;
}

CardLearningModel::CardViewMode CardLearningModel::getViewMode() const {
    return viewMode_;
}

CardLearningModel::UserInputMode CardLearningModel::getInputMode() const {
    return inputMode_;
}
