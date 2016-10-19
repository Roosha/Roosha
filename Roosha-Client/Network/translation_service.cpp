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

using roosha::translation::Translation;
using roosha::translation::Translations;
using roosha::translation::TranslationRequest;
using roosha::translation::RooshaTranslationService;

TranslationService::TranslationService(const grpc::string &target) {
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
    static quint32 currentId = 0;

    TranslationRequest request;
    request.set_source(source.toStdString());

    AsyncClientCall<Translations>* call = new AsyncClientCall<Translations>;
    call->id_ = currentId++;
    call->context_.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis));
    call->responseReader_ = stub_->Asynctranslate(&call->context_, request, &completionQueue_);
    call->responseReader_->Finish(&call->response_, &call->status_, static_cast<void*>(call));

    return call->id_;
}

void TranslationService::proposeUserTranslations(Translations userTranslation) {
    ClientContext context;
    //TODO: check if skipping of response reader produces any error.
    stub_->AsyncproposeUserTranslations(&context, userTranslation, &completionQueue_);
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
    void* call;
    bool ok;
    while (translationService_->completionQueue_.Next(&call, &ok)) {
        // TODO: make possible to receive different message types via union or something like that.
        auto translateResponse = static_cast<AsyncClientCall<Translations>*>(call);
        if (translateResponse->status_.ok()) {
//            std::cout << "emit translationSucceeded" << std::endl;
            translationService_->emitTranslationSucceeded(translateResponse->id_, translateResponse->response_);
        }
        else {
//            std::cout << "emit translationFaled:" << std::endl <<
//                         "id = " << translateResponse->id_ << std::endl <<
//                         "error_code = " << translateResponse->status_.error_code() << std::endl <<
//                         "error_message = " << translateResponse->status_.error_message() << std::endl;
            translationService_->emitTranslationFailed(translateResponse->id_, translateResponse->status_);
        }
        delete translateResponse;
    }
}
