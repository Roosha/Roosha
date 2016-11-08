#include <QSharedPointer>
#include <QMutexLocker>

#include <grpc++/grpc++.h>

#include "Helpers/protobuf_converter.h"
#include "authentication_manager.h"
#include "roosha_service_connector.h"
#include "network_manager.h"

using namespace ProtobufConverter;

const grpc::string AuthenticationManager::TOKEN_METADATA_KEY = "roosha-auth-token";
const quint32 AuthenticationManager::TECHNICAL_REQUEST_ID = 0u;

AuthenticationManager::AuthenticationManager(NetworkManager *n) :
    state_(AWAIT_CREDENTIALS),
    connector_(new RooshaServiceConnector(this)),
    netManager_(n) {
}

void AuthenticationManager::authorizeOrRegistrate(AuthorizeOrRegistrateAsyncCall *call) {
    QMutexLocker lock(&stateMutex_);

    switch (state_) {
    case AWAIT_CREDENTIALS:
    case AUTHENTICATED:
        state_ = AWAIT_AUTHENTICATION;
        token_ = "";
        call->send(connector_);
        break;
    case AWAIT_AUTHENTICATION:
        call->fail(netManager_, RPCErrorStatus::ALREADY_IN_AUTHNTICATION_PROCESS);
        break;
    }
}


void AuthenticationManager::sendWithMetadata(AuthenticatedAsyncCall *call) {
    QMutexLocker lock(&stateMutex_);

    switch (state_) {
    case AUTHENTICATED:
        call->context_.AddMetadata(TOKEN_METADATA_KEY, token_);
        call->send(connector_);
        break;
    case AWAIT_AUTHENTICATION:
        callsQueue_.enqueue(call);
        break;
    case AWAIT_CREDENTIALS:
        call->fail(netManager_, RPCErrorStatus::NOT_AUTHENTICATED);
        break;
    }
}

void AuthenticationManager::receiveAuthenticatedCall(AuthenticatedAsyncCall *call) {
    if (call->status_.ok()) {
        call->succeed(netManager_);
        return;
    } else if (call->status_.error_code() != grpc::StatusCode::UNAUTHENTICATED) {
        call->fail(netManager_);
    }

    QMutexLocker lock(&stateMutex_);
    switch (state_) {
    case AuthenticationManager::AUTHENTICATED:
        if (tryToUpdateToken()) {
            callsQueue_.enqueue(call);
        } else {
            call->fail(netManager_, RPCErrorStatus::NOT_AUTHENTICATED);
        }
        break;
    case AuthenticationManager::AWAIT_AUTHENTICATION:
        callsQueue_.enqueue(call);
        break;
    case AuthenticationManager::AWAIT_CREDENTIALS:
        call->fail(netManager_, RPCErrorStatus::NOT_AUTHENTICATED);
        break;
    }
}

void AuthenticationManager::receiveAuthorizeOrRegistrateResponse(AuthorizeOrRegistrateAsyncCall *call) {
    processAuthorizeOrRegitrateResponse(call);

    if (call->id_ != TECHNICAL_REQUEST_ID) {
        if (call->status_.ok()) {
            call->succeed(netManager_);
        } else {
            call->fail(netManager_);
        }
    }
}

bool AuthenticationManager::tryToUpdateToken() {
    token_ = "";
    // TODO: look for credentials and send authorization request.
    return false;
}

void AuthenticationManager::processAuthorizeOrRegitrateResponse(AuthorizeOrRegistrateAsyncCall *call) {
    QMutexLocker lock(&stateMutex_);
    if (state_ != AWAIT_AUTHENTICATION) {
        qWarning("Illegal state %d when received AuthorizeOrRegistrateAsyncCall with id %du. Ignore it", state_, call->id_);
        return;
    }

    if (call->status_.ok()) {
        state_ = AUTHENTICATED;
        token_ = call->response_.token();
        for (auto curCall: callsQueue_) {
            curCall->send(connector_);
        }
    } else {
        state_ = AWAIT_CREDENTIALS;
        token_ = "";
        for (auto curCall: callsQueue_) {
            curCall->fail(netManager_);
        }
    }
}
