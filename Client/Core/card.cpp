#include "Core/card.h"

Card::Card() { }

QString Card::getSource() {
    return source;
}

QStringList Card::getTargets() {
    return targets;
}

QStringList Card::getExamples() {
    return examples;
}
