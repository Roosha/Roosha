//
// Created by gylo on 22.02.17.
//

#ifndef ROOSHA_CLIENT_CARDDIFFICULTY_H
#define ROOSHA_CLIENT_CARDDIFFICULTY_H

#include <QObject>

class CardDifficulty : public QObject {
 Q_OBJECT
 public:
    enum class Rate {
        UNKNOWN,
        SKIPPED,
        FAILED,
        EASY,
        NORMAL,
        DIFFICULT
    };

    Q_ENUM(Rate)
};


#endif //ROOSHA_CLIENT_CARDDIFFICULTY_H
