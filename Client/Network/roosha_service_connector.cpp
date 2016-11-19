#include <QtDebug>
#include <QtConcurrent>

#include "roosha_service_connector.h"
#include "server_response.h"
#include "Proto/roosha_service.grpc.pb.h"
#include "Helpers/protobuf_converter.h"

#ifndef DEBUG_CALL
#define DEBUG_CALL(name)\
    qDebug("RooshaServiceConnector::%s: send call %d", (name), call->id_);
#else
#error DEBUG_CALL macro is already defined
#endif // DEBUG_CALL

using ProtobufConverter::changeFromProtobuf;

RooshaServiceConnector::RooshaServiceConnector(AuthenticationManager *m) :
    responseListener_(new AsyncRpcResponseListener(this)),
    authManager_(m) {
    qDebug("Create RooshaServiceConnector");

    auto channel = grpc::CreateChannel("localhost:1543", grpc::InsecureChannelCredentials());
    stub_ = roosha::RooshaService::NewStub(channel);

    QObject::connect(responseListener_, &AsyncRpcResponseListener::finished,
                     responseListener_, &AsyncRpcResponseListener::deleteLater);

    responseListener_->setParent(this);
    responseListener_->start();
}

RooshaServiceConnector::~RooshaServiceConnector() {
    qDebug("Destroy RooshaServiceConnector");
}

void RooshaServiceConnector::translate(TranslateAsyncCall *call) {
    DEBUG_CALL("translate")
    auto responseReader = stub_->Asynctranslate(&call->context_, call->request_, &completionQueue_);
    responseReader->Finish(&call->response_, &call->status_, call);
}

void RooshaServiceConnector::proposeUserTranslation(ProposeUserTranslationsAsyncCall *call) {
    DEBUG_CALL("proposeUserTranslation")
    auto responseReader = stub_->AsyncproposeUserTranslations(&call->context_, call->request_, &completionQueue_);
    responseReader->Finish(&call->response_, &call->status_, call);
}

void RooshaServiceConnector::authorize(AuthorizeAsyncCall *call) {
    DEBUG_CALL("authorize")
    auto responseReader = stub_->Asyncauthorize(&call->context_, call->request_, &completionQueue_);
    responseReader->Finish(&call->response_, &call->status_, call);
}

void RooshaServiceConnector::registrate(RegistrateAsyncCall *call) {
    DEBUG_CALL("registrate")
    auto responseReader = stub_->Asyncregistrate(&call->context_, call->request_, &completionQueue_);
    responseReader->Finish(&call->response_, &call->status_, call);
}

void RooshaServiceConnector::saveChanges(SaveChangesAsyncCall *call) {
    DEBUG_CALL("saveChanges")
    QtConcurrent::run([call, this] {
        qDebug("Start client-stream request writer for call %d", call->id_);
        auto writer = stub_->saveChanges(&call->context_, &call->response_);
        for (const auto& change : call->request_) {
            if (!writer->Write(change)) {
                qDebug("Request %d stream writer: stream broken", call->id_);
                break;
            }
            qDebug("Request %d stream writer: write message", call->id_);
        }
        writer->WritesDone();
        call->status_ = writer->Finish();
        qDebug("Request %d stream writer: writes done", call->id_);
        call->verify(authManager_);
    });
}

void RooshaServiceConnector::loadChanges(LoadChangesAsyncCall *call) {
    DEBUG_CALL("loadChanges")
    QtConcurrent::run([call, this] {
        qDebug("Start server-stream response reader for call %d", call->id_);
        auto reader = stub_->loadChanges(&call->context_, call->request_);
        roosha::Change receivedChange;
        while (reader->Read(&receivedChange)) {
            qDebug("Request %d stream reader: read message", call->id_);
            call->response_.append(changeFromProtobuf(receivedChange));
        }
        call->status_ = reader->Finish();
        qDebug("Request %d stream reader: finish reading", call->id_);
        call->verify(authManager_);
    });
}

void RooshaServiceConnector::receiveResponse(RpcAsyncCall *call) {
    qDebug("RooshaServiceConnector::receiveResponse: receive response for call %d", call->id_);
    call->verify(authManager_);
}

AsyncRpcResponseListener::AsyncRpcResponseListener(RooshaServiceConnector *r) :
    connector_(r) {

    qDebug("Create AsyncRpcResponseListener");
}

void AsyncRpcResponseListener::run() {
    qDebug("AsyncRpcResponseListener::run: start listening");
    void *callLabel;
    bool ok;
    while (connector_->completionQueue_.Next(&callLabel, &ok)) {
        auto call = static_cast<RpcAsyncCall*>(callLabel);
        qDebug("AsyncRpcResponseListener::run: receive response for call %d", call->id_);
        call->receive(connector_);
    }
}

#ifdef DEBUG_CALL
#undef DEBUG_CALL
#else
#error DEBUG_CALL macro is not defined
#endif
