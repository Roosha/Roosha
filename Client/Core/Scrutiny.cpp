//
// Created by gylo on 22.02.17.
//

#include "Scrutiny.h"
#include "Helpers/protobuf_converter.h"

using namespace ProtobufConverter;

Scrutiny::Scrutiny() : IChange(roosha::Change()){
    rawChange.mutable_scrutiny();
}


Scrutiny::Scrutiny(QUuid cardId,
                   QDateTime moment,
                   LearningInputType inputMethod,
                   LearningViewType viewMethod,
                   CardDifficulty::Rate cardDifficulty):
        IChange(roosha::Change()) {
    auto rawScrutiny = rawChange.mutable_scrutiny();
    rawScrutiny->set_cardid(cardId.toString().toStdString());
    rawScrutiny->set_moment(moment.toString(Qt::ISODate).toStdString());
    rawScrutiny->set_inputmethod(learningInputToProtobuf(inputMethod));
    rawScrutiny->set_viewmethod(learningViewToProtobuf(viewMethod));
    rawScrutiny->set_status(cardDifficultyRateToProtobuf(cardDifficulty));
}

Scrutiny::Scrutiny(const roosha::Change &data) : IChange(data) {
    if (data.change_case() != roosha::Change::kScrutiny) {
        throw std::invalid_argument("rawData passed to Scrutiny::Scrutiny(const roosha::Change&) is not a scrutiny");
    }
}

void Scrutiny::apply(World *world) {
    // nop
}

QUuid Scrutiny::getCardId() const {
    return QUuid(rawScrutiny().cardid().c_str());
}

QDateTime Scrutiny::getMoment() const {
    return QDateTime::fromString(QString::fromStdString(rawScrutiny().moment()), Qt::ISODate);
}

LearningInputType Scrutiny::getInputType() const {
    return learningInputFromProtobuf(rawScrutiny().inputmethod());
}

LearningViewType Scrutiny::getViewType() const {
    return learningViewFromProtobuf(rawScrutiny().viewmethod());
}

CardDifficulty::Rate Scrutiny::getDifficultyRate() const {
    return cardDifficultyRateFromProtobuf(rawScrutiny().status());
}

const roosha::Scrutiny &Scrutiny::rawScrutiny() const {
    return rawChange.scrutiny();
}
