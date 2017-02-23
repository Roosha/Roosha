//
// Created by gylo on 22.02.17.
//

#ifndef ROOSHA_CLIENT_SCRUTINY_H
#define ROOSHA_CLIENT_SCRUTINY_H

#include <QtCore/QUuid>
#include <QtCore/QDateTime>
#include <GUI/Learning/CardLearningViewModels.h>
#include <GUI/Learning/UserInputModels.h>
#include <GUI/Learning/CardDifficulty.h>
#include "Network/Proto/learning.pb.h"

class Scrutiny {
 public:
    Scrutiny();
    Scrutiny(QUuid cardId,
             QDateTime moment,
             LearningInputType inputMethod,
             LearningViewType viewMethod,
             CardDifficulty::Rate cardDifficulty);
    Scrutiny(const roosha::Scrutiny &data_);

    roosha::Scrutiny rawScrutiny() { return data_; }

    QUuid getCardId() const;
    QDateTime getMoment() const;
    LearningInputType getInputType() const;
    LearningViewType getViewType() const;
    CardDifficulty::Rate getDifficultyRate() const;

 private:
    roosha::Scrutiny data_;
};

#endif //ROOSHA_CLIENT_SCRUTINY_H
