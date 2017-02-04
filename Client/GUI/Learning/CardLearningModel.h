// Created by gylo on 05.02.17.
//

#ifndef ROOSHA_CLIENT_CARDLEARNINGMODEL_H
#define ROOSHA_CLIENT_CARDLEARNINGMODEL_H

#include <QtCore/QObject>
#include <Core/dbcard.h>

class CardLearningModel : public QObject{
 Q_OBJECT
 public:
    //@formatter:off
    enum CardViewMode {
        PLAIN
    };
    Q_ENUM(CardViewMode)

    enum UserInputMode {
        TEXT_INPUT
    };
    Q_ENUM(UserInputMode)

    Q_PROPERTY(QObject* card READ getCard)
    Q_PROPERTY(CardViewMode viewMode READ getViewMode)
    Q_PROPERTY(UserInputMode inputMode READ getInputMode)
    //@formatter:on
 public:
    CardLearningModel(DBCard *card, CardViewMode viewMode, UserInputMode inputMode);

    QObject *getCard() const;
    CardViewMode getViewMode() const;
    UserInputMode getInputMode() const;

 private:
    DBCard *card_;
    CardViewMode viewMode_;
    UserInputMode inputMode_;
};

#endif //ROOSHA_CLIENT_CARDLEARNINGMODEL_H
