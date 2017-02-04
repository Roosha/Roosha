#include "StateHolder.h"

StateHolder::StateHolder() : isAuthorized(false) {}
StateHolder::~StateHolder() {}

StateHolder &StateHolder::Instance() {
    static StateHolder stateHolder;
    return stateHolder;
}
bool StateHolder::isAuth() const {
    return isAuthorized;
}

void StateHolder::setAuth(bool newAuth) {
    isAuthorized = newAuth;
    emit authChanged();
}
qint32 StateHolder::getEditors() const {
    return editors;
}
void StateHolder::setEditors(qint32 edits) {
    editors = edits;
    emit editionChange();
}

bool StateHolder::isSync() const {
    return synchronizationInProgress;
}
void StateHolder::setSync(bool newSync) {
    synchronizationInProgress = newSync;
    emit syncChanged();
}
bool StateHolder::isEditing(QUuid id) {
    return editingCards.contains(id);
};
void StateHolder::startEdit(QUuid id) {
    editingCards.insert(id);
};
void StateHolder::endEdit(QUuid id) {
    editingCards.remove(id);
};

