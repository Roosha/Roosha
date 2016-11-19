#include <QtDebug>

#include <grpc++/grpc++.h>

#include "server_response.h"
#include "authentication_manager.h"
#include "roosha_service_connector.h"
#include "Helpers/protobuf_converter.h"
#include "network_manager.h"

using namespace ProtobufConverter;

#ifndef DEBUG
#define DEBUG(methodName)\
    qDebug("RPC call %d: '%s' called", id_, (methodName));
#else
#error DEBUG macro is already defined
#endif // DEBUG

void AuthorizeAsyncCall::send(RooshaServiceConnector *connector) {
    DEBUG("AuthorizeAsyncCall::send");
    connector->authorize(this);
}

void AuthorizeAsyncCall::succeed(NetworkManager *netManager) {
    DEBUG("AuthorizeAsyncCall::succeed");
    emit netManager->successAuthorize(id_);
    delete this;
}

void AuthorizeAsyncCall::fail(NetworkManager *netManager, RPCErrorStatus status) {
    DEBUG("AuthorizeAsyncCall::fail");
    emit netManager->failAuthorize(id_, status);
    delete this;
}

void RegistrateAsyncCall::send(RooshaServiceConnector *connector) {
    DEBUG("RegistrateAsyncCall::send");
    connector->registrate(this);
}

void RegistrateAsyncCall::succeed(NetworkManager *netManager) {
    DEBUG("RegistrateAsyncCall::succeed");
    emit netManager->successRegistrate(id_);
    delete this;
}

void RegistrateAsyncCall::fail(NetworkManager *netManager, RPCErrorStatus status) {
    DEBUG("RegistrateAsyncCall::fail");
    emit netManager->failRegistrate(id_, status);
    delete this;
}

void TranslateAsyncCall::send(RooshaServiceConnector *connector) {
    DEBUG("TranslateAsyncCall::send");
    connector->translate(this);
}

void TranslateAsyncCall::succeed(NetworkManager *netManager) {
    DEBUG("TranslateAsyncCall::succeed");
    emit netManager->successTranslate(id_, translationsFromProtobuf(response_));
    delete this;
}

void TranslateAsyncCall::fail(NetworkManager *netManager, RPCErrorStatus status) {
    DEBUG("TranslateAsyncCall::fail");
    netManager->failTranslate(id_, status);
    delete this;
}

void ProposeUserTranslationsAsyncCall::send(RooshaServiceConnector *connector) {
    DEBUG("ProposeUserTranslationsAsyncCall::send")
    connector->proposeUserTranslation(this);
}

void ProposeUserTranslationsAsyncCall::succeed(NetworkManager *netManager) {
    DEBUG("ProposeUserTranslationsAsyncCall::succeed");
    emit netManager->successAuthorize(id_);
    delete this;
}

void ProposeUserTranslationsAsyncCall::fail(NetworkManager *netManager, RPCErrorStatus status) {
    DEBUG("ProposeUserTranslationsAsyncCall::fail")
    netManager->failPropose(id_, status);
    delete this;
}

RpcAsyncCall::RpcAsyncCall(quint32 id, quint32 timeoutMillis): id_(id) {
    qDebug("Create RPC call with id: %d", id_);
    context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));
}

RpcAsyncCall::~RpcAsyncCall() {
    qDebug("RPC call %d: DESTROY", id_);
}

void RpcAsyncCall::receive(RooshaServiceConnector *connector) {
    DEBUG("RpcAsyncCall::receive")
    connector->receiveResponse(this);
}

void RpcAsyncCall::fail(NetworkManager *netManager) {
    DEBUG("default RpcAsyncCall::fail")
    this->fail(netManager, errorStatusFromGrpc(status_));
    // DO NOT DELETE THIS HERE!
}

void AuthorizeOrRegistrateAsyncCall::authenticate(AuthenticationManager *authManager) {
    DEBUG("AuthorizeOrRegistrateAsyncCall::authenticate")
    authManager->authorizeOrRegistrate(this);
}

void AuthorizeOrRegistrateAsyncCall::verify(AuthenticationManager *authManager) {
    DEBUG("AuthorizeOrRegistrateAsyncCall::verify")
    authManager->receiveAuthorizeOrRegistrateResponse(this);
}

void AuthenticatedAsyncCall::authenticate(AuthenticationManager *authManager) {
    DEBUG("AuthenticatedAsyncCall::authenticate")
    authManager->sendWithMetadata(this);
}

void AuthenticatedAsyncCall::verify(AuthenticationManager *authManager) {
    DEBUG("AuthenticatedAsyncCall::verify")
    authManager->receiveAuthenticatedCall(this);
}

void SaveChangesAsyncCall::send(RooshaServiceConnector *connector) {
    DEBUG("SaveChangesAsyncCall::send")
    connector->saveChanges(this);
}

void SaveChangesAsyncCall::succeed(NetworkManager *netManager) {
    DEBUG("SaveChangesAsyncCall::succeed")
    netManager->successSaveChanges(id_);
    delete this;
}

void SaveChangesAsyncCall::fail(NetworkManager *netManager, RPCErrorStatus status) {
    DEBUG("SaveChangesAsyncCall::fail")
    netManager->failSaveChanges(id_, status);
    delete this;
}


void LoadChangesAsyncCall::send(RooshaServiceConnector *connector) {
    DEBUG("LoadChangesAsyncCall::send")
    connector->loadChanges(this);
}

void LoadChangesAsyncCall::succeed(NetworkManager *netManager) {
    DEBUG("LoadChangesAsyncCall::succeed")
    netManager->successLoadChanges(id_, response_);
    delete this;
}

void LoadChangesAsyncCall::fail(NetworkManager *netManager, RPCErrorStatus status) {
    DEBUG("LoadChangesAsyncCall::fail")
    netManager->failLoadChanges(id_, status);
    delete this;
}

#ifdef DEBUG
#undef DEBUG
#else
#error DEBUG macro is not defined
#endif // DEBUG
