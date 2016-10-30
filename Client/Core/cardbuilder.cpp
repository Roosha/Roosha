#include "cardbuilder.h"
#include "changes.h"

CardBuilder::CardBuilder() {
    built = false;
    newCard = DBCard();
}

CardBuilder& CardBuilder::setSource(const QString &source) {
    check_flag();
    newCard.setSource(source);
    return *this;
}


CardBuilder& CardBuilder::edit(const enum Field field, const QString &example, const qint32 &pos) {
    check_flag();
    newCard.editElem(field, example, pos);
    return *this;
}

CardBuilder& CardBuilder::insert(const enum Field field, const QString &example, const qint32 &pos) {
    check_flag();
    newCard.insertElem(field, example, pos);
    return *this;
}

CardBuilder& CardBuilder::del(const enum Field field, const qint32 &pos) {
    check_flag();
    newCard.deleteElem(field, pos);
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
