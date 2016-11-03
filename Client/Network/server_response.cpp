#include <QtDebug>

#include "server_response.h"


ServerResponse::ServerResponse(RooshaRpcService *rpcService):
    rpcService_(rpcService) {
}

void TranslateResponse::process() {
    if (status_.ok()) {
        rpcService_->emitTranslationSucceeded(id_, translations_);
    } else {
        rpcService_->emitTranslationFailed(id_, status_);
    }
}

void ProposeUserTranslationsResponse::process() {
    if (status_.ok()) {
        rpcService_->emitUserTranslationProposalSucceeded(id_, response_);
    } else {
        rpcService_->emitUserTranslationProposalFailed(id_, status_);
    }
}
