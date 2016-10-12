#include "translation_service.h"

#include <grpc/grpc.h>
#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>

using grpc::ClientContext;
using grpc::Status;

using roosha::translation::Translation;
using roosha::translation::Translations;
using roosha::translation::TranslationRequest;
using roosha::translation::RooshaTranslationService;

TranslationService::TranslationService(const grpc::string &target) {
    std::shared_ptr<grpc::Channel> channel =  grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    stub_ = RooshaTranslationService::NewStub(channel);
}

Translations&& TranslationService::translate(QString source) {
    ClientContext clientContext;

    TranslationRequest request;
    request.set_source(source.toStdString());

    Translations response;
    Status status = stub_->translate(&clientContext, request, &response);
    if (status.ok()) {
        return std::move(response);
    }
    throw std::exception();
}

void TranslationService::proposeUserTranslation(Translation userTranslation) {
    ClientContext ClientContext;

    roosha::commons::Void response;

    Status status = stub_->proposeUserTranslation(ClientContext, userTranslation, &response);

    if (!status.ok()) {
        throw std::exception();
    }
}
