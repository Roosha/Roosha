#ifndef ASYNCCLIENTCALL_H
#define ASYNCCLIENTCALL_H
#include <memory>

#include <QSharedPointer>

#include <grpc++/grpc++.h>

#include "Proto/roosha_service.pb.h"
#include "Proto/commons.pb.h"
#include "roosha_service.h"

class RooshaServiceConnector;

/**
 * Base struct for all rpc data. It should be used to hold the all information about call in grpc CompletionQueue.
 */
struct RpcAsyncCall {
    // TODO: now rpcService and authManager are not singletones, so we pass them in constructor, but when we make them
    // singletones, this should be altered.
    RpcAsyncCall();
    virtual ~RpcAsyncCall();

    virtual void receive(RooshaServiceConnector *connector) = 0;

    quint32 id_;
    grpc::ClientContext context_;
    grpc::Status status_;
};

/**
 * Base struct for authorization rpc data.
 */
struct AuthorizeOrRegistrateAsyncCall : public RpcAsyncCall {
    using RpcAsyncCall::RpcAsyncCall;

    roosha::commons::Credentials request_;
    roosha::commons::AuthenticationToken response_;
};

// ----------------------------------------------------------------------------------------------------

/**
 * This struct represents 'authorize' asynchronous call.
 */
struct AuthorizeAsyncCall : public AuthorizeOrRegistrateAsyncCall {
    using AuthorizeOrRegistrateAsyncCall::AuthorizeOrRegistrateAsyncCall;

    void receive(RooshaServiceConnector *connector) override;
};

/**
 * This struct represents 'registrate' asynchronous call.
 */
struct RegistrateAsyncCall : public AuthorizeOrRegistrateAsyncCall {
    using AuthorizeOrRegistrateAsyncCall::AuthorizeOrRegistrateAsyncCall;

    void receive(RooshaServiceConnector *connector) override;
};


/**
 * This struct represents 'translate' rpc.
 */
struct TranslateAsyncCall : public RpcAsyncCall {
    using RpcAsyncCall::RpcAsyncCall;

    void receive(RooshaServiceConnector *connector) override;

    roosha::translation::TranslationRequest request_;
    roosha::translation::Translations response_;
};

/**
 * This struct represents 'proposeUserTransaltions' rpc.
 */
struct ProposeUserTranslationsAsyncCall : public RpcAsyncCall {
    using RpcAsyncCall::RpcAsyncCall;

    void receive(RooshaServiceConnector *connector) override;

    roosha::translation::UserTranslationsProposal request_;
    roosha::commons::Void response_;
};

#endif // ASYNCCLIENTCALL_H
