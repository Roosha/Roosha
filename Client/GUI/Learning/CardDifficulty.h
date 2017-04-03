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

    static quint32 ord(Rate rate) {
        switch (rate) {
            case Rate::UNKNOWN: return 0;
            case Rate::SKIPPED: return 1;
            case Rate::FAILED: return 2;
            case Rate::EASY: return 3;
            case Rate::NORMAL: return 4;
            case Rate::DIFFICULT: return 5;
        }
    }
};


#endif //ROOSHA_CLIENT_CARDDIFFICULTY_H
