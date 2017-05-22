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
#include "ichange.h"

class Scrutiny : public IChange {
 public:
    Scrutiny();

    Scrutiny(QUuid cardId,
             QDateTime moment,
             LearningInputType inputMethod,
             LearningViewType viewMethod,
             CardDifficulty::Rate cardDifficulty);
    Scrutiny(const roosha::Change &data);

    void apply(World *world) override;

    QUuid getCardId() const;
    QDateTime getMoment() const;
    LearningInputType getInputType() const;
    LearningViewType getViewType() const;
    CardDifficulty::Rate getDifficultyRate() const;

 private:
    const roosha::Scrutiny &rawScrutiny() const;
};

#endif //ROOSHA_CLIENT_SCRUTINY_H
