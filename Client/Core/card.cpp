#include "Core/card.h"

Card::Card() { }

QString Card::getSource() const{
    return source;
}

QStringList Card::getTargets() const{
    return targets;
}

QStringList Card::getExamples() const{
    return examples;
}

QString Card::getExampleString() const {
    return examples.join("; ");
}

QString Card::getTargetString() const {
    return targets.join("; ");
}
