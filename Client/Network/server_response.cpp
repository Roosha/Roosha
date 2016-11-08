#include <QtDebug>

#include <grpc++/grpc++.h>

#include "server_response.h"
#include "authentication_manager.h"
#include "roosha_service_connector.h"
#include "Helpers/protobuf_converter.h"
#include "network_manager.h"

using namespace ProtobufConverter;

void AuthorizeAsyncCall::receive(RooshaServiceConnector *connector) {
    connector->receiveAuthorizeResponse(this);
}

void AuthorizeAsyncCall::send(RooshaServiceConnector *connector) {
    connector->authorize(this);
}

void AuthorizeAsyncCall::succeed(NetworkManager *netManager) {
    emit netManager->successAuthorize(id_);
}

void AuthorizeAsyncCall::fail(NetworkManager *netManager) {
    emit netManager->failAuthorize(id_, errorStatusFromGrpc(status_));
}

void RegistrateAsyncCall::receive(RooshaServiceConnector *connector) {
    connector->receiveRegistrateResponse(this);
}

void RegistrateAsyncCall::send(RooshaServiceConnector *connector) {
    connector->registrate(this);
}

void RegistrateAsyncCall::succeed(NetworkManager *netManager) {
    emit netManager->successRegistrate(id_);
}

void RegistrateAsyncCall::fail(NetworkManager *netManager) {
    emit netManager->failRegistrate(id_, errorStatusFromGrpc(status_));
}

void TranslateAsyncCall::receive(RooshaServiceConnector *connector) {
    connector->receiveTranslateResponse(this);
}

void TranslateAsyncCall::send(RooshaServiceConnector *connector) {
    connector->translate(this);
}

void TranslateAsyncCall::succeed(NetworkManager *netManager) {
    emit netManager->successTranslate(id_, translationsFromProtobuf(response_));
}

void TranslateAsyncCall::fail(NetworkManager *netManager) {
    emit netManager->failTranslate(id_, errorStatusFromGrpc(status_));
}

void ProposeUserTranslationsAsyncCall::receive(RooshaServiceConnector *connector) {
    connector->receiveProposeUserTranslationResponse(this);
}

void ProposeUserTranslationsAsyncCall::send(RooshaServiceConnector *connector) {
    connector->proposeUserTranslation(this);
}

void ProposeUserTranslationsAsyncCall::succeed(NetworkManager *netManager) {
    emit netManager->successAuthorize(id_);
}

void ProposeUserTranslationsAsyncCall::fail(NetworkManager *netManager) {
    emit netManager->failPropose(id_, errorStatusFromGrpc(status_));
}

RpcAsyncCall::RpcAsyncCall(quint32 id, quint32 timeoutMillis): id_(id) {
    context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));
}

RpcAsyncCall::~RpcAsyncCall() {

}
