#include "translation_service.h"
#include "async_client_call.h"

#include <grpc/grpc.h>
#include <grpc++/grpc++.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

#include <QtConcurrent/qtconcurrentrun.h>
#include <QObject>

#include <thread>
#include <iostream>

using grpc::ClientContext;
using grpc::Status;

using roosha::commons::Void;
using roosha::translation::Translation;
using roosha::translation::Translations;
using roosha::translation::TranslationRequest;
using roosha::translation::RooshaTranslationService;

TranslationService::TranslationService(const grpc::string &target):
        requestIdCount_(0) {
    std::shared_ptr<grpc::Channel> channel =  grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    stub_ = RooshaTranslationService::NewStub(channel);

    // run thread which will wait for translation service responces and
    rpcStatusListener_ = new AsyncRpcStatusListener;
    rpcStatusListener_->setTranslationService(this);
    QObject::connect(rpcStatusListener_, &AsyncRpcStatusListener::finished, rpcStatusListener_, &QObject::deleteLater);
    rpcStatusListener_->start();
}

TranslationService::~TranslationService() {
    rpcStatusListener_->quit();
    rpcStatusListener_->wait();
    delete rpcStatusListener_;
}


quint32 TranslationService::translate(QString source, quint32 timeoutMillis) {
    TranslationRequest request;
    request.set_source(source.toStdString());

    AsyncClientCall* call = new AsyncClientCall;
    call->id_ = requestIdCount_++;
    call->requestType_ = TRANSLATE;
    call->response_ = new Translations;
    call->context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));

    auto responseReader = stub_->Asynctranslate(&call->context_, request, &completionQueue_);
    responseReader->Finish(static_cast<Translations*>(call->response_), &call->status_, static_cast<void*>(call));

    return call->id_;
}

quint32 TranslationService::proposeUserTranslations(Translations userTranslation, quint32 timeoutMillis) {
    AsyncClientCall* call = new AsyncClientCall;
    call->id_ = requestIdCount_++;
    call->requestType_ = PROPOSE_USER_TRANSLATION;
    call->response_ = new Void;
    call->context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));

    auto responseReader = stub_->AsyncproposeUserTranslations(&call->context_, userTranslation, &completionQueue_);
    responseReader->Finish(static_cast<Void*>(call->response_), &call->status_, static_cast<void*>(call));
    return call->id_;
}

void TranslationService::emitTranslationSucceeded(quint32 id, Translations translations) {
    emit translationSucceeded(id, translations);
}

void TranslationService::emitTranslationFailed(quint32 id, Status status) {
    emit translationFailed(id, status);
}


void AsyncRpcStatusListener::setTranslationService(TranslationService* translationService) {
    translationService_ = translationService;
}

void AsyncRpcStatusListener::run() {
    void* label;
    bool ok;
    while (translationService_->completionQueue_.Next(&label, &ok)) {
        auto call = static_cast<AsyncClientCall*>(label);
        if (call->status_.ok()) {
            switch (call->requestType_) {
                case TRANSLATE:
                    auto response = *static_cast<Translations*>(call->response_);
                    translationService_->emitTranslationSucceeded(call->id_, response);

            }
        }
        else {
            translationService_->emitTranslationFailed(call->id_, call->status_);
        }
        delete call;
    }
}
