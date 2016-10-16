#include "translation.h"

Translation::Translation() {

}

Translation::Translation(QString d) : data(d) {}

QString Translation::getSource() const {
    return data;
}

void Translation::setSource(const QString &src) {
    data = src;
}
