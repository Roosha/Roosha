//
// Created by gylo on 06.02.17.
//

#ifndef ROOSHA_CLIENT_CARDLEARNINGVIEWMODEL_H
#define ROOSHA_CLIENT_CARDLEARNINGVIEWMODEL_H

#include <QtCore/QObject>

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

 private:
    QString frontViewName_;
    QString backViewName_;
};

class TargetsAndExampleViewModel : public CardViewModelBase {
 public:
    TargetsAndExampleViewModel(QObject *parent = Q_NULLPTR);
};

#endif //ROOSHA_CLIENT_CARDLEARNINGVIEWMODEL_H
