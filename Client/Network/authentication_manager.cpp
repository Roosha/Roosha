#include <QSharedPointer>

#include "authentication_manager.h"
#include "roosha_service_connector.h"
#include "network_manager.h"

AuthenticationManager::AuthenticationManager(NetworkManager *n) :
    connector_(new RooshaServiceConnector(this)),
    netManager_(n) {
}

void AuthenticationManager::translate(TranslateAsyncCall *call) {
    connector_->translate(call);
}

void AuthenticationManager::proposeUserTranslation(ProposeUserTranslationsAsyncCall *call) {
    connector_->proposeUserTranslation(call);
}

void AuthenticationManager::authorize(AuthorizeAsyncCall *call) {
    connector_->authorize(call);
}

void AuthenticationManager::registrate(RegistrateAsyncCall *call) {
    connector_->registrate(call);
}

void AuthenticationManager::receiveTranslateResponse(TranslateAsyncCall *call) {
    if (call->status_.ok()) {
        Translations result;
        for (int i = 0; i < call->response_.translation_size(); i++) {
            auto raw_translation = call->response_.translation(i);
            Translation *translation = new Translation;
            translation->setSource(QString::fromStdString(call->response_.source()));

            QStringList targets;
            for (int i = 0; i < raw_translation.target_size(); i++) {
                targets.append(QString::fromStdString(raw_translation.target(i)));
            }
            translation->setTarget(targets);

            QStringList examples;
            for (int i = 0; i < raw_translation.example_size(); i++) {
                examples.append(QString::fromStdString(raw_translation.example(i)));
            }
            translation->setExamples(examples);

            result.append(QSharedPointer<Translation>(translation));
        }
        emit netManager_->successTranslate(call->id_, result);
    } else {
        emit netManager_->failTranslate(call->id_, call->getStatus());
    }
}

void AuthenticationManager::receiveProposeUserTranslationResponse(ProposeUserTranslationsAsyncCall *call) {
    if (call->status_.ok()) {
        emit netManager_->successPropose(call->id_);
    } else {
        emit netManager_->failPropose(call->id_, call->getStatus());
    }
}

void AuthenticationManager::receiveAuthorizeResponse(AuthorizeAsyncCall *call) {
    if (call->status_.ok()) {
        emit netManager_->successAuthorize(call->id_);
    } else {
        emit netManager_->failAuthorize(call->id_, call->getStatus());
    }
}

void AuthenticationManager::receiveRegistrateResponse(RegistrateAsyncCall *call) {
    if (call->status_.ok()) {
        emit netManager_->successRegistrate(call->id_);
    } else {
        emit netManager_-> failRegistrate(call->id_, call->getStatus());
    }
}
