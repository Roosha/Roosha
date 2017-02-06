//
// Created by gylo on 06.02.17.
//

#include "CardLearningViewModels.h"

CardViewModelBase::CardViewModelBase(const QString &frontViewName, const QString &backViewName, QObject *parent) :
        QObject(parent),
        frontViewName_(frontViewName),
        backViewName_(backViewName) {}

QString CardViewModelBase::getFrontViewName() const {
    return frontViewName_;
}

QString CardViewModelBase::getBackViewName() const {
    return backViewName_;
}
