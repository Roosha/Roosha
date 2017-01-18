#include <QSharedPointer>
#include <QMutexLocker>
#include <QCryptographicHash>

#include <grpc++/grpc++.h>

#include "Helpers/protobuf_converter.h"
#include "Helpers/configuremanager.h"
#include "authentication_manager.h"
#include "roosha_service_connector.h"
#include "network_manager.h"

#ifndef DEBUG_CALL
#define DEBUG_CALL(methodName)\
    qDebug("AuthenticationManager::%s: passed call %d", (methodName), call->id_);
#else
#error DEBUG_CALL macro is already defined
#endif

#ifndef DEBUG_QUEUE_PUT
#define DEBUG_QUEUE_PUT(methodName)\
    qDebug("AuthenticationManager::%s: put call %d to callsQueue", (methodName), call->id_);
#else
#error DEBUG_QUEUE_PUT macro is already defined
#endif

using namespace ProtobufConverter;

AuthenticationManager::AuthenticationManager(NetworkManager *n) :
        state_(AWAIT_CREDENTIALS),
        connector_(new RooshaServiceConnector(this)),
        netManager_(n) {

    qDebug("AuthenticationManager created");
}

AuthenticationManager::~AuthenticationManager() {
    qDebug("AuthenticationManager destroyed");
}

std::string AuthenticationManager::hashPassword(QString password) {
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha3_256).toHex().toStdString();
}

void AuthenticationManager::authorizeOrRegistrate(AuthorizeOrRegistrateAsyncCall *call) {
    DEBUG_CALL("authorizeOrRegistrate")
    QMutexLocker lock(&stateMutex_);

    switch (state_) {
        case AWAIT_CREDENTIALS:
        case AUTHENTICATED:setState(AWAIT_AUTHENTICATION);
            ConfigureManager::Instance().setToken("");
            call->send(connector_);
            break;
        case AWAIT_AUTHENTICATION:call->fail(netManager_, RPCErrorStatus::ALREADY_IN_AUTHNTICATION_PROCESS);
            break;
    }
}

void AuthenticationManager::sendWithMetadata(AuthenticatedAsyncCall *call) {
    DEBUG_CALL("sendWithMetadata")
    QMutexLocker lock(&stateMutex_);

    if (state_ == AUTHENTICATED) {
        call->context_.AddMetadata(TOKEN_METADATA_KEY, ConfigureManager::Instance().getToken().toStdString());
    }

    if (state_ == AUTHENTICATED || !call->isAuthenticationRequired()) {
        call->send(connector_);
    } else if (state_ == AWAIT_AUTHENTICATION) {
        DEBUG_QUEUE_PUT("AuthenticationManager::sendWithMetadata")
        callsQueue_.enqueue(call);
    } else if (state_ == AWAIT_CREDENTIALS) {
        call->fail(netManager_, RPCErrorStatus::NOT_AUTHENTICATED);
    } else {
        qWarning("Unexpected AuthenticationManager::state_ value: %d", state_);
    }
}

void AuthenticationManager::receiveAuthenticatedCall(AuthenticatedAsyncCall *call) {
    DEBUG_CALL("receiveAuthenticatedCall")
    if (call->status_.ok()) {
        call->succeed(netManager_);
        return;
    } else if (call->status_.error_code() != grpc::StatusCode::UNAUTHENTICATED) {
        call->fail(netManager_);
        return;
    }

    // -----------------Authentication error-------------
    QMutexLocker lock(&stateMutex_);
    qDebug("AuthenticationManager::receiveAuthenticatedCall: authentication error");
    switch (state_) {
        case AuthenticationManager::AUTHENTICATED:
            if (tryToUpdateToken()) {
                DEBUG_QUEUE_PUT("AuthenticationManager::receiveAuthenticatedCall")
                callsQueue_.enqueue(call);
            } else {
                call->fail(netManager_, RPCErrorStatus::NOT_AUTHENTICATED);
            }
            break;
        case AuthenticationManager::AWAIT_AUTHENTICATION:DEBUG_QUEUE_PUT(
                    "AuthenticationManager::receiveAuthenticatedCall")
            callsQueue_.enqueue(call);
            break;
        case AuthenticationManager::AWAIT_CREDENTIALS:call->fail(netManager_, RPCErrorStatus::NOT_AUTHENTICATED);
            break;
    }
}

