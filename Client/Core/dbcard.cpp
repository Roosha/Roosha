#include "Core/dbcard.h"
#include <QDebug>

DBCard::DBCard() { }
DBCard::DBCard(QUuid newId) : id(newId) { }

QUuid DBCard::getId() {
    return id;
}
