#include <QtDebug>

#include "roosha_service_connector.h"
#include "server_response.h"
#include "Proto/roosha_service.grpc.pb.h"

RooshaServiceConnector::RooshaServiceConnector(AuthenticationManager *m) :
    responseListener_(new AsyncRpcResponseListener(this)),
    authManager_(m) {

    auto channel = grpc::CreateChannel("localhost:1543", grpc::InsecureChannelCredentials());
    stub_ = roosha::RooshaService::NewStub(channel);

    QObject::connect(responseListener_, &AsyncRpcResponseListener::finished,
                     responseListener_, &AsyncRpcResponseListener::deleteLater);
    responseListener_->start();
}

RooshaServiceConnector::~RooshaServiceConnector() {
//    responseListener_->quit();
    responseListener_->wait();
//    delete responseListener_;
}

void RooshaServiceConnector::translate(TranslateAsyncCall *call) {
    auto responseReader = stub_->Asynctranslate(&call->context_, call->request_, &completionQueue_);
    responseReader->Finish(&call->response_, &call->status_, call);
}

void RooshaServiceConnector::proposeUserTranslation(ProposeUserTranslationsAsyncCall *call) {
    auto responseReader = stub_->AsyncproposeUserTranslations(&call->context_, call->request_, &completionQueue_);
    responseReader->Finish(&call->response_, &call->status_, call);
}

void RooshaServiceConnector::authorize(AuthorizeAsyncCall *call) {
    auto responseReader = stub_->Asyncauthorize(&call->context_, call->request_, &completionQueue_);
    responseReader->Finish(&call->response_, &call->status_, call);
}

void RooshaServiceConnector::registrate(RegistrateAsyncCall *call) {
    auto responseReader = stub_->Asyncregistrate(&call->context_, call->request_, &completionQueue_);
    responseReader->Finish(&call->response_, &call->status_, call);
}

void RooshaServiceConnector::receiveResponse(RpcAsyncCall *call) {
    call->verify(authManager_);
}

AsyncRpcResponseListener::AsyncRpcResponseListener(RooshaServiceConnector *r) :
    connector_(r) {}

void AsyncRpcResponseListener::run() {
    void *callLabel;
    bool ok;
    while (connector_->completionQueue_.Next(&callLabel, &ok)) {
        qDebug() << "received response\n";
        auto call = static_cast<RpcAsyncCall*>(callLabel);
        call->receive(connector_);
    }
}
