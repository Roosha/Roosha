//
// Created by gylo on 06.02.17.
//

#ifndef ROOSHA_CLIENT_CARDLEARNINGVIEWMODEL_H
#define ROOSHA_CLIENT_CARDLEARNINGVIEWMODEL_H

#include <QtCore/QObject>

enum class LearningViewType {
    TARGETS_AND_EXAMPLE,
};

class CardViewModelBase : public QObject {
    //@formatter:off
 Q_OBJECT
 public:
    Q_PROPERTY(QString frontViewName READ getFrontViewName)
    Q_PROPERTY(QString backViewName READ getBackViewName)
    //@formatter:on
 public:
    CardViewModelBase(const QString &frontSideViewName_, const QString &backSideViewName_, QObject *parent = Q_NULLPTR);

    QString getFrontViewName() const;
    QString getBackViewName() const;

    virtual LearningViewType getType() const = 0;
 private:
    QString frontViewName_;
    QString backViewName_;
};

class TargetsAndExampleViewModel : public CardViewModelBase {
 public:
    TargetsAndExampleViewModel(QObject *parent = Q_NULLPTR);

    LearningViewType getType() const override;
};

#endif //ROOSHA_CLIENT_CARDLEARNINGVIEWMODEL_H
