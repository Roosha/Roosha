#include "roosha_service.h"
#include "server_response.h"

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
    requestIdCount_(0) {
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
}


quint32 RooshaRpcService::translate(QString source, quint32 timeoutMillis) {
    TranslationRequest request;
    request.set_source(source.toStdString());

    TranslateResponse* response = new TranslateResponse(this);
    response->id_ = requestIdCount_++;
    response->context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));

    auto responseReader = stub_->Asynctranslate(&response->context_, request, &completionQueue_);
    responseReader->Finish(static_cast<Translations*>(&response->translations_), &response->status_, static_cast<void*>(response));

    return response->id_;
}

quint32 RooshaRpcService::proposeUserTranslations(Translations userTranslation, quint32 timeoutMillis) {
    ProposeUserTranslationsResponse* response = new ProposeUserTranslationsResponse(this);
    response->id_ = requestIdCount_++;
    response->context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));

    UserTranslationsProposal request;
    // TODO: figure out, whether doest 'set_allocated_*' free allocated message automatically or not.
    request.set_allocated_proposedtranslations(&userTranslation);
    auto responseReader = stub_->AsyncproposeUserTranslations(&response->context_, request, &completionQueue_);
    responseReader->Finish(static_cast<Void*>(&response->response_), &response->status_, static_cast<void*>(response));
    return response->id_;
}

void RooshaRpcService::emitTranslationSucceeded(quint32 id, Translations translations) {
    emit translationSucceeded(id, translations);
}

void RooshaRpcService::emitTranslationFailed(quint32 id, Status status) {
    emit translationFailed(id, status);
}

void RooshaRpcService::emitUserTranslationProposalSucceeded(quint32 requestId, roosha::commons::Void response) {
    emit userTranslationProposalSucceeded(requestId, response);
}

void RooshaRpcService::emitUserTranslationProposalFailed(quint32 requestId, grpc::Status status) {
    emit userTranslationProposalFailed(requestId, status);
}

void AsyncRpcStatusListener::setTranslationService(RooshaRpcService* translationService) {
    rpcService_ = translationService;
}

void AsyncRpcStatusListener::run() {
    void* label;
    bool ok;
    while (rpcService_->completionQueue_.Next(&label, &ok)) {
        auto response = static_cast<ServerResponse*>(label);
        response->process();
        delete response;
    }
}
