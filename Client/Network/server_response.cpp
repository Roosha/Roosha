#include <QtDebug>

#include <grpc++/grpc++.h>

#include "server_response.h"
#include "authentication_manager.h"
#include "roosha_service_connector.h"


void AuthorizeAsyncCall::receive(RooshaServiceConnector *connector) {
    connector->receiveAuthorizeResponse(this);
}

void RegistrateAsyncCall::receive(RooshaServiceConnector *connector) {
    connector->receiveRegistrateResponse(this);
}

void TranslateAsyncCall::receive(RooshaServiceConnector *connector) {
    connector->receiveTranslateResponse(this);
}

void ProposeUserTranslationsAsyncCall::receive(RooshaServiceConnector *connector) {
    connector->receiveProposeUserTranslationResponse(this);
}

RpcAsyncCall::RpcAsyncCall(quint32 id, quint32 timeoutMillis): id_(id) {
    context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));
}

RpcAsyncCall::~RpcAsyncCall() {

}

RPCErrorStatus RpcAsyncCall::getStatus() {
    switch (status_.error_code()) {
    case grpc::StatusCode::DEADLINE_EXCEEDED:
        return DEADLINE_EXCEEDED;
    case grpc::StatusCode::UNAUTHENTICATED:
        if (status_.error_message() == "Failed to authorize user, most likely due to incorrect credentials.") {
            return AUTHORIZATION_ERROR;
        } else if (status_.error_message() == "Failed to register new user, most likely due to login is already used.") {
            return REGISTRATION_ERROR;
        } else if (status_.error_message() == "Specified token is expired, call 'register' or 'authorize' rpc to get new valid token.") {
            return EXPIRED_TOKEN;
        } else {
            goto unknown;
        }
    default:
        goto unknown;
    }

unknown:
    qWarning("unexpected grpc status: %d\nerror message: %s", status_.error_code(), status_.error_message().c_str());
    return UNKNOWN;
}
