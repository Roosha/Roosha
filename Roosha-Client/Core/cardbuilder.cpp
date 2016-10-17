#include "cardbuilder.h"


CardBuilder::CardBuilder() {
    built = false;
    newCard = DBCard();
}

CardBuilder& CardBuilder::setSource(QString source) {
    check_flag();
    newCard.setSource(source);
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
