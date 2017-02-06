//
// Created by gylo on 06.02.17.
//

#ifndef ROOSHA_CLIENT_USERINPUTMODELS_H
#define ROOSHA_CLIENT_USERINPUTMODELS_H

#include <QtCore/QObject>

// TODO: there should be stored input status, received from Javascript
class UserInputModelBase : public QObject {
    //@formatter:off
    Q_OBJECT
 public:
    Q_PROPERTY(QString viewName READ getViewName)
    //@formatter:on
 public:
    UserInputModelBase(const QString &viewName_, QObject *parent = Q_NULLPTR);

    QString getViewName() const;
 private:
    QString viewName_;
};

#endif //ROOSHA_CLIENT_USERINPUTMODELS_H