void AuthenticationManager::receiveAuthorizeOrRegistrateResponse(AuthorizeOrRegistrateAsyncCall *call) {
    //TODO: handle network failures.
    DEBUG_CALL("receiveAuthorizeOrRegistrateResponse")
    processAuthorizeOrRegitrateResponse(call);

    if (call->id_ != TECHNICAL_AUTHENTICATION_RPC_ID) {
        if (call->status_.ok()) {
            call->succeed(netManager_);
        } else {
            call->fail(netManager_);
        }
    } else {
        delete call;
    }
}

void AuthenticationManager::setState(AuthenticationManager::State state) {
    switch (state) {
        case AuthenticationManager::AUTHENTICATED : qDebug("AuthenticationManager: toggle state to AUTHENTICATED");
            break;
        case AuthenticationManager::AWAIT_AUTHENTICATION:
            qDebug("AuthenticationManager: toggle state to AWAIT_AUTHENTICATION");
            break;
        case AuthenticationManager::AWAIT_CREDENTIALS :qDebug("AuthenticationManager: toggle state to AWAIT_CREDENTIALS");
            break;
    }
    state_ = state;
}

bool AuthenticationManager::tryToUpdateToken() {
    ConfigureManager::Instance().setToken("");
    std::string login = ConfigureManager::Instance().getLogin().toStdString();
    std::string passwordHash = ConfigureManager::Instance().getPasswordHash().toStdString();

    if (login != "") {
        qDebug("AuthenticationManager::tryToUpdateToken: try to authorize with credentials from config");
        auto call = new AuthorizeAsyncCall(TECHNICAL_AUTHENTICATION_RPC_ID, DEFAULT_TIMEOUT_MILLIS);
        setState(AWAIT_AUTHENTICATION);
        call->send(connector_);
        return true;
    } else {
        qDebug("AuthenticationManager::tryToUpdateToken: failed to take credentials from config");
        setState(AWAIT_CREDENTIALS);
        return false;
    }
}

void AuthenticationManager::processAuthorizeOrRegitrateResponse(AuthorizeOrRegistrateAsyncCall *call) {
    DEBUG_CALL("processAuthorizeOrRegitrateResponse");
    QMutexLocker lock(&stateMutex_);
    if (state_ != AWAIT_AUTHENTICATION) {
        qWarning("Illegal state %d when received AuthorizeOrRegistrateAsyncCall with id %du. Ignore it",
                 state_,
                 call->id_);
        return;
    }

    if (call->status_.ok()) {
        setState(AUTHENTICATED);
        ConfigureManager::Instance().setLogin(QString::fromStdString(call->request_.login()));
        ConfigureManager::Instance().setPasswordHash(QString::fromStdString(call->request_.passwordhash()));
        ConfigureManager::Instance().setToken(QString::fromStdString(call->response_.token()));

        for (auto curCall: callsQueue_) {
            qDebug("AuthenticationManager::processAuthorizeOrRegitrateResponse: retry call %d", call->id_);
            curCall->send(connector_);
        }
    } else {
        setState(AWAIT_CREDENTIALS);
        ConfigureManager::Instance().setToken("");
        for (auto curCall: callsQueue_) {
            qDebug("AuthenticationManager::processAuthorizeOrRegitrateResponse: fail call %d", call->id_);
            curCall->fail(netManager_);
        }
    }
}

#ifdef DEBUG_CALL
#undef DEBUG_CALL
#else
#error DEBUG_CALL macro is not defined
#endif // DEBUG_CALL

#ifdef DEBUG_QUEUE_PUT
#undef DEBUG_QUEUE_PUT
#else
#error DEBUG_QUEUE_PUT macro is not defined
#endif
