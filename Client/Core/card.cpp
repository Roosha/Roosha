#include "Core/card.h"

Card::Card() {}

QString Card::getSource() const {
    return source_;
}

QUuid Card::getId() const {
    return id_;
}

QStringList Card::getTargets() const {
    return targets_.toList();
}

QStringList Card::getExamples() const {
    return examples_.toList();
}

QString Card::getExampleString() const {
    return examples_.toList().join(";\n");
}

QString Card::getTargetString() const {
    return targets_.toList().join("; ");
}
