#include "Core/card.h"

Card::Card()
{

}

QString Card::getSource()
{
    return source;
}

QVector<QString> Card::getTarget()
{
    return target;
}

QVector<QString> Card::getExamples()
{
    return examples;
}
