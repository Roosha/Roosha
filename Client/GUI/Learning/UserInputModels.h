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
    Q_PROPERTY(QString inputViewName READ getInputViewName)
    Q_PROPERTY(QString statusViewName READ getStatusViewName)
    //@formatter:on
 public:
    UserInputModelBase(const QString &inputViewName, const QString &statusViewName, QObject *parent = Q_NULLPTR);

    QString getInputViewName() const;
    QString getStatusViewName() const;
 private:
    QString inputViewName_;
    QString statusViewName_;
};

class TextUserInputModel : public UserInputModelBase {
 public:
    TextUserInputModel(QObject *parent = Q_NULLPTR);
};

#endif //ROOSHA_CLIENT_USERINPUTMODELS_H
