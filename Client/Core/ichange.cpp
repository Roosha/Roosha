//
// Created by anna on 03.04.17.
//


#include "ichange.h"

IChange::~IChange() {}
IChange::IChange(roosha::Change change) : rawChange(change) {}
CMP IChange::compare(IChange *otherChange) {
    roosha::Change ch1 = this->rawChange;
    roosha::Change ch2 = otherChange->rawChange;
    if (ch1.change_case() != roosha::Change::kCardChange &&
        ch2.change_case() != roosha::Change::kCardChange)
        return DIFFER; // изменения прогресса и т.д. не конфликтуют, то есть нас интресует только случай, когда оба изменения -- изменения карточек

    roosha::CardChange cch1 = ch1.cardchange();
    roosha::CardChange cch2 = ch2.cardchange();
    if (cch1.cardid() != cch1.cardid() || cch1.change_case() != cch2.change_case())
        return DIFFER;
    roosha::CardChange::ChangeCase changeCase = cch1.change_case();
    cch1.cardid();
    if (changeCase == roosha::CardChange::kChangeSource) {
        if (cch1.changesource().newsource() == cch2.changesource().newsource())
            return EQUAL;
        else
            return CONFLICT;
    }
    if (changeCase == roosha::CardChange::kEditElem) {
        if (cch1.editelem().field() == cch1.editelem().field() && cch1.editelem().position() == cch2.editelem().position()) {
            if (cch1.editelem().value() == cch1.editelem().value())
                return EQUAL;
            else
                return CONFLICT;
        }
    }
    return DIFFER;
}