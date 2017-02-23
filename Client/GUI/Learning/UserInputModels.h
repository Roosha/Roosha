//
// Created by gylo on 06.02.17.
//

#ifndef ROOSHA_CLIENT_USERINPUTMODELS_H
#define ROOSHA_CLIENT_USERINPUTMODELS_H

#include <QtCore/QObject>

enum class LearningInputType {
    TEXT_USER_INPUT,
};

/// UserInputModel classes are used to store settings of user input, such as whether it should be checked
/// case-insensitively or not.
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

    virtual LearningInputType getType() const = 0;
 private:
    QString inputViewName_;
    QString statusViewName_;
};

class TextUserInputModel : public UserInputModelBase {
    Q_OBJECT
 public:
    Q_PROPERTY(bool caseInsensitive READ isCaseInsensitive)

    TextUserInputModel(bool isCaseInsensitive = true, QObject *parent = Q_NULLPTR);

    bool isCaseInsensitive() const;

    LearningInputType getType() const override;
 private:
    const bool caseInsensitive_;
};

#endif //ROOSHA_CLIENT_USERINPUTMODELS_H
