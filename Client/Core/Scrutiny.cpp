//
// Created by gylo on 22.02.17.
//

#include "Scrutiny.h"
#include "Helpers/protobuf_converter.h"

using namespace ProtobufConverter;

Scrutiny::Scrutiny() {}

Scrutiny::Scrutiny(QUuid cardId,
                   QDateTime moment,
                   LearningInputType inputMethod,
                   LearningViewType viewMethod,
                   CardDifficulty::Rate cardDifficulty) {
    data_.set_cardid(cardId.toString().toStdString());
    data_.set_moment(moment.toString(Qt::ISODate).toStdString());
    data_.set_inputmethod(learningInputToProtobuf(inputMethod));
    data_.set_viewmethod(learningViewToProtobuf(viewMethod));
    data_.set_status(cardDifficultyRateToProtobuf(cardDifficulty));
}

Scrutiny::Scrutiny(const roosha::Scrutiny &data_) : data_(data_) {}

QUuid Scrutiny::getCardId() const {
    return QUuid(data_.cardid().c_str());
}

QDateTime Scrutiny::getMoment() const {
    return QDateTime::fromString(QString::fromStdString(data_.moment()), Qt::ISODate);
}

LearningInputType Scrutiny::getInputType() const {
    return learningInputFromProtobuf(data_.inputmethod());
}

LearningViewType Scrutiny::getViewType() const {
    return learningViewFromProtobuf(data_.viewmethod());
}

CardDifficulty::Rate Scrutiny::getDifficultyRate() const {
    return cardDifficultyRateFromProtobuf(data_.status());
}


