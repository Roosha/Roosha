#include <QtDebug>

#include <grpc++/grpc++.h>

#include "server_response.h"
#include "authentication_manager.h"
#include "roosha_service_connector.h"
#include "Helpers/protobuf_converter.h"
#include "network_manager.h"

using namespace ProtobufConverter;

void AuthorizeAsyncCall::send(RooshaServiceConnector *connector) {
    connector->authorize(this);
}

void AuthorizeAsyncCall::succeed(NetworkManager *netManager) {
    emit netManager->successAuthorize(id_);
}

void AuthorizeAsyncCall::fail(NetworkManager *netManager, RPCErrorStatus status) {
    emit netManager->failAuthorize(id_, status);
}

void RegistrateAsyncCall::send(RooshaServiceConnector *connector) {
    connector->registrate(this);
}

void RegistrateAsyncCall::succeed(NetworkManager *netManager) {
    emit netManager->successRegistrate(id_);
}

void RegistrateAsyncCall::fail(NetworkManager *netManager, RPCErrorStatus status) {
    emit netManager->failRegistrate(id_, status);
}

void TranslateAsyncCall::send(RooshaServiceConnector *connector) {
    connector->translate(this);
}

void TranslateAsyncCall::succeed(NetworkManager *netManager) {
    emit netManager->successTranslate(id_, translationsFromProtobuf(response_));
}

void TranslateAsyncCall::fail(NetworkManager *netManager, RPCErrorStatus status) {
    netManager->failTranslate(id_, status);
}

void ProposeUserTranslationsAsyncCall::send(RooshaServiceConnector *connector) {
    connector->proposeUserTranslation(this);
}

void ProposeUserTranslationsAsyncCall::succeed(NetworkManager *netManager) {
    emit netManager->successAuthorize(id_);
}

void ProposeUserTranslationsAsyncCall::fail(NetworkManager *netManager, RPCErrorStatus status) {
    netManager->failPropose(id_, status);
}

RpcAsyncCall::RpcAsyncCall(quint32 id, quint32 timeoutMillis): id_(id) {
    context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));
}

RpcAsyncCall::~RpcAsyncCall() {

}

void RpcAsyncCall::receive(RooshaServiceConnector *connector) {
    connector->receiveResponse(this);
}

void RpcAsyncCall::fail(NetworkManager *netManager) {
    this->fail(netManager, errorStatusFromGrpc(status_));
}

void AuthorizeOrRegistrateAsyncCall::authenticate(AuthenticationManager *authManager) {
    authManager->authorizeOrRegistrate(this);
}

void AuthorizeOrRegistrateAsyncCall::verify(AuthenticationManager *authManager) {
    authManager->receiveAuthorizeOrRegistrateResponse(this);
}

void AuthenticatedAsyncCall::authenticate(AuthenticationManager *authManager) {
    authManager->sendWithMetadata(this);
}

void AuthenticatedAsyncCall::verify(AuthenticationManager *authManager) {
    authManager->receiveAuthenticatedCall(this);
}
