#include "Network/network_manager.h"
#include "authentication_manager.h"
#include "server_response.h"

#include <QList>
#include <QSharedPointer>
#include <QMessageBox>
#include <QThread>

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent),
    authenticationManager_(new AuthenticationManager(this)) {
}

NetworkManager::~NetworkManager() {
}

quint32 NetworkManager::translate(QString source, quint32 timeoutMills) {
    TranslateAsyncCall *call = new TranslateAsyncCall(++currentId_, timeoutMills);
    call->request_.set_source(source.toStdString());
    authenticationManager_->translate(call);
    return call->id_;
}

quint32 NetworkManager::proposeUserTranslations(Translations translations, quint32 timeoutMills) {
    ProposeUserTranslationsAsyncCall *call = new ProposeUserTranslationsAsyncCall(++currentId_, timeoutMills);
    if (translations.size() != 0) {
        call-> request_.set_source(translations[0]->getSource().toStdString());
        for (const auto& translation: translations) {
            roosha::Translation *reqTranslation = call->request_.add_translation();
            reqTranslation->set_provider(roosha::TranslationProvider::UNKNOWN);
            for (auto target: translation->getTarget()) {
                reqTranslation->add_target(target.toStdString());
            }
            for (auto example: translation->getExamples()) {
                reqTranslation->add_example(example.toStdString());
            }
        }
    }

    authenticationManager_->proposeUserTranslation(call);
    return call->id_;
}

quint32 NetworkManager::authorize(QString login, QString password, quint32 timeoutMills) {
    AuthorizeAsyncCall *call = new AuthorizeAsyncCall(++currentId_, timeoutMills);
    call->request_.set_login(login.toStdString());
    call->request_.set_passwordhash(password.toStdString()); // pass hash insted of raw password

    authenticationManager_->authorize(call);
    return call->id_;
}

quint32 NetworkManager::registrate(QString login, QString password, quint32 timeoutMills) {
    RegistrateAsyncCall *call = new RegistrateAsyncCall(++currentId_, timeoutMills);
    call->request_.set_login(login.toStdString());
    call->request_.set_passwordhash(password.toStdString()); // pass hash insted of raw password

    authenticationManager_->registrate(call);
    return call->id_;
}
