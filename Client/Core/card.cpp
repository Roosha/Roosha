#include "Core/card.h"

Card::Card() { }

QString Card::getSource() const{
    return source_;
}

QUuid Card::getId() {
    return id_;
}

QStringList Card::getTargets() const{
    return targets_;
}

QStringList Card::getExamples() const{
    return examples_;
}

QString Card::getExampleString() const {
    return examples_.join(";\n");
}

QString Card::getTargetString() const {
    return targets_.join("; ");
}
