//
// Created by gylo on 06.02.17.
//

#include "UserInputModels.h"
QString UserInputModelBase::getViewName() const {
    return viewName_;
}
UserInputModelBase::UserInputModelBase(const QString &viewName_, QObject *parent) :
        QObject(parent),
        viewName_(viewName_) {}
