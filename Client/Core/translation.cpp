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

QString Translation::getExampleString() const {
    QString str = examples[0];
    for (int i = 1; i < examples.size(); i++) {
        str += "; ";
        str += examples[i];
    }
    return str;
}

QString Translation::getTargetString() const {

    QString str = target[0];
    for (int i = 1; i < target.size(); i++) {
        str += "; ";
        str += target[i];
    }
    return str;
}
