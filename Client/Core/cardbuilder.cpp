#include "cardbuilder.h"


CardBuilder::CardBuilder() {
    built = false;
    newCard = DBCard();
}

CardBuilder& CardBuilder::setSource(const QString &source) {
    check_flag();
    newCard.setSource(source);
    return *this;
}


CardBuilder& CardBuilder::editExample(const QString &example, const qint32 &pos) {
    check_flag();
    newCard.editExampleElem(example, pos);
    return *this;
}

CardBuilder& CardBuilder::insertExample(const QString &example, const qint32 &pos) {
    check_flag();
    newCard.insertExampleElem(example, pos);
    return *this;
}

CardBuilder& CardBuilder::deleteExample(const qint32 &pos) {
    check_flag();
    newCard.deleteExampleElem(pos);
    return *this;
}

CardBuilder& CardBuilder::editTarget(const QString &target, const qint32 &pos) {
    check_flag();
    newCard.editTargetElem(target, pos);
    return *this;
}

CardBuilder& CardBuilder::insertTarget(const QString &target, const qint32 &pos) {
    check_flag();
    newCard.insertTargetElem(target, pos);
    return *this;
}

CardBuilder& CardBuilder::deleteTarget(const qint32 &pos) {
    check_flag();
    newCard.deleteTargetElem(pos);
    return *this;
}

DBCard&& CardBuilder::build() {
    check_flag();
    built = true;
    return std::move(newCard);
}

void CardBuilder::check_flag() {
    if (built) {
        throw std::logic_error("Data already has been built");
    }
}
