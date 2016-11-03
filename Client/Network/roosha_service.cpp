#include "roosha_service.h"
#include "server_response.h"
#include "authentication_manager.h"

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
using roosha::translation::RooshaService;
using roosha::translation::UserTranslationsProposal;

RooshaRpcService::RooshaRpcService(const grpc::string &target):
    requestIdCount_(0),
    authManager_(new AuthenticationManager) {
    std::shared_ptr<grpc::Channel> channel =  grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    stub_ = RooshaService::NewStub(channel);

    // run thread which will wait for translation service responces and
    rpcStatusListener_ = new AsyncRpcStatusListener;
    rpcStatusListener_->setTranslationService(this);
    QObject::connect(rpcStatusListener_, &AsyncRpcStatusListener::finished, rpcStatusListener_, &QObject::deleteLater);
    rpcStatusListener_->start();
}

RooshaRpcService::~RooshaRpcService() {
    rpcStatusListener_->quit();
    rpcStatusListener_->wait();
    delete rpcStatusListener_;
    delete authManager_;
}


quint32 RooshaRpcService::translate(QString source, quint32 timeoutMillis) {
    TranslateAsyncCall* call = new TranslateAsyncCall(this, authManager_);
    call->request_.set_source(source.toStdString());
    call->id_ = requestIdCount_++;
    call->context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));

    sendTranslateRequest(call);
    return call->id_;
}

quint32 RooshaRpcService::proposeUserTranslations(Translations userTranslation, quint32 timeoutMillis) {
    ProposeUserTranslationsAsyncCall* call = new ProposeUserTranslationsAsyncCall(this, authManager_);
    call->id_ = requestIdCount_++;
    call->context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));

    // TODO: figure out, whether doest 'set_allocated_*' free allocated message automatically or not.
    call->request_.set_allocated_proposedtranslations(&userTranslation);

    sendUserTranslationProposal(call);
    return call->id_;
}

void RooshaRpcService::emitTranslationSucceeded(quint32 id, Translations translations) {
    emit translationSucceeded(id, translations);
}

void RooshaRpcService::emitTranslationFailed(quint32 id, Status status) {
    emit translationFailed(id, status);
}

void RooshaRpcService::sendTranslateRequest(TranslateAsyncCall *call) {
    auto responseReader = stub_->Asynctranslate(&call->context_, call->request_, &completionQueue_);
    responseReader->Finish(static_cast<Translations*>(&call->response_), &call->status_, static_cast<void*>(call));
}

void RooshaRpcService::emitUserTranslationProposalSucceeded(quint32 requestId, roosha::commons::Void response) {
    emit userTranslationProposalSucceeded(requestId, response);
}

void RooshaRpcService::emitUserTranslationProposalFailed(quint32 requestId, grpc::Status status) {
    emit userTranslationProposalFailed(requestId, status);
}

void RooshaRpcService::sendUserTranslationProposal(ProposeUserTranslationsAsyncCall *call) {
    auto responseReader = stub_->AsyncproposeUserTranslations(&call->context_, call->request_, &completionQueue_);
    responseReader->Finish(static_cast<Void*>(&call->response_), &call->status_, static_cast<void*>(call));
}

void AsyncRpcStatusListener::setTranslationService(RooshaRpcService* translationService) {
    rpcService_ = translationService;
}

void AsyncRpcStatusListener::run() {
    void* label;
    bool ok;
    while (rpcService_->completionQueue_.Next(&label, &ok)) {
        auto response = static_cast<RpcAsyncCall*>(label);
        response->handle();
        //TODO: here is a memory leak. Need something to do with it.
//        delete response;
    }
}
