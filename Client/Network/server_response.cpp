#include <QtDebug>

#include <grpc++/grpc++.h>

#include "server_response.h"
#include "authentication_manager.h"


void ProposeUserTranslationsAsyncCall::emitSucceded() {
    rpcService_->emitUserTranslationProposalSucceeded(id_, response_);
}

void ProposeUserTranslationsAsyncCall::emitFailure() {
    rpcService_->emitUserTranslationProposalFailed(id_, status_);
}

void ProposeUserTranslationsAsyncCall::retry() {
    rpcService_->sendUserTranslationProposal(this);
}

void TranslateAsyncCall::emitSucceded() {
    rpcService_->emitTranslationSucceeded(id_, response_);
}

void TranslateAsyncCall::emitFailure() {
    rpcService_->emitTranslationFailed(id_, status_);
}

void TranslateAsyncCall::retry() {
    rpcService_->sendTranslateRequest(this);
}

void GeneralPurposeAsyncCall::handle() {
    if (isAuthenticationFailed()) {
        authManager_->handle(this);
    } else if (status_.ok()) {
        emitSucceded();
    } else {
        emitFailure();
    }
}

bool RpcAsyncCall::isAuthenticationFailed() {
    return status_.error_code() == grpc::UNAUTHENTICATED;
}

void RegistrateAsyncCall::handle() {
    authManager_->handle(this);
}

void AuthorizeAsyncCall::handle() {
    authManager_->handle(this);
}
