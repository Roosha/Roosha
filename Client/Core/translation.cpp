#include "translation.h"
#include <QUuid>

Translation::Translation() {

}
QUuid Translation::getId() const {
    return QUuid();
}

QString Translation::getSource() const {
    return source;
}

void Translation::setSource(const QString &value) {
    source = value;
}

QStringList Translation::getTarget() const {
    return target;
}

void Translation::setTarget(const QStringList &value) {
    target = value;
}

QStringList Translation::getExamples() const {
    return examples;
}

void Translation::setExamples(const QStringList &value) {
    examples = value;
}

QString Translation::getExampleString() const {
    return examples.join(";\n");;
}

QString Translation::getTargetString() const {
    return target.join("; ");
}
