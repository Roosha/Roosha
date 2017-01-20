#include "StateHolder.h"

StateHolder::StateHolder() : auth(false) {}
StateHolder::~StateHolder() {}

StateHolder &StateHolder::Instance() {
    static StateHolder stateHolder;
    return stateHolder;
}
bool StateHolder::getAuth() const {
    return auth;
}

void StateHolder::setAuth(bool newAuth) {
    auth = newAuth;
    emit authChanged();
}
qint32 StateHolder::getEditions() const {
    return editions;
}
void StateHolder::setEditions(qint32 edits) {
    editions = edits;
    emit editionChange();
}

bool StateHolder::getSync() const {
    return sync;
}
void StateHolder::setSync(bool newSync) {
    sync = newSync;
    emit syncChanged();
}


