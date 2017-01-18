#include "Network/network_manager.h"
#include "authentication_manager.h"
#include "Helpers/protobuf_converter.h"

#include <QMessageBox>
#include <QThread>

#ifndef DEBUG_CALL
#define DEBUG_CALL(methodName)\
    qDebug("NetworkManager::%s: called", (methodName));
#else
#error DEBUG_CALL macro is already defined
#endif

using ProtobufConverter::translationsToProtobuf;

NetworkManager::NetworkManager(QObject *parent) :
        QObject(parent),
        currentId_(MINIMAL_PUBLIC_RPC_ID),
        authenticationManager_(new AuthenticationManager(this)) {

    qDebug("NetworkManager created");
}

NetworkManager::~NetworkManager() {
    qDebug("NetworkManager destroyed");
}

quint32 NetworkManager::translate(QString source, quint32 timeoutMills) {
    DEBUG_CALL("translate")
    TranslateAsyncCall *call = new TranslateAsyncCall(++currentId_, timeoutMills);
    call->request_.set_source(source.toStdString());
    call->authenticate(authenticationManager_);
    return call->id_;
}

quint32 NetworkManager::proposeUserTranslations(Translations translations, quint32 timeoutMills) {
    DEBUG_CALL("proposeUserTranslations")
    ProposeUserTranslationsAsyncCall *call = new ProposeUserTranslationsAsyncCall(++currentId_, timeoutMills);
    if (translations.size() != 0) {
//        call->request_ = translationsToProtobuf(translations);
    }

    call->authenticate(authenticationManager_);
    return call->id_;
}

quint32 NetworkManager::authorize(QString login, QString password, quint32 timeoutMills) {
    DEBUG_CALL("authorize")
    AuthorizeAsyncCall *call = new AuthorizeAsyncCall(++currentId_, timeoutMills);
    call->request_.set_login(login.toStdString());
    call->request_.set_passwordhash(AuthenticationManager::hashPassword(password));

    call->authenticate(authenticationManager_);
    return call->id_;
}

quint32 NetworkManager::registrate(QString login, QString password, quint32 timeoutMills) {
    DEBUG_CALL("registrate")
    RegistrateAsyncCall *call = new RegistrateAsyncCall(++currentId_, timeoutMills);
    call->request_.set_login(login.toStdString());
    call->request_.set_passwordhash(AuthenticationManager::hashPassword(password));

    call->authenticate(authenticationManager_);
    return call->id_;
}

quint32 NetworkManager::saveChanges(ChangeList changes, quint32 timeoutMillis) {
    DEBUG_CALL("saveChanges")
    SaveChangesAsyncCall *call = new SaveChangesAsyncCall(++currentId_, timeoutMillis);
    for (auto change : changes) {
        call->request_.append(change->toProtobuf());
    }

    call->authenticate(authenticationManager_);
    return call->id_;
}

quint32 NetworkManager::loadChanges(quint32 timeoutMillis) {
    DEBUG_CALL("loadChanges")
    LoadChangesAsyncCall *call = new LoadChangesAsyncCall(++currentId_, timeoutMillis);

    call->authenticate(authenticationManager_);
    return call->id_;
}

#ifdef DEBUG_CALL
#undef DEBUG_CALL
#else
#error DEBUG_CALL macro is not defined
#endif // DEBUG_CALL
