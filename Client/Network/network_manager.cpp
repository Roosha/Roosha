#include "Network/network_manager.h"
#include "authentication_manager.h"
#include "server_response.h"
#include "Helpers/protobuf_converter.h"

#include <QList>
#include <QSharedPointer>
#include <QMessageBox>
#include <QThread>

using ProtobufConverter::translationsToProtobuf;

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent),
    authenticationManager_(new AuthenticationManager(this)) {
}

NetworkManager::~NetworkManager() {
}

quint32 NetworkManager::translate(QString source, quint32 timeoutMills) {
    TranslateAsyncCall *call = new TranslateAsyncCall(++currentId_, timeoutMills);
    call->request_.set_source(source.toStdString());
    call->authenticate(authenticationManager_);
    return call->id_;
}

quint32 NetworkManager::proposeUserTranslations(Translations translations, quint32 timeoutMills) {
    ProposeUserTranslationsAsyncCall *call = new ProposeUserTranslationsAsyncCall(++currentId_, timeoutMills);
    if (translations.size() != 0) {
        call->request_ = translationsToProtobuf(translations);
    }

    call->authenticate(authenticationManager_);
    return call->id_;
}

quint32 NetworkManager::authorize(QString login, QString password, quint32 timeoutMills) {
    AuthorizeAsyncCall *call = new AuthorizeAsyncCall(++currentId_, timeoutMills);
    call->request_.set_login(login.toStdString());
    call->request_.set_passwordhash(password.toStdString()); // TODO: pass hash insted of raw password

    call->authenticate(authenticationManager_);
    return call->id_;
}

quint32 NetworkManager::registrate(QString login, QString password, quint32 timeoutMills) {
    RegistrateAsyncCall *call = new RegistrateAsyncCall(++currentId_, timeoutMills);
    call->request_.set_login(login.toStdString());
    call->request_.set_passwordhash(password.toStdString()); // TODO: pass hash insted of raw password

    call->authenticate(authenticationManager_);
    return call->id_;
}

quint32 NetworkManager::saveChanges(QList<QSharedPointer<IChange>> changes, quint32 timeoutMillis) {
    SaveChangesAsyncCall *call = new SaveChangesAsyncCall(++currentId_, timeoutMillis);
    for (auto change : changes) {
        call->request_.append(change->toProtobuf());
    }

    call->authenticate(authenticationManager_);
    return call->id_;
}

quint32 NetworkManager::loadChanges(quint32 timeoutMillis) {
    LoadChangesAsyncCall *call = new LoadChangesAsyncCall(++currentId_, timeoutMillis);

    call->authenticate(authenticationManager_);
    return call->id_;
}
