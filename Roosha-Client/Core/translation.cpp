#include "translation.h"

Translation::Translation() {

}

QString Translation::getSource() const
{
    return source;
}

void Translation::setSource(const QString &value)
{
    source = value;
}

QStringList Translation::getTarget() const
{
    return target;
}

void Translation::setTarget(const QStringList &value)
{
    target = value;
}

QStringList Translation::getExamples() const
{
    return examples;
}

void Translation::setExamples(const QStringList &value)
{
    examples = value;
}
