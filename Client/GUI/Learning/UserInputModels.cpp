//
// Created by gylo on 06.02.17.
//

#include "UserInputModels.h"

UserInputModelBase::UserInputModelBase(const QString &inputViewName, const QString &statusViewName, QObject *parent) :
        QObject(parent),
        inputViewName_(inputViewName),
        statusViewName_(statusViewName) {
}

QString UserInputModelBase::getInputViewName() const {
    return inputViewName_;
}

QString UserInputModelBase::getStatusViewName() const {
    return statusViewName_;
}

TextUserInputModel::TextUserInputModel(bool isCaseInsensitive, QObject *parent) :
        UserInputModelBase("inputs/TextInputView.qml", "inputs/TextInputStatus.qml", parent),
        caseInsensitive_(isCaseInsensitive) {
}

bool TextUserInputModel::isCaseInsensitive() const {
    return caseInsensitive_;
}

LearningInputType TextUserInputModel::getType() const {
    return LearningInputType::TEXT_USER_INPUT;
}
