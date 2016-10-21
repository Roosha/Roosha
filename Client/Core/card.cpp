#include "Core/card.h"

Card::Card() { }

QString Card::getSource() {
    return source;
}

QVector<QString> Card::getTargets() {
    return targets;
}

QVector<QString> Card::getExamples() {
    return examples;
}
